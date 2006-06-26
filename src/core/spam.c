/*
 * $Id$
 *
 * Copyright (c) 2006, Christian Biere
 * Copyright (c) 2004, Raphael Manfredi
 * Copyright (c) 2003, Markus Goetz
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
 * SHA-1 based spam filtering.
 *
 * @author Markus Goetz
 * @date 2003
 * @author Raphael Manfredi
 * @date 2004
 * @author Christian Biere
 * @date 2006
 */

#include "common.h"

RCSID("$Id$");

#ifdef HAS_SQLITE
#include "gdb.h"
#endif /* HAS_SQLITE */

#include "spam.h"
#include "settings.h"
#include "nodes.h"

#include "lib/atoms.h"
#include "lib/bit_array.h"
#include "lib/file.h"
#include "lib/getdate.h"
#include "lib/misc.h"
#include "lib/glib-missing.h"
#include "lib/walloc.h"
#include "lib/watcher.h"

#include "if/gnet_property.h"
#include "if/gnet_property_priv.h"
#include "if/bridge/c2ui.h"

#include "lib/override.h"		/* Must be the last header included */

static const gchar spam_text_file[] = "spam.txt";
static const gchar spam_what[] = "Spam database";

struct spam_lut {
#ifdef HAS_SQLITE
	struct gdb_stmt *lookup_stmt;
	struct gdb_stmt *insert_stmt;
#else  /* HAS_SQLITE */
	GHashTable *ht;
#endif /* HAS_SQLITE */
};

struct spam_lut spam_lut;

typedef enum {
	SPAM_TAG_UNKNOWN = 0,
	SPAM_TAG_ADDED,
	SPAM_TAG_END,
	SPAM_TAG_SHA1,

	NUM_SPAM_TAGS
} spam_tag_t;

struct spam_item {
	gchar sha1[SHA1_RAW_SIZE];
	time_t added;
};

static const struct spam_tag {
	spam_tag_t	tag;
	const gchar *str;
} spam_tag_map[] = {
	/* Must be sorted alphabetically for dichotomic search */

#define SPAM_TAG(x) { CAT2(SPAM_TAG_,x), STRINGIFY(x) }
	SPAM_TAG(ADDED),
	SPAM_TAG(END),
	SPAM_TAG(SHA1),

	/* Above line intentionally left blank (for "!}sort" on vi) */
#undef SPAM_TAG
};


static spam_tag_t
spam_string_to_tag(const gchar *s)
{
	STATIC_ASSERT(G_N_ELEMENTS(spam_tag_map) == (NUM_SPAM_TAGS - 1));

#define GET_ITEM(i) (spam_tag_map[(i)].str)
#define FOUND(i) G_STMT_START { \
	return spam_tag_map[(i)].tag; \
	/* NOTREACHED */ \
} G_STMT_END

	/* Perform a binary search to find ``s'' */
	BINARY_SEARCH(const gchar *, s, G_N_ELEMENTS(spam_tag_map), strcmp,
		GET_ITEM, FOUND);

#undef FOUND
#undef GET_ITEM
	return SPAM_TAG_UNKNOWN;
}

static gint
spam_db_open(struct spam_lut *lut)
#ifdef HAS_SQLITE
{
	g_assert(lut);

	{
		static const gchar cmd[] =
			"DROP TABLE IF EXISTS spam;"
			"CREATE TABLE spam (sha1 BLOB PRIMARY KEY);";
		gchar *errmsg;
		gint ret;

		ret = gdb_exec(cmd, &errmsg);
		if (0 != ret) {
			g_warning("gdb_exec() failed: %s", errmsg);
			gdb_free(errmsg);
			goto failure;
		}
		ret = gdb_declare_types("spam", "sha1", GDB_CT_SHA1, (void *) 0);
		if (0 != ret) {
			g_warning("gdb_exec() failed: %s", gdb_error_message());
			goto failure;
		}
	}

	{
		static const gchar cmd[] =
			"INSERT INTO spam VALUES($x);";
	  
		if (0 != gdb_stmt_prepare(cmd, &lut->insert_stmt)) {
			g_warning("gdb_prepare() failed: %s",
				gdb_error_message());
			goto failure;
		}
	}

	{
		static const gchar cmd[] =
			"SELECT OID FROM spam WHERE sha1 = $x;";

		if (0 != gdb_stmt_prepare(cmd, &lut->lookup_stmt)) {
			g_warning("gdb_prepare() failed: %s",
				gdb_error_message());
			goto failure;
		}
	}
	
	return 0;
	
failure:
	spam_close();
	return -1;
}
#else	/* !HAS_SQLITE */
{
	g_assert(lut);

	lut->ht = g_hash_table_new(sha1_hash, sha1_eq);
	return 0;
}
#endif	/* HAS_SQLITE */

