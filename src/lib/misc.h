/*
 * $Id$
 *
 * Copyright (c) 2001-2003, Raphael Manfredi
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
 *
 * This misc.[ch] provides several miscellaneous small routines & macros for:
 *
 * - Array size determination
 * - Flag handling
 * - Sorting constants
 * - Network related string routines
 * - Date string conversions
 * - Time string conversions
 * - Size string conversions
 * - SHA1<->base32 string conversion
 * - Tests
 * - Random numbers
 * - Stuff...
 */

#ifndef _misc_h_
#define _misc_h_

#include "config.h"				/* Needed for FreeBSD compiles */

#include <time.h>
#include <stdio.h>
#include <ctype.h>

#include <glib.h>

#define SIZE_FIELD_MAX 64		/* Max size of sprintf-ed size quantity */

/*
 * STATIC_ASSERT() can be used to verify conditions at compile-time e.g., that
 * an array has a minimum size. This is better than a run-time * assertion
 * because the condition is checked even if the code would seldomly or never
 * reached at run-time. However, this can only be used for static conditions
 * which can verified at compile-time.
 *
 * N.B.: The trick is using a switch case, if the term is false
 *	 there are two cases for zero - which is invalid C. This cannot be
 *	 used outside a function.
 */
#define STATIC_ASSERT(x) \
	do { switch (0) { case ((x) ? 1 : 0): case 0: break; } } while(0)

/*
 * Needs to be defined if we are not using Glib 2
 */
#ifndef USE_GLIB2

#ifndef HAVE_STRLCPY
size_t strlcpy(gchar *dst, const gchar *src, size_t dst_size);
#endif

#define g_ascii_strcasecmp g_strcasecmp
#define g_ascii_strncasecmp g_strncasecmp
#define g_string_printf g_string_sprintf
#define g_strlcpy strlcpy
#endif

/* Wrappers for ctype functions that allow only ASCII characters whereas
 * the locale would allow others. The parameter doesn't have to be casted
 * to (unsigned char) because isascii() is defined for all values so that
 * these macros return false for everything out of [0..127].
 *
 * GLib 2.x has similar macros/functions but defines only a subset.
 */
#define is_ascii_alnum(c) (isascii(c) && isalnum(c))
#define is_ascii_alpha(c) (isascii(c) && isalpha(c))
#ifdef isblank
#define is_ascii_blank(c) (isascii(c) && isblank(c))
#else /* !isblank */
#define is_ascii_blank(c) ((c) == ' ' || (c) == '\t')
#endif /* isblank */
#define is_ascii_cntrl(c) (isascii(c) && iscntrl(c))
#define is_ascii_digit(c) (isascii(c) && isdigit(c))
#define is_ascii_graph(c) (isascii(c) && isgraph(c))
#define is_ascii_lower(c) (isascii(c) && islower(c))
#define is_ascii_print(c) (isascii(c) && isprint(c))
#define is_ascii_punct(c) (isascii(c) && ispunct(c))
#define is_ascii_space(c) (isascii(c) && isspace(c))
#define is_ascii_upper(c) (isascii(c) && isupper(c))
#define is_ascii_xdigit(c) (isascii(c) && isxdigit(c))

/*
 * Array size determination
 */
#ifndef G_N_ELEMENTS
#define G_N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))
#endif

/* 
 * Set/clear binary flags 
 */
typedef guint16 flag_t;
#define set_flags(r,f) (r = r | (f))
#define clear_flags(r,f) (r = r & ~(f))

/*
 * Network related string routines
 */
guint32  gchar_to_ip(const gchar *);
gboolean gchar_to_ip_strict(const gchar *s, guint32 *addr, gchar const **ep);
gboolean gchar_to_ip_and_mask(const gchar *str, guint32 *ip, guint32 *netmask);
gboolean gchar_to_ip_port(const gchar *str, guint32 *ip, guint16 *port);
gboolean gchar_to_ip_and_mask(const gchar *str, guint32 *ip, guint32 *netmask);
gchar *  ip_to_gchar(guint32);
gchar *  ip2_to_gchar(guint32);
gchar *  ip_port_to_gchar(guint32, guint16);
gchar *hostname_port_to_gchar(const gchar *hostname, guint16 port);
guint32  host_to_ip(const gchar *);
gchar *  host_name(void);
#define port_is_valid(port) (port != 0)
gboolean ip_is_valid(guint32);

/*
 * Date string conversions
 */
