/*
 * $Id$
 *
 * Copyright (c) 2002-2003, Raphael Manfredi
 *
 *----------------------------------------------------------------------
 * This file is part of gtk-gnutella.
 *
 *  gtk-gnutella is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  gtk-gnutella is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gtk-gnutella; if not, write to the Free Software
 *  Foundation, Inc.:
 *      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *----------------------------------------------------------------------
 */

/**
 * @ingroup core
 * @file
 *
 * Asychronous file moving operations.
 *
 * @author Raphael Manfredi
 * @date 2002-2003
 */

#include "common.h"

RCSID("$Id$")

#include "downloads.h"
#include "fileinfo.h"
#include "move.h"

#include "lib/atoms.h"
#include "lib/bg.h"
#include "lib/file.h"
#include "lib/misc.h"
#include "lib/tm.h"
#include "lib/walloc.h"

#include "if/gnet_property.h"
#include "if/gnet_property_priv.h"

#include "lib/override.h"	/* Must be the last header included */

#define COPY_BLOCK_SHIFT	12			/**< Power of two of copy unit credit */
#define COPY_BUF_SIZE		65536		/**< Size of the reading buffer */

static struct bgtask *move_daemon;

#define MOVED_MAGIC	0x00c0b100

/**
 * Moving daemon context.
 */
struct moved {
	gint magic;				/**< Magic number */
	struct download *d;		/**< Download for which we're moving file */
	gchar *buffer;			/**< Large buffer, where data is read */
	gchar *target;			/**< Target file name, in case an error occurs */
	time_t start;			/**< Start time, to determine copying rate */
	filesize_t size;		/**< Size of file */
	filesize_t copied;		/**< Amount of data copied so far */
	gint rd;				/**< Opened file descriptor for read, -1 if none */
	gint wd;				/**< Opened file descriptor for write, -1 if none */
	gint error;				/**< Error code */
};

/**
 * Work queue entry.
 */
struct work {
	struct download *d;		/**< Download to move */
	const gchar *dest;		/**< Target directory (atom) */
	const gchar *ext;		/**< Trailing extension (atom) */
};

/**
 * Allocate work queue entry.
 */
static struct work *
we_alloc(struct download *d, const gchar *dest, const gchar *ext)
{
	struct work *we;

	we = walloc(sizeof(*we));
	we->d = d;
	we->dest = atom_str_get(dest);
	we->ext = atom_str_get(ext);

	return we;
}

/**
 * Freeing of work queue entry.
 */
static void
we_free(gpointer data)
{
	struct work *we = data;

	atom_str_free_null(&we->dest);
	atom_str_free_null(&we->ext);
	wfree(we, sizeof(*we));
}

/**
 * Signal handler for termination.
 */
static void
d_sighandler(struct bgtask *unused_h, gpointer u, bgsig_t sig)
{
	struct moved *md = u;

	(void) unused_h;
	g_assert(md->magic == MOVED_MAGIC);

	switch (sig) {
	case BG_SIG_TERM:
		/*
		 * Get rid of incompletely moved file.  Moving will be resumed
		 * when we are relaunched.
		 */

		if (md->target != NULL && -1 == unlink(md->target))
			g_warning("cannot unlink \"%s\": %s",
				md->target, g_strerror(errno));
		break;
	default:
		break;
	}
}

/**
 * Freeing of computation context.
 */
static void
d_free(gpointer ctx)
{
	struct moved *md = ctx;

	g_assert(md->magic == MOVED_MAGIC);

	if (md->rd != -1) {
		close(md->rd);
		md->rd = -1;
	}
	if (md->wd != -1) {
		close(md->wd);
		md->wd = -1;
	}
	g_free(md->buffer);
	md->magic = 0;
	wfree(md, sizeof(*md));
}

/**
 * Daemon's notification of start/stop.
 */
static void
d_notify(struct bgtask *unused_h, gboolean on)
{
	(void) unused_h;
	gnet_prop_set_boolean_val(PROP_FILE_MOVING, on);
}

/**
 * Daemon's notification: starting to work on item.
 */
static void
d_start(struct bgtask *h, gpointer ctx, gpointer item)
{
	struct moved *md = ctx;
	struct work *we = item;
	struct download *d = we->d;
	struct stat buf;
	const gchar *name;

	g_assert(md->magic == MOVED_MAGIC);
	g_assert(md->rd == -1);
	g_assert(md->wd == -1);
	g_assert(md->target == NULL);

	download_move_start(d);
	bg_task_signal(h, BG_SIG_TERM, d_sighandler);

	md->d = we->d;
	md->rd = file_open(download_pathname(d), O_RDONLY);

	if (md->rd == -1) {
		md->error = errno;
		goto abort_read;
	}

	if (-1 == fstat(md->rd, &buf)) {
		md->error = errno;
		g_warning("can't fstat \"%s\": %s",
			download_pathname(d), g_strerror(errno));
		goto abort_read;
	}

	if (!S_ISREG(buf.st_mode)) {
		g_warning("file \"%s\" is not a regular file", download_pathname(d));
		goto abort_read;
	}

	/*
	 * Don't keep an URN-like name when the file is done, if possible.
	 */

	name = file_info_readable_filename(d->file_info);

	md->target = unique_filename(we->dest, name, we->ext, NULL);
	if (NULL == md->target)
		goto abort_read;

	md->wd = file_create(md->target, O_WRONLY | O_TRUNC, buf.st_mode);

	if (md->wd == -1)
		goto abort_read;

	md->start = tm_time();
	md->size = download_filesize(d);
	md->copied = 0;
	md->error = 0;

	if (GNET_PROPERTY(dbg) > 1)
		g_message("Moving \"%s\" to \"%s\"",
				download_basename(d), md->target);

	return;

abort_read:
	md->error = errno;
	if (md->rd != -1) {
		close(md->rd);
		md->rd = -1;
	}
	g_warning("can't copy \"%s\" to \"%s\"", download_pathname(d), we->dest);
	return;
}

