/*
 * $Id$
 *
 * Copyright (c) 2003, Raphael Manfredi
 *
 * Functions that should be in glib-1.2 but are not.
 * They are all prefixed with "gm_" as in "Glib Missing".
 *
 * We also include FIXED versions of glib-1.2 routines that are broken
 * and make sure those glib versions are never called directly.
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

#ifndef _glib_missing_h_
#define _glib_missing_h_

#include <sys/types.h>
#include <glib.h>

/*
 * Public interface.
 */

GSList *gm_slist_insert_after(GSList *list, GSList *lnk, gpointer data);

GList *gm_list_insert_after(GList *list, GList *lnk, gpointer data);

#ifndef USE_GTK2
GList *g_list_delete_link(GList *l, GList *lnk);
#endif /* !USE_GTK */

size_t gm_vsnprintf(gchar *str, size_t n, gchar const *fmt, va_list args);
size_t gm_snprintf(gchar *str, size_t n, gchar const *fmt, ...)
	G_GNUC_PRINTF (3, 4);

gchar *gm_strconcatv(const gchar *s, va_list args);
void gm_savemain(gint argc, gchar **argv, gchar **env);
void gm_setproctitle(gchar *title);
unsigned long gm_atoul(const char *str, char **endptr, int *errorcode);

#define GM_STRCONCAT_NULL(x, ...) g_strconcat(x, ## __VA_ARGS__, NULL);

/* NB: Sub-statement func is evaluated more than once! */
#define G_LIST_FOREACH(list, func, user_data) \
	do { \
		GList *_l = (list); \
		gpointer _user_data = (user_data); \
		while (NULL != _l) { \
			(*(func))(_l->data, _user_data); \
			_l = g_list_next(_l); \
		} \
	} while(0)

#define G_LIST_FOREACH_SWAPPED(list, func, user_data) \
	do { \
		GList *_l = (list); \
		gpointer _user_data = (user_data); \
		while (NULL != _l) { \
			(*(func))(_user_data, _l->data); \
			_l = g_list_next(_l); \
		} \
	} while(0)

/* NB: Sub-statement func is evaluated more than once! */
#define G_SLIST_FOREACH(slist, func, user_data) \
	do { \
		GSList *_sl = (slist); \
		gpointer _user_data = (user_data); \
		while (NULL != _sl) { \
			(*(func))(_sl->data, _user_data); \
			_sl = g_slist_next(_sl); \
		} \
	} while(0)

/* NB: Sub-statement func is evaluated more than once! */
#define G_SLIST_FOREACH_SWAPPED(slist, func, user_data) \
	do { \
		GSList *_sl = (slist); \
		gpointer _user_data = (user_data); \
		while (NULL != _sl) { \
			(*(func))(_user_data, _sl->data); \
			_sl = g_slist_next(_sl); \
		} \
	} while(0)


#endif	/* _glib_missing_h_ */
