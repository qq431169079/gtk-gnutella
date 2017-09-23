[= AutoGen5 Template =][=
#
# Autogen template file to generate the property files.
#
# Copyright (c) 2001-2003, Richard Eckart
# Copyright (c) 2013, Raphael Manfredi
#
=][=
(define license (sprintf
"/*
 * Copyright (c) 2001-2003, Richard Eckart
 *
 * THIS FILE IS AUTOGENERATED! DO NOT EDIT!
 * This file is generated from %s using autogen.
 * Autogen is available at http://autogen.sourceforge.net/.
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
 */" (def-file)))
(define prop-max (sprintf "%s_MAX" (string-upcase (get "property_set"))))
(define prop-min (sprintf "%s_MIN" (string-upcase (get "property_set"))))
(define prop-num (sprintf "%s_NUM" (string-upcase (get "property_set"))))
(define prop-end (sprintf "%s_END" (string-upcase (get "property_set"))))
(define private-src (get "private_src"))
(define set-name-down (string-downcase (get "property_set")))
(define set-name-up (string-upcase (get "property_set")))
(define prop-set (. set-name-down))
(define prop-array (sprintf "%s->props" (. prop-set)))
(define prop-offset (get "offset"))
(define (type_ok? type)
    (cond
        ((= type "boolean") #t)
        ((= type "guint32") #t)
        ((= type "guint64") #t)
        ((= type "ip") #t)
        ((= type "string") #t)
        ((= type "storage") #t)
        ((= type "timestamp") #t)
        ((= type "multichoice") #t)
        (else #f)))
(define func-prefix
	(if (exist? "func_prefix") (get "func_prefix") (. prop-set)))
=][=
IF (not (exist? "property_set"))=][=
    (error "property set has no name")=][=
ENDIF=][=
IF (not (exist? "offset"))=][=
    (error "no offset for property numbering given")=][=
ENDIF=]
Generating files:
[= (sprintf "%s.h" (. set-name-down)) =]
[= (sprintf "%s_priv.h" (. set-name-down)) =]
[= (sprintf "%s.c" (. set-name-down)) =]

[=
(out-switch (sprintf "%s.h" (. set-name-down)))
(. license)
=]

#ifndef _[=(. set-name-down)=]_h_
#define _[=(. set-name-down)=]_h_

#include "lib/prop.h"
#include "lib/log.h"		/* For s_carp() */

#define [=(. prop-min)=] ([=offset=])
#define [=(. prop-max)=] ([=offset=]+[=(. prop-end)=]-1)
#define [=(. prop-num)=] ([=(. prop-end)=]-[=offset=])

typedef enum {[=
    FOR prop =]
    PROP_[=(string-upcase (get "name"))=][=
        IF (= (for-index) 0)=]=[=(. prop-offset)=][=ENDIF=],[=
    ENDFOR prop =]
    [=(. prop-end)=]
} [= (. set-name-down) =]_t;

/*
 * Property set stub
 */
const prop_set_stub_t *[=(. func-prefix)=]_get_stub(void);

/*
 * Property definition
 */
prop_def_t *[=(. func-prefix)=]_get_def(property_t);
property_t [=(. func-prefix)=]_get_by_name(const char *);
pslist_t *[=(. func-prefix)=]_get_by_regex(const char *, int *);
const char *[=(. func-prefix)=]_name(property_t);
const char *[=(. func-prefix)=]_type_to_string(property_t);
const char *[=(. func-prefix)=]_to_string(property_t prop);
const char *[=(. func-prefix)=]_to_typed_string(property_t prop);
const char *[=(. func-prefix)=]_default_to_string(property_t);
const char *[=(. func-prefix)=]_description(property_t);
gboolean [=(. func-prefix)=]_is_saved(property_t);
gboolean [=(. func-prefix)=]_is_internal(property_t);
prop_type_t [=(. func-prefix)=]_type(property_t);
void [=(. func-prefix)=]_set_from_string(property_t, const char *);

void [=(. func-prefix)=]_lock(property_t);
void [=(. func-prefix)=]_unlock(property_t);

/*
 * Property-change listeners
 */
void [=(. func-prefix)=]_add_prop_changed_listener(
    property_t, prop_changed_listener_t, gboolean);
void [=(. func-prefix)=]_remove_prop_changed_listener(
    property_t, prop_changed_listener_t);

/*
 * get/set functions
 *
 * The *_val macros are shortcuts for single scalar properties.
 */
void [=(. func-prefix)=]_set_boolean(
    property_t, const gboolean *, size_t, size_t);
gboolean *[=(. func-prefix)=]_get_boolean(
    property_t, gboolean *, size_t, size_t);

static inline void
[=(. func-prefix)=]_set_boolean_val(property_t p, gboolean value)
{
	[=(. func-prefix)=]_set_boolean(p, &value, 0, 1);
}

static inline void
[=(. func-prefix)=]_get_boolean_val(property_t p, gboolean *value_ptr)
{
	[=(. func-prefix)=]_get_boolean(p, value_ptr, 0, 1);
}


void [=(. func-prefix)=]_set_string(property_t, const char *);
char *[=(. func-prefix)=]_get_string(property_t, char *, size_t);

void [=(. func-prefix)=]_set_guint32(
    property_t, const guint32 *, size_t, size_t);
guint32 *[=(. func-prefix)=]_get_guint32(
    property_t, guint32 *, size_t, size_t);

static inline void
[=(. func-prefix)=]_set_guint32_val(property_t p, guint32 value)
{
	[=(. func-prefix)=]_set_guint32(p, &value, 0, 1);
}

static inline void
[=(. func-prefix)=]_get_guint32_val(property_t p, guint32 *value_ptr)
{
	[=(. func-prefix)=]_get_guint32(p, value_ptr, 0, 1);
}

static inline void
[=(. func-prefix)=]_incr_guint32(property_t p)
{
	guint32 value;
	[=(. func-prefix)=]_lock(p);
	[=(. func-prefix)=]_get_guint32_val(p, &value);
	if G_UNLIKELY((guint32) -1 == value) {
		s_carp("%s(): incrementing property \"%s\" would overflow",
			G_STRFUNC, [=(. func-prefix)=]_name(p));
	} else {
		value++;
		[=(. func-prefix)=]_set_guint32_val(p, value);
	}
	[=(. func-prefix)=]_unlock(p);
}

static inline void
[=(. func-prefix)=]_decr_guint32(property_t p)
{
	guint32 value;
	[=(. func-prefix)=]_lock(p);
	[=(. func-prefix)=]_get_guint32_val(p, &value);
	if G_UNLIKELY(0 == value) {
		s_carp("%s(): decrementing property \"%s\" would underflow",
			G_STRFUNC, [=(. func-prefix)=]_name(p));
	} else {
		value--;
		[=(. func-prefix)=]_set_guint32_val(p, value);
	}
	[=(. func-prefix)=]_unlock(p);
}

void [=(. func-prefix)=]_set_guint64(
    property_t, const guint64 *, size_t, size_t);
guint64 *[=(. func-prefix)=]_get_guint64(
    property_t, guint64 *, size_t, size_t);

static inline void
[=(. func-prefix)=]_set_guint64_val(property_t p, guint64 value)
{
	[=(. func-prefix)=]_set_guint64(p, &value, 0, 1);
}

static inline void
[=(. func-prefix)=]_get_guint64_val(property_t p, guint64 *value_ptr)
{
	[=(. func-prefix)=]_get_guint64(p, value_ptr, 0, 1);
}

void [=(. func-prefix)=]_set_timestamp(
    property_t, const time_t *, size_t, size_t);
time_t *[=(. func-prefix)=]_get_timestamp(
    property_t, time_t *, size_t, size_t);

static inline void
[=(. func-prefix)=]_set_timestamp_val(property_t p, time_t value)
{
	[=(. func-prefix)=]_set_timestamp(p, &value, 0, 1);
}

static inline void
[=(. func-prefix)=]_get_timestamp_val(property_t p, time_t *value_ptr)
{
	[=(. func-prefix)=]_get_timestamp(p, value_ptr, 0, 1);
}

void [=(. func-prefix)=]_set_ip(
    property_t, const host_addr_t *, size_t, size_t);
host_addr_t *[=(. func-prefix)=]_get_ip(
    property_t, host_addr_t *, size_t, size_t);

static inline void
[=(. func-prefix)=]_set_ip_val(property_t p, host_addr_t value)
{
	[=(. func-prefix)=]_set_ip(p, &value, 0, 1);
}

static inline void
[=(. func-prefix)=]_get_ip_val(property_t p, host_addr_t *value_ptr)
{
	[=(. func-prefix)=]_get_ip(p, value_ptr, 0, 1);
}

void [=(. func-prefix)=]_set_storage(property_t, gconstpointer, size_t);
gpointer [=(. func-prefix)=]_get_storage(property_t, gpointer, size_t);

#endif /* _[=(. set-name-down)=]_h_ */

/* vi: set ts=4 sw=4 cindent: */
[=
(out-switch (sprintf "%s_priv.h" (. set-name-down)))
(. license)
=]

#ifndef _[=(. set-name-down)=]_priv_h_
#define _[=(. set-name-down)=]_priv_h_

#include <glib.h>

#include "lib/prop.h"

#ifdef [=(. private-src)=]

#define [=(. set-name-up)=](name) 	([=(. set-name-down)=]_variable_ ## name)
#define [=(. set-name-up)=]_PTR(name)	(&[=(. set-name-down)=]_variable_ ## name)

/*
 * Includes specified by "uses"-statement in .ag file
 */
[= FOR uses =]#include "[=uses=]"
[= ENDFOR uses =]

[= FOR prop =][=
(define item (sprintf "%s_variable_%s"
		(. set-name-down)
		(if (exist? "data.value")
			(get "data.value") (string-downcase (get "name")))))
=][=
CASE type=][=
= boolean=]extern const gboolean [=(. item)=][=
= guint32=]extern const guint32  [=(. item)=][=
= guint64=]extern const guint64  [=(. item)=][=
= ip     =]extern const host_addr_t  [=(. item)=][=
= multichoice=]extern const guint32  [=(. item)=][=
= string =]extern const char   *[=(. item)=][=
= storage=]extern const char    [=(. item)=][=
= timestamp=]extern const time_t  [=(. item)=][=
ESAC =][=
IF (exist? "vector_size") =][[=vector_size=]][=ENDIF=];
[= ENDFOR prop =]

prop_set_t *[=(. func-prefix)=]_init(void);
void [=(. func-prefix)=]_shutdown(void);

#endif /* [=(. private-src)=] */

#endif /* _[=(. set-name-down)=]_priv_h_ */

/* vi: set ts=4 sw=4 cindent: */
[=
(out-switch (sprintf "%s.c" (. set-name-down)))
(. license)
=]

[=	IF (= (get "property_set") "gui_property")	=]
[=	(sprintf "#ifndef USE_TOPLESS")	=]
[=	ENDIF	=]

#include "lib/prop.h"
#include "lib/eval.h"
#include "lib/mutex.h"
#include "lib/omalloc.h"
#include "lib/pslist.h"

#include "[=(sprintf "%s.h" (. set-name-down))=]"

/*
 * Includes specified by "uses"-statement in .ag file
 */
[= FOR uses =]#include "[=uses=]"
[= ENDFOR uses =]
#include "lib/override.h"		/* Must be the last header included */

/*
 * Some of the generated const variables can be unused, avoid warnings.
 *
 * The -Wunused-const-variable warning may not be available with a particular
 * compiler, so the -Wpragmas that is issued before prevents a warning to be
 * emitted in that case...
 */
G_IGNORE(-Wunknown-pragmas);		/* clang does not know -Wpragmas */
G_IGNORE(-Wpragmas);				/* For gcc, not clang */
G_IGNORE(-Wunused-const-variable);	/* Appears in clang version 3.4.1 */
[=
FOR prop =][=
	(define item (sprintf "%s_variable_%s"
		(. set-name-down)
		(if (exist? "data.value")
			(get "data.value") (string-downcase (get "name")))))=][=
    IF (= (get "type") "storage")=]
char   [=(. item)=][[=vector_size=]];
static const char   [=(. item)=]_default[[=vector_size=]];
[=
    ELIF (= (get "type") "ip")=]
host_addr_t  [=(. item)=];[=
    ELSE=][=
		(define vtype (cond
            ((= (get "type") "boolean")		"gboolean ")
            ((= (get "type") "guint32")		"guint32  ")
            ((= (get "type") "guint64")		"guint64  ")
            ((= (get "type") "multichoice")	"guint32  ")
            ((= (get "type") "timestamp")	"time_t  ")
            ((= (get "type") "ip")			"host_addr_t  ")
            ((= (get "type") "string")		"char   *")
			)
		)

		(define vdef (cond
            ((= (get "type") "boolean")		(get "data.default"))
            ((= (get "type") "guint32")		(get "data.default"))
            ((= (get "type") "guint64")		(get "data.default"))
            ((= (get "type") "multichoice")	(get "data.default"))
            ((= (get "type") "timestamp")	(get "data.default"))
            ((= (get "type") "string")
				(if (= (get "data.default") "NULL")
					(sprintf "NULL")
					(sprintf "\"%s\"" (get "data.default"))
				))
			)
		)
            =][=
        IF (exist? "vector_size")=]
[=  (. vtype)=][=(. item)=][[=vector_size=]]     = [=(. vdef)=];
static const [=  (. vtype)=][=(. item)=]_default[[=vector_size=]] = [=(. vdef)=];[=
        ELSE=]
[=  (. vtype)=][=(. item)=]     = [=(. vdef)=];
static const [=  (. vtype)=][=(. item)=]_default = [=(. vdef)=];[=
        ENDIF=][=
    IF (= (get "type") "multichoice")=]
prop_def_choice_t [=(. item)=]_choices[] = {[=
            FOR choice =]
    {N_("[=name=]"), [=value=]},[=
            ENDFOR choice =]
    {NULL, 0}
};[=
    ENDIF=][=
    ENDIF=][=
ENDFOR prop =]

static prop_set_t *[=(. prop-set)=];

prop_set_t * G_COLD
[=(. func-prefix)=]_init(void) {
    guint32 n;

    [=(. prop-set)=] = omalloc(sizeof(prop_set_t));
    [=(. prop-set)=]->name   = "[=property_set=]";
    [=(. prop-set)=]->desc   = "";
    [=(. prop-set)=]->size   = [=(. prop-num)=];
    [=(. prop-set)=]->offset = [=offset=];
    [=(. prop-set)=]->mtime  = 0;
    OMALLOC_ARRAY([=(. prop-set)=]->props, [=(. prop-num)=]);
    [=(. prop-set)=]->get_stub = [=(. func-prefix)=]_get_stub;
    [=(. prop-set)=]->dirty = FALSE;
    [=(. prop-set)=]->by_name = NULL;
	spinlock_init(&[=(. prop-set)=]->lock);[=

FOR prop =][=
    (define current-prop (sprintf "%s[%u]"
        (. prop-array) (for-index)))

    (if (not (and (exist? "type") (type_ok? (get "type"))))
        (error "type missing or invalid"))

    (if (not (exist? "name"))
        (error "no name given"))

    (if (not (exist? "desc"))
        (error "no description given"))

    (define prop-var-name
		(if (exist? "data.value")
			(get "data.value")
			(string-downcase (get "name"))
		)
	)

    (if (and (not (exist? "data.default"))
	(not (or (= (get "type") "storage") (= (get "type") "ip"))))
        (error "no default value given"))

    (if (and (not (exist? "vector_size")) (= (get "type") "storage"))
        (error "must give vector_size for a storage-type property"))
    =]


    /*
     * PROP_[=(string-upcase (get "name"))=]:
     *
     * General data:
     */
    [= IF (exist? "cfgvar") =][=
        (. current-prop) =].name = "[= cfgvar =]";[=
    ELSE =][=
        (. current-prop) =].name = "[= name =]";[=
    ENDIF =]
    [=(. current-prop)=].desc = _("[=desc=]");
    [=(. current-prop)=].ev_changed = event_new("[= name =]_changed");[=
    IF (exist? "save") =]
    [=  (. current-prop) =].save = [=save=];[=
    ELSE =]
    [=  (. current-prop) =].save = TRUE;[=
    ENDIF =][=
    IF (exist? "internal") =]
    [=  (. current-prop) =].internal = [=internal=];[=
    ELSE =]
    [=  (. current-prop) =].internal = FALSE;[=
    ENDIF =][=
    IF (exist? "vector_size") =]
    [=  (. current-prop) =].vector_size = [=vector_size=];[=
        (define prop-var	(sprintf "%s_variable_%s"
					(. prop-set) (. prop-var-name)))
	(define prop-def-var	(sprintf "(void *) %s_default" (. prop-var)))
    =][= ELSE =]
    [=  (. current-prop) =].vector_size = 1;[=
        (define prop-var	(sprintf "(void *) &%s_variable_%s"
					(. prop-set) (. prop-var-name)))
	(define prop-def-var	(sprintf "%s_default" (. prop-var)))
    =][= ENDIF =]
	mutex_init(&[=  (. current-prop) =].lock);

    /* Type specific data: */[=
    CASE type =][=

    = boolean =][=
    IF (not (exist? "data.default")) =][=
        (error "no default given")=][=
    ENDIF=]
    [=(. current-prop)=].type               = PROP_TYPE_BOOLEAN;
    [=(. current-prop)=].data.boolean.def   = [=(. prop-def-var)=];
    [=(. current-prop)=].data.boolean.value = [=(. prop-var)=];[=

    = storage =]
    [=(. current-prop)=].type               = PROP_TYPE_STORAGE;
    [=(. current-prop)=].data.storage.value = [=(. prop-var)=];[=

    = guint32 =]
    [=(. current-prop)=].type               = PROP_TYPE_GUINT32;
    [=(. current-prop)=].data.guint32.def   = [=(. prop-def-var)=];
    [=(. current-prop)=].data.guint32.value = [=(. prop-var)=];
    [=(. current-prop)=].data.guint32.choices = NULL;[=
    IF (exist? "data.max")=]
    [=(. current-prop)=].data.guint32.max   = [=data.max=];[=
    ELSE=]
    [=(. current-prop)=].data.guint32.max   = 0xFFFFFFFF;[=
    ENDIF=][=
    IF (exist? "data.min")=]
    [=(. current-prop)=].data.guint32.min   = [=data.min=];[=
    ELSE=]
    [=(. current-prop)=].data.guint32.min   = 0x00000000;[=
    ENDIF=][=

    = guint64 =]
    [=(. current-prop)=].type               = PROP_TYPE_GUINT64;
    [=(. current-prop)=].data.guint64.def   = [=(. prop-def-var)=];
    [=(. current-prop)=].data.guint64.value = [=(. prop-var)=];
    [=(. current-prop)=].data.guint64.choices = NULL;[=
    IF (exist? "data.max")=]
    [=(. current-prop)=].data.guint64.max   = [=data.max=];[=
    ELSE=]
    [=(. current-prop)=].data.guint64.max   = (guint64) -1;[=
    ENDIF=][=
    IF (exist? "data.min")=]
    [=(. current-prop)=].data.guint64.min   = [=data.min=];[=
    ELSE=]
    [=(. current-prop)=].data.guint64.min   = 0x0000000000000000;[=
    ENDIF=][=

    = timestamp =]
    [=(. current-prop)=].type               = PROP_TYPE_TIMESTAMP;
    [=(. current-prop)=].data.timestamp.def   = [=(. prop-def-var)=];
    [=(. current-prop)=].data.timestamp.value = [=(. prop-var)=];
    [=(. current-prop)=].data.timestamp.choices = NULL;[=
    IF (exist? "data.max")=]
    [=(. current-prop)=].data.timestamp.max   = [=data.max=];[=
    ELSE=]
    [=(. current-prop)=].data.timestamp.max   = (time_t) ((1U << 31) - 1);[=
    ENDIF=][=
    IF (exist? "data.min")=]
    [=(. current-prop)=].data.timestamp.min   = [=data.min=];[=
    ELSE=]
    [=(. current-prop)=].data.timestamp.min   = 0x0000000000000000;[=
    ENDIF=][=

	= ip =]
    [=(. current-prop)=].type               = PROP_TYPE_IP;
    [=(. current-prop)=].data.ip.value = [=(. prop-var)=];[=

    = multichoice =]
    [=(. current-prop)=].type               = PROP_TYPE_MULTICHOICE;
    [=(. current-prop)=].data.guint32.def   = [=(. prop-def-var)=];
    [=(. current-prop)=].data.guint32.value = [=(. prop-var)=];
    [=(. current-prop)=].data.guint32.max   = 0xFFFFFFFF;
    [=(. current-prop)=].data.guint32.min   = 0x00000000;
    [=(. current-prop)=].data.guint32.choices = [=
        (sprintf "%s_choices" (. prop-var))=];[=

    = string =]
    [=(. current-prop)=].type               = PROP_TYPE_STRING;
    [=(. current-prop)=].data.string.def    = [=(. prop-def-var)=];
    [=(. current-prop)=].data.string.value  = [=(. prop-var)=];
    if ([=(. current-prop)=].data.string.def) {
        *[=(. current-prop)=].data.string.value =
            g_strdup(eval_subst(*[=(. current-prop)=].data.string.def));
    }[=
    ESAC =][=
ENDFOR prop=]

    [=(. prop-set)=]->by_name = htable_create(HASH_KEY_STRING, 0);
    for (n = 0; n < [=(. prop-num)=]; n ++) {
        htable_insert([=(. prop-set)=]->by_name,
            [=(. prop-array)=][n].name, int_to_pointer(n+[=offset=]));
    }

    return [=(. prop-set)=];
}

/**
 * Free memory allocated by the property set.
 */
void G_COLD
[=(. func-prefix)=]_shutdown(void) {
    guint32 n;

    htable_free_null(&[=(. prop-set)=]->by_name);

    for (n = 0; n < [=(. prop-num)=]; n ++) {
        if ([=(. prop-set)=]->props[n].type == PROP_TYPE_STRING) {
			char **p = [=(. prop-array)=][n].data.string.value;
            struct event *e = [=(. prop-array)=][n].ev_changed;
	    G_FREE_NULL(*p);
            if (e)
                event_destroy(e);
        }
    }

	/*
	 * We don't free [=(. prop-array)=] and [=(. prop-set)=].
	 * They are allocated via omalloc().
	 *
	 * Prevent further access to the now shutdown properties, that's all.
	 */

	[=(. prop-set)=] = NULL;
}

prop_def_t *
[=(. func-prefix)=]_get_def(property_t p)
{
    return prop_get_def([=(. prop-set)=], p);
}

/**
 * Lock property.
 */
void
[=(. func-prefix)=]_lock(property_t p)
{
    prop_lock([=(. prop-set)=], p);
}

/**
 * Unlock property.
 */
void
[=(. func-prefix)=]_unlock(property_t p)
{
    prop_unlock([=(. prop-set)=], p);
}

/**
 * Add a change listener to a given property. If init is TRUE then
 * the listener is immediately called.
 */
void
[=(. func-prefix)=]_add_prop_changed_listener(
    property_t prop, prop_changed_listener_t l, gboolean init)
{
    prop_add_prop_changed_listener([=(. prop-set)=], prop, l, init);
}

/**
 * Add a change listener to a given property. If init is TRUE then
 * the listener is immediately called.
 */
void
[=(. func-prefix)=]_add_prop_changed_listener_full(
    property_t prop, prop_changed_listener_t l, gboolean init,
    enum frequency_type freq, guint32 interval)
{
    prop_add_prop_changed_listener_full([=(. prop-set)=], prop, l, init,
        freq, interval);
}

void
[=(. func-prefix)=]_remove_prop_changed_listener(
    property_t prop, prop_changed_listener_t l)
{
    prop_remove_prop_changed_listener([=(. prop-set)=], prop, l);
}

void
[=(. func-prefix)=]_set_boolean(
    property_t prop, const gboolean *src, size_t offset, size_t length)
{
    prop_set_boolean([=(. prop-set)=], prop, src, offset, length);
}

gboolean *
[=(. func-prefix)=]_get_boolean(
    property_t prop, gboolean *t, size_t offset, size_t length)
{
    return prop_get_boolean([=(. prop-set)=], prop, t, offset, length);
}

void
[=(. func-prefix)=]_set_guint32(
    property_t prop, const guint32 *src, size_t offset, size_t length)
{
    prop_set_guint32([=(. prop-set)=], prop, src, offset, length);
}

guint32 *
[=(. func-prefix)=]_get_guint32(
    property_t prop, guint32 *t, size_t offset, size_t length)
{
    return prop_get_guint32([=(. prop-set)=], prop, t, offset, length);
}

void
[=(. func-prefix)=]_set_guint64(
    property_t prop, const guint64 *src, size_t offset, size_t length)
{
    prop_set_guint64([=(. prop-set)=], prop, src, offset, length);
}

guint64 *
[=(. func-prefix)=]_get_guint64(
    property_t prop, guint64 *t, size_t offset, size_t length)
{
    return prop_get_guint64([=(. prop-set)=], prop, t, offset, length);
}

void
[=(. func-prefix)=]_set_timestamp(
    property_t prop, const time_t *src, size_t offset, size_t length)
{
    prop_set_timestamp([=(. prop-set)=], prop, src, offset, length);
}

time_t *
[=(. func-prefix)=]_get_timestamp(
    property_t prop, time_t *t, size_t offset, size_t length)
{
    return prop_get_timestamp([=(. prop-set)=], prop, t, offset, length);
}

void
[=(. func-prefix)=]_set_ip(
    property_t prop, const host_addr_t *src, size_t offset, size_t length)
{
    prop_set_ip([=(. prop-set)=], prop, src, offset, length);
}

host_addr_t *
[=(. func-prefix)=]_get_ip(
    property_t prop, host_addr_t *t, size_t offset, size_t length)
{
    return prop_get_ip([=(. prop-set)=], prop, t, offset, length);
}


void
[=(. func-prefix)=]_set_string(property_t prop, const char *val)
{
    prop_set_string([=(. prop-set)=], prop, val);
}

char *
[=(. func-prefix)=]_get_string(property_t prop, char *t, size_t size)
{
    return prop_get_string([=(. prop-set)=], prop, t, size);
}

void
[=(. func-prefix)=]_set_storage(property_t p, gconstpointer v, size_t l)
{
    prop_set_storage([=(. prop-set)=], p, v, l);
}

gpointer
[=(. func-prefix)=]_get_storage(property_t p, gpointer t, size_t l)
{
    return prop_get_storage([=(. prop-set)=], p, t, l);
}

const char *
[=(. func-prefix)=]_to_typed_string(property_t prop)
{
    return prop_to_typed_string([=(. prop-set)=], prop);
}

const char *
[=(. func-prefix)=]_to_string(property_t prop)
{
    return prop_to_string([=(. prop-set)=], prop);
}

const char *
[=(. func-prefix)=]_default_to_string(property_t prop)
{
    return prop_default_to_string([=(. prop-set)=], prop);
}

const char *
[=(. func-prefix)=]_name(property_t p)
{
    return prop_name([=(. prop-set)=], p);
}

prop_type_t
[=(. func-prefix)=]_type(property_t p)
{
    return prop_type([=(. prop-set)=], p);
}

const char *
[=(. func-prefix)=]_type_to_string(property_t p)
{
    return prop_type_to_string([=(. prop-set)=], p);
}

const char *
[=(. func-prefix)=]_description(property_t p)
{
    return prop_description([=(. prop-set)=], p);
}

gboolean
[=(. func-prefix)=]_is_saved(property_t p)
{
    return prop_is_saved([=(. prop-set)=], p);
}

gboolean
[=(. func-prefix)=]_is_internal(property_t p)
{
    return prop_is_internal([=(. prop-set)=], p);
}

property_t
[=(. func-prefix)=]_get_by_name(const char *name)
{
    return pointer_to_uint(htable_lookup([=(. prop-set)=]->by_name, name));
}

pslist_t *
[=(. func-prefix)=]_get_by_regex(const char *pattern, int *error)
{
    return prop_get_by_regex([=(. prop-set)=], pattern, error);
}

void
[=(. func-prefix)=]_set_from_string(property_t prop, const char *val)
{
	prop_set_from_string([=(. prop-set)=], prop, val, FALSE);
}

/**
 * Returns a new stub struct for this property set. Just g_free it
 * when it is no longer needed. All fields are read only!
 */
const prop_set_stub_t *
[=(. func-prefix)=]_get_stub(void)
{
	static prop_set_stub_t stub;
	static gboolean inited;

	if G_LIKELY(inited)
		return &stub;

	stub.size    = [=(. prop-num)=];
	stub.offset  = [=(. prop-min)=];
	stub.get_def = [=(. func-prefix)=]_get_def;
	stub.get_by_name = [=(. func-prefix)=]_get_by_name;
	stub.to_string = [=(. func-prefix)=]_to_string;

	stub.prop_changed_listener.add =
		[=(. func-prefix)=]_add_prop_changed_listener;
	stub.prop_changed_listener.add_full =
		[=(. func-prefix)=]_add_prop_changed_listener_full;
	stub.prop_changed_listener.remove =
		[=(. func-prefix)=]_remove_prop_changed_listener;

	stub.boolean.get = [=(. func-prefix)=]_get_boolean;
	stub.boolean.set = [=(. func-prefix)=]_set_boolean;

	stub.guint32.get = [=(. func-prefix)=]_get_guint32;
	stub.guint32.set = [=(. func-prefix)=]_set_guint32;

	stub.guint64.get = [=(. func-prefix)=]_get_guint64;
	stub.guint64.set = [=(. func-prefix)=]_set_guint64;

	stub.string.get = [=(. func-prefix)=]_get_string;
	stub.string.set = [=(. func-prefix)=]_set_string;

	stub.storage.get = [=(. func-prefix)=]_get_storage;
	stub.storage.set = [=(. func-prefix)=]_set_storage;

	stub.timestamp.get = [=(. func-prefix)=]_get_timestamp;
	stub.timestamp.set = [=(. func-prefix)=]_set_timestamp;

	stub.ip.get = [=(. func-prefix)=]_get_ip;
	stub.ip.set = [=(. func-prefix)=]_set_ip;

	inited = TRUE;
	return &stub;
}

[=	IF (= (get "property_set") "gui_property")	=]
[=	(sprintf "#endif /* !USE_TOPLESS */")	=]
[=	ENDIF	=]

/* vi: set ts=4 sw=4 cindent: */