gchar *date_to_iso_gchar(time_t date);
gchar *date_to_rfc822_gchar(time_t date);
gchar *date_to_rfc822_gchar2(time_t date);
gchar *date_to_rfc1123_gchar(time_t date);

/*
 * Time string conversions
 */
gchar *short_time(gint s);
gchar *short_uptime(gint s);

/* Use a macro so that's possible to not use difftime where it's not
 * necessary because time_t is flat encoded
 */
/* XXX: Hardcoded to difftime because there's no Configure check yet */
#if 1 || defined(USE_DIFFTIME)
#define delta_time(a, b) ((gint64) difftime((a), (b)))
#else
#define ((gint64) ((a) - (b)))
#endif

/*
 * Size string conversions
 */
gchar *short_size(guint32);
gchar *short_size64(guint64 size);
gchar *short_kb_size(guint32);
gchar *short_kb_size64(guint64 size);
gchar *compact_size(guint32 size);
gchar *compact_size64(guint64 size);
gchar *compact_kb_size(guint32 size);

/*
 * SHA1<->base32 string conversion
 */
gchar *sha1_base32(const gchar *sha1);
gchar *base32_sha1(const gchar *base32);

/*
 * GUID<->hex string conversion
 */
gchar *guid_hex_str(const gchar *guid);
gboolean hex_to_guid(const gchar *hexguid, gchar *guid);

/*
 * GUID<->base32 string conversion
 */
gchar *guid_base32_str(const gchar *guid);
gchar *base32_to_guid(const gchar *base32);

/*
 * Tests
 */
gboolean is_string_ip(const gchar *);
gboolean is_private_ip(guint32 ip);
gboolean is_absolute_path(const char *);
gboolean is_directory(const gchar *);
gboolean is_regular(const gchar *);
gboolean is_symlink(const gchar *);
gboolean file_exists(const gchar *);
guint32 next_pow2(guint32 n);

static inline gboolean is_pow2(guint32 value) G_GNUC_CONST;
static inline gboolean is_pow2(guint32 value)
{
	return value && !(value & (value - 1));
}

/*
 * Random numbers
 */
void random_init(void);
guint32 random_value(guint32 max);
void guid_random_fill(gchar *xuid);

/*
 * Stuff
 */
gint str_chomp(gchar *str, gint len);
gint hex2dec(guchar c);
gboolean is_printable(const gchar *buf, gint len);
void dump_hex(FILE *, const gchar *, gconstpointer, gint);
void strlower(gchar *, const gchar *);
void ascii_strlower(gchar *dst, const gchar *src);
gint strcmp_delimit(const gchar *a, const gchar *b, const gchar *delimit);
char *unique_filename(const gchar *path, const gchar *file, const gchar *ext);
gchar *hex_escape(const gchar *name, gboolean strict);
gint highest_bit_set(guint32 n) G_GNUC_CONST;
gfloat force_range(gfloat value, gfloat min, gfloat max);
gchar *make_pathname(const gchar *dir, const gchar *file);
gchar *short_filename(gchar *fullname);
gchar *data_hex_str(const gchar *data, size_t len);
gint create_directory(const gchar *dir);
gboolean filepath_exists(const gchar *dir, const gchar *file);
guint64 parse_uint64(const gchar *, gchar **, gint, gint *);

#ifdef HAVE_STRCASESTR
char *strcasestr(const char *haystack, const char *needle);
#else
gchar *strcasestr(const gchar *haystack, const gchar *needle);
#endif

#define NULL_STRING(s) (s != NULL ? s : "(null)")

/**
 * Swap endianness of a guint32.
 *
 * @param i the guint32 to swap
 *
 * @returns the value of i after swapping its byte order.
 */
static inline guint32
swap_guint32(guint32 i)
{
	gint a = i & 0x000000ff;
	gint b = (i & 0x0000ff00) >> 8;
	gint c = (i & 0x00ff0000) >> 16;
	gint d = (i & 0xff000000) >> 24;

	return d + (c << 8) + (b << 16) + (a << 24);
}

/*
 * Syscall wrappers for errno == 0 bug. --RAM, 27/10/2003
 */

struct stat;

extern gint do_errno;

gint do_stat(const gchar *path, struct stat *buf);

/*
 * CIDR split of IP range.
 */

typedef void (*cidr_split_t)(guint32 ip, guint8 bits, gpointer udata);

void ip_range_split(
	guint32 lower_ip, guint32 upper_ip, cidr_split_t cb, gpointer udata);

#endif /* _misc_h_ */

/* vi: set ts=4 sw=4 cindent: */