void
spam_add(const struct spam_item *item)
{
	g_assert(item);

#ifdef HAS_SQLITE
	if (spam_lut.insert_stmt) {
		struct gdb_stmt *stmt = spam_lut.insert_stmt;
		gint ret;
		
		ret = gdb_stmt_reset(stmt);
		if (0 == ret) {
			ret = gdb_stmt_bind_static_blob(stmt,
					1, item->sha1, sizeof item->sha1);
			if (0 == ret) {
				ret = gdb_stmt_step(stmt);
				if (GDB_STEP_DONE != ret) {
					g_warning("%s: gdb_stmt_step() failed: %s",
						"spam_add", gdb_error_message());
				}
			} else {
				g_warning("%s: gdb_stmt_bind_static_blob() failed: %s",
					"spam_add", gdb_error_message());
			}
		} else {
			g_warning("%s: gdb_stmt_reset() failed: %s",
				"spam_add", gdb_error_message());
		}
	}
#else	/* HAS_SQLITE */
	if (spam_lut.ht) {
		struct spam_item *item_copy;

		item_copy = wcopy(item, sizeof *item);
		g_hash_table_insert(spam_lut.ht, &item_copy->sha1, item_copy);
	}
#endif	/* HAS_SQLITE */
}


/**
 * Load spam database from the supplied FILE.
 *
 * The current file format is as follows:
 *
 * # Comment
 * SHA1 <SHA-1>
 * ADDED <date>
 * END
 *
 * @returns the amount of entries loaded or -1 on failure.
 */
static glong
spam_load(FILE *f)
{
	static const struct spam_item zero_item;
	struct spam_item item;
	gchar line[1024];
	guint line_no = 0;
	bit_array_t tag_used[BIT_ARRAY_SIZE(NUM_SPAM_TAGS)];
	gboolean done = FALSE;
	gulong item_count = 0;

	g_assert(f);

	/* Reset state */
	done = FALSE;
	item = zero_item;
	bit_array_clear_range(tag_used, 0, (guint) NUM_SPAM_TAGS - 1);

	if (0 != spam_db_open(&spam_lut)) {
		return -1;
	}
	gdb_begin();

	while (fgets(line, sizeof line, f)) {
		const gchar *tag_name, *value;
		gchar *sp, *nl;
		gboolean damaged;
		spam_tag_t tag;

		line_no++;

		damaged = FALSE;
		nl = strchr(line, '\n');
		if (!nl) {
			/*
			 * If the line is too long or unterminated the file is either
			 * corrupt or was manually edited without respecting the
			 * exact format. If we continued, we would read from the
			 * middle of a line which could be the filename or ID.
			 */
			g_warning("spam_load(): "
				"line too long or missing newline in line %u",
				line_no);
			break;
		}
		*nl = '\0';

		/* Skip comments and empty lines */
		if (*line == '#' || *line == '\0')
			continue;

		sp = strchr(line, ' ');
		if (sp) {
			*sp = '\0';
			value = &sp[1];
		} else {
			value = strchr(line, '\0');
		}
		tag_name = line;

		tag = spam_string_to_tag(tag_name);
		g_assert((gint) tag >= 0 && tag < NUM_SPAM_TAGS);
		if (SPAM_TAG_UNKNOWN != tag && !bit_array_flip(tag_used, tag)) {
			g_warning("spam_load(): duplicate tag \"%s\" in entry in line %u",
				tag_name, line_no);
			break;
		}
		
		switch (tag) {
		case SPAM_TAG_ADDED:
			{
				time_t t;
				
				t = date2time(value, tm_time());
				if ((time_t) -1 != t) {
					item.added = t;
				} else {
					damaged |= TRUE;
				}
			}
			break;
			
		case SPAM_TAG_SHA1:
			{
				if (strlen(value) != SHA1_BASE32_SIZE) {
					damaged = TRUE;
					g_warning("spam_load(): SHA-1 has wrong length.");
				} else {
					const gchar *raw;

					raw = base32_sha1(value);
					if (!raw)
						damaged = TRUE;
					else
						memcpy(item.sha1, raw, sizeof item.sha1);
				}
			}
			break;

		case SPAM_TAG_END:
			if (!bit_array_get(tag_used, SPAM_TAG_SHA1)) {
				g_warning("spam_load(): missing SHA1 tag");
				damaged = TRUE;
			}
			if (!bit_array_get(tag_used, SPAM_TAG_ADDED)) {
				g_warning("spam_load(): missing ADDED tag");
				damaged = TRUE;
			}
			done = TRUE;
			break;

		case SPAM_TAG_UNKNOWN:
			/* Ignore */
			break;
			
		case NUM_SPAM_TAGS:
			g_assert_not_reached();
			break;
		}

		if (damaged) {
			g_warning("Damaged spam entry in line %u: "
				"tag_name=\"%s\", value=\"%s\"",
				line_no, tag_name, value);
			break;
		}

		if (done) {
			if (spam_check(item.sha1)) {
				g_warning(
					"Ignoring duplicate spam item around line %u (sha1=%s)",
				   	line_no, sha1_base32(item.sha1));
			} else {
				spam_add(&item);
				item_count++;
			}
			
			/* Reset state */
			done = FALSE;
			item = zero_item;
			bit_array_clear_range(tag_used, 0, (guint) NUM_SPAM_TAGS - 1);
		}
	}

	gdb_commit();

	return item_count;
}