/**
 * Daemon's notification: finished working on item.
 */
static void
d_end(struct bgtask *h, gpointer ctx, gpointer item)
{
	struct moved *md = ctx;
	struct download *d = md->d;
	gint elapsed = 0;

	g_assert(md->magic == MOVED_MAGIC);
	g_assert(md->d == ((struct work *) item)->d);

	bg_task_signal(h, BG_SIG_TERM, NULL);

	if (md->rd == -1) {			/* Did not start properly */
		g_assert(md->error);
		goto finish;
	}

	close(md->rd);
	md->rd = -1;

	close(md->wd);
	md->wd = -1;

	/*
	 * If copying went well, get rid of the source file.
	 *
	 * If an error occurred, the target file is removed, whilst the source
	 * file is kept intact.
	 */

	if (md->error == 0) {
		g_assert(md->copied == md->size);

		if (-1 == unlink(download_pathname(md->d)))
			g_warning("cannot unlink \"%s\": %s",
				download_basename(md->d), g_strerror(errno));
	} else {
		if (md->target != NULL && -1 == unlink(md->target))
			g_warning("cannot unlink \"%s\": %s",
				md->target, g_strerror(errno));
	}

	elapsed = delta_time(tm_time(), md->start);
	elapsed = MAX(1, elapsed);		/* time warp? clock not monotic? */

	if (GNET_PROPERTY(dbg) > 1)
		printf("Moved file \"%s\" at %lu bytes/sec [error=%d]\n",
			download_basename(md->d), (gulong) md->size / elapsed, md->error);

finish:
	if (md->error == 0) {
		file_info_mark_stripped(d->file_info);
		download_move_done(d, md->target, elapsed);
	} else
		download_move_error(d);

	G_FREE_NULL(md->target);
}

/**
 * Copy file around, incrementally.
 */
static bgret_t
d_step_copy(struct bgtask *h, gpointer u, gint ticks)
{
	struct moved *md = u;
	ssize_t r;
	size_t amount;
	guint64 remain;
	gint used;

	g_assert(md->magic == MOVED_MAGIC);

	if (md->rd == -1)			/* Could not open the file */
		return BGR_DONE;		/* Computation done */

	if (md->size == 0)			/* Empty file */
		return BGR_DONE;

	g_assert(md->size > md->copied);
	remain = md->size - md->copied;

	/*
	 * Each tick we have can buy us 2^COPY_BLOCK_SHIFT bytes.
	 *
	 * We read into a COPY_BUF_SIZE bytes buffer, and at most md->size
	 * bytes total, to stop before the fileinfo trailer.
	 */

	amount = ticks << COPY_BLOCK_SHIFT;
	remain = MIN(remain, COPY_BUF_SIZE);
	amount = MIN(amount, remain);

	g_assert(amount > 0);

	r = read(md->rd, md->buffer, amount);
	if ((ssize_t) -1 == r) {
		md->error = errno;
		g_warning("error while reading \"%s\" for moving: %s",
			download_basename(md->d), g_strerror(errno));
		return BGR_DONE;
	} else if (r == 0) {
		g_warning("EOF while reading \"%s\" for moving!",
			download_basename(md->d));
		md->error = -1;
		return BGR_DONE;
	}

	/*
	 * Any partially read block counts as one block, hence the second term.
	 */

	used = (r >> COPY_BLOCK_SHIFT) +
		((r & ((1 << COPY_BLOCK_SHIFT) - 1)) ? 1 : 0);

	if (used != ticks)
		bg_task_ticks_used(h, used);

	r = write(md->wd, md->buffer, amount);
	if ((ssize_t) -1 == r) {
		md->error = errno;
		g_warning("error while writing for moving \"%s\": %s",
			download_basename(md->d), g_strerror(errno));
		return BGR_DONE;
	} else if ((size_t) r < amount) {
		md->error = -1;
		g_warning("short write whilst moving \"%s\"", download_basename(md->d));
		return BGR_DONE;
	}

	g_assert((size_t) r == amount);

	md->copied += r;
	download_move_progress(md->d, md->copied);

	return md->copied == md->size ? BGR_DONE : BGR_MORE;
}

/**
 * Enqueue completed download file for verification.
 */
void
move_queue(struct download *d, const gchar *dest, const gchar *ext)
{
	struct work *we;

	we = we_alloc(d, dest, ext);
	bg_daemon_enqueue(move_daemon, we);
}

/**
 * Initializes the background moving/copying task.
 */
void
move_init(void)
{
	struct moved *md;
	bgstep_cb_t step = d_step_copy;

	md = walloc(sizeof(*md));
	md->magic = MOVED_MAGIC;
	md->rd = -1;
	md->wd = -1;
	md->buffer = g_malloc(COPY_BUF_SIZE);
	md->target = NULL;

	move_daemon = bg_daemon_create("file moving",
		&step, 1,
		md, d_free,
		d_start, d_end, we_free,
		d_notify);
}

/**
 * Called at shutdown time.
 */
void
move_close(void)
{
	bg_task_cancel(move_daemon);
}

/* vi: set ts=4 sw=4 cindent: */
