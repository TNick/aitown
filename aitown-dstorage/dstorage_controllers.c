/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_controllers.c
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "dstorage_controllers.h"

#include <stdlib.h>
#include <string.h>

#include <aitown/dbg_assert.h>
#include <aitown/controllers/dstorage_ctrl_local_db.h>
#include <aitown/controllers/dstorage_ctrl_local_dir.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! list of controller callbacks
///
/// Each type of controller that the system knows of; a plug-in providing a
/// dstorage controller must register a callback here.
typedef struct _dstorage_controllers_t {
    void *      array;
    size_t		allocated;
    size_t		used;
    size_t		deleted;
} dstorage_controllers_t;

/// a structure representing one callback
typedef struct _dstorage_ctrl_entry_t {
    const char *p_name;
    dstorage_ctrl_creator kb;
} dstorage_ctrl_entry_t;

/// initial spots for callbacks
#define DSTORAGE_CONTROLLERS_KB_SLOTS 16

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static dstorage_controllers_t callbacks = {NULL, 0, 0, 0};

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

void dstorage_controllers_debug()
{
#ifdef _DEBUG
/*    printf("----------------------------------------------------------\n");
    printf("array: 0x%X, allocated %u, used %u, deleted %u\n",
           (int)callbacks.array,
           (unsigned)callbacks.allocated,
           (unsigned)callbacks.used,
           (unsigned)callbacks.deleted);

    size_t i;
    dstorage_ctrl_entry_t * p = (dstorage_ctrl_entry_t *)callbacks.array;
    for (i=0; i < callbacks.used; i++) {
        printf("entry %u: 0x%X - %s\n",
                (unsigned)i,
                (int)p->kb,
                p->p_name);
        p++;
    }*/
#endif
}

void dstorage_controllers_init ()
{
    if (callbacks.array == NULL ) {
        // clear the structure
        memset (&callbacks, 0, sizeof(dstorage_controllers_t));

        // allocate initial space
        callbacks.array = malloc(
                    sizeof(dstorage_ctrl_entry_t)*DSTORAGE_CONTROLLERS_KB_SLOTS);
        callbacks.allocated = DSTORAGE_CONTROLLERS_KB_SLOTS;
        callbacks.used = 0;
        callbacks.deleted = 0;

        // add build-in types
        dstorage_ctrl_local_dir_init();
        dstorage_ctrl_local_db_init();
    }
}

void dstorage_controllers_end ()
{
    dstorage_controllers_debug();

    if (callbacks.array != NULL) {
        size_t i;
        dstorage_ctrl_entry_t * p = (dstorage_ctrl_entry_t *)callbacks.array;
        for (i=0; i < callbacks.used; i++) {
            if (p->p_name != NULL) {
                free ((void*)p->p_name);
            }
            p++;
        }

        free (callbacks.array);
    }
    memset (&callbacks, 0, sizeof(dstorage_controllers_t));
}

static dstorage_ctrl_entry_t * dstorage_controllers_find (const char *p_name)
{
    DBG_ASSERT(callbacks.array != NULL);
    size_t i;
    dstorage_ctrl_entry_t * p = (dstorage_ctrl_entry_t *)callbacks.array;
    for (i=0; i < callbacks.used; i++) {
        if (p->p_name != NULL) {
            if (!strcasecmp (p->p_name, p_name)) {
                return p;
            }
        }
        p++;
    }
    return NULL;
}

dstorage_ctrl_creator dstorage_controllers_find_callback (const char *p_name)
{
    DBG_ASSERT(callbacks.array != NULL);
    dstorage_ctrl_entry_t * entry = dstorage_controllers_find (p_name);
    if (entry != NULL) {
        return entry->kb;
    } else {
        return NULL;
    }
}

void dstorage_controllers_add_callback (dstorage_ctrl_creator kb, const char *p_name)
{
    DBG_ASSERT(callbacks.array != NULL);
    size_t i;
    dstorage_ctrl_entry_t * entry = dstorage_controllers_find (p_name);
    if (entry != NULL) {
        entry->kb = kb;
    } else {
        dstorage_ctrl_entry_t * p = (dstorage_ctrl_entry_t *)callbacks.array;
        if (callbacks.deleted > 0) {
            for (i=0; i < callbacks.used; i++) {
                if (p->p_name == NULL) {
                    entry = p;
                    callbacks.deleted--;
                    break;
                }
                p++;
            }
            DBG_ASSERT(entry != NULL);
        } else if (callbacks.used < callbacks.allocated) {
            entry = p + callbacks.used;
            callbacks.used++;
        } else {
            DBG_ASSERT(callbacks.used == callbacks.allocated);
            p = (dstorage_ctrl_entry_t *)realloc(
                        callbacks.array,
                        (callbacks.allocated+DSTORAGE_CONTROLLERS_KB_SLOTS)*
                            sizeof(dstorage_ctrl_entry_t)
                        );
            if ( p != NULL ) {
                callbacks.array = p;
                callbacks.allocated += DSTORAGE_CONTROLLERS_KB_SLOTS;
                entry = p + callbacks.used;
                callbacks.used++;
            }
        }
        entry->kb = kb;
        entry->p_name = strdup (p_name);
    }

}

void dstorage_controllers_rem_callback (const char *p_name)
{
    DBG_ASSERT(callbacks.array != NULL);
    dstorage_ctrl_entry_t * entry = dstorage_controllers_find (p_name);
    if (entry != NULL) {
        free ((void*)entry->p_name);
        entry->p_name = NULL;
        entry->kb = NULL;
        callbacks.deleted++;
    }

}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