/**
 * Watcher callback, invoked when the file from which we read the spam 
 * changed.
 */
static void
spam_changed(const gchar *filename, gpointer unused_udata)
{
	FILE *f;

	(void) unused_udata;

	f = file_fopen(filename, "r");
	if (f) {
		gchar buf[80];
		gulong count;

		spam_close();
		count = spam_load(f);
		fclose(f);

		gm_snprintf(buf, sizeof(buf), "Reloaded %lu spam items.", count);
		gcu_statusbar_message(buf);
	}
}

static void
spam_retrieve_from_file(FILE *f, const gchar *path, const gchar *filename)
{
	gchar *pathname;

	g_assert(f);
	g_assert(path);
	g_assert(filename);

	pathname = make_pathname(path, filename);
	watcher_register(pathname, spam_changed, NULL);
	G_FREE_NULL(pathname);
	spam_load(f);
}

/**
 * Loads the spam.txt into memory.
 *
 * The selected file will then be monitored and a reloading will occur
 * shortly after a modification.
 */
static void
spam_retrieve(void)
{
	FILE *f;
	gint idx;
	file_path_t fp[1];

	file_path_set(&fp[0], settings_config_dir(), spam_text_file);
	f = file_config_open_read_norename_chosen(spam_what,
			fp, G_N_ELEMENTS(fp), &idx);

	if (f) {
		spam_retrieve_from_file(f, fp[idx].dir, fp[idx].name);
		fclose(f);
	}
}

/**
 * Called on startup. Loads the spam.txt into memory.
 */
void
spam_init(void)
{
	spam_retrieve();
}

#ifndef HAS_SQLITE
static void
spam_item_free(gpointer unused_key, gpointer value, gpointer unused_x)
{
	struct spam_item *item = value;
	
	(void) unused_key;
	(void) unused_x;

	wfree(item, sizeof *item);
}
#endif /* HAS_SQLITE */

/**
 * Frees all entries in the spam database.
 */
void
spam_close(void)
{
#ifdef HAS_SQLITE
	gdb_stmt_finalize(&spam_lut.insert_stmt);
	gdb_stmt_finalize(&spam_lut.lookup_stmt);
#else /* HAS_SQLITE */
	if (spam_lut.ht) {
		g_hash_table_foreach(spam_lut.ht, spam_item_free, NULL);
		g_hash_table_destroy(spam_lut.ht);
		spam_lut.ht = NULL;
	}
#endif /* HAS_SQLITE */
}

/**
 * Check the given SHA-1 against the spam database.
 *
 * @param sha1 the SHA-1 to check.
 * @returns TRUE if found, and FALSE if not.
 */
gboolean
spam_check(const char *sha1)
{
	g_assert(sha1);

#ifdef HAS_SQLITE
	if (spam_lut.lookup_stmt) {
		struct gdb_stmt *stmt;
		gint ret;
	
		stmt = spam_lut.lookup_stmt;	
		ret = gdb_stmt_reset(stmt);
		if (0 == ret) {
			ret = gdb_stmt_bind_static_blob(stmt, 1, sha1, SHA1_RAW_SIZE);
			if (0 == ret) {
				enum gdb_step step;
				
				step = gdb_stmt_step(stmt);
				if (GDB_STEP_ROW == step) {
					return TRUE;
				}
				if (GDB_STEP_DONE != step) {
					g_warning("%s: gdb_step() failed: %s",
						"spam_check", gdb_error_message());
				}
			} else {
				g_warning("%s: gdb_stmt_bind_static_blob() failed: %s",
					"spam_check", gdb_error_message());
			}
		} else {
			g_warning("%s: gdb_stmt_reset() failed: %s",
				"spam_check", gdb_error_message());
		}
		return FALSE;
	}
#else	/* HAS_SQLITE */
	if (spam_lut.ht) {
		return NULL != g_hash_table_lookup(spam_lut.ht, sha1);
	}
#endif	/* HAS_SQLITE */
	return FALSE;
}

/* vi: set ts=4 sw=4 cindent: */
