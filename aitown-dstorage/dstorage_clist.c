/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_handle.c
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

#include "aitown-dstorage.h"

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>
#include <aitown/char_buff.h>
#include <aitown/utils_unused.h>
#include <aitown/struct_ini.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define DSTORAGE_CLIST_STEPU 16

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

static dstorage_ctrl_t ** dstorage_clist_add_new (dstorage_clist_t *clist)
{
    dstorage_ctrl_t ** p = (dstorage_ctrl_t **)clist->array;
    if (clist->used < clist->allocated) {
        p = p + clist->used;
        clist->used++;
    } else {
        DBG_ASSERT(clist->used == clist->allocated);
        p = (dstorage_ctrl_t **)realloc(
                    clist->array,
                    (clist->allocated+DSTORAGE_CLIST_STEPU)*
                        sizeof(dstorage_ctrl_t*)
                    );
        if ( p != NULL ) {
            clist->array = p;
            clist->allocated += DSTORAGE_CLIST_STEPU;
            p = p + clist->used;
            clist->used++;
        }
    }
    return p;
}

void dstorage_clist_init (dstorage_t * dstorage)
{
    dstorage_clist_t *clist = &dstorage->clist;
    struct_ini_t* cfg = &dstorage->sini;

    // clear the structure
    memset (clist, 0, sizeof(dstorage_clist_t));

    // allocate a buffer for the array
    clist->array = malloc(
                sizeof(dstorage_ctrl_t*)*DSTORAGE_CLIST_STEPU);
    clist->allocated = DSTORAGE_CLIST_STEPU;
    clist->used = 0;

    // read the list of controllers from config
    char namebuff[128];
    struct_ini_sect_t * sect;
    struct_ini_value_t * entry;
    const char *name;
    unsigned i;
    dstorage_ctrl_creator kb;
    dstorage_ctrl_t ** slot;
    dstorage_ctrl_t * controller;
    for (i=1; i< 65000; i++) {

        // check if the controller at this index exists
        sprintf(namebuff,"controller/%u", i);
        sect = struct_ini_find_section_0 (cfg, namebuff);
        if (sect == NULL) {
            break;
        }

        // it must have a name
        entry = struct_ini_find_value_0 (sect,"name");
        if ((entry != NULL) && (entry->value != NULL)) {
            name = entry->value;
        } else {
            err_message ("dstorage controller %u has no name", i);
            break;
        }

        // does it require a plug-in?
        entry = struct_ini_find_value_0 (sect,"pluigin");
        if ((entry != NULL) && (entry->value != NULL)) {
            /** @todo load plugin; don't break on errors */

        }

        // now look for the callback of this controller
        kb = dstorage_controllers_find_callback (name);
        if (kb == NULL) {
            log_message ("No way to create controller %u (%s)", i, name);
            controller = NULL;
        } else {
            controller = kb(dstorage, sect);
        }

        // add controller or NULL to array
        slot = dstorage_clist_add_new (clist);
        if (slot == NULL) {
            err_message ("Memeory allocation eror for dstorage controller %u", i);
            break;
        }
        *slot = controller;
    }
}

static int remove_all (dstorage_ctrl_t* itr, void* user)
{
    VAR_UNUSED (user);
    if (itr != NULL) {
        itr->ending (itr);
    }
    return 1;
}

void dstorage_clist_end (dstorage_clist_t *clist)
{

    if (clist->array != NULL) {
        // loop in structures and call end, then free the array
        dstorage_clist_foreach (clist, remove_all, NULL);

        free (clist->array);
    }

    memset (clist, 0, sizeof(dstorage_clist_t));
}

int dstorage_clist_foreach (
        dstorage_clist_t *clist, dstorage_clist_iterator kb, void* user)
{
    // see if there is an array
    dstorage_ctrl_t ** iter = (dstorage_ctrl_t**)clist->array;
    if (iter == NULL)
        return 1;
    // iterate it
    unsigned i;
    unsigned i_max = clist->used;
    int result = 1;
    for (i=0; i<i_max; i++ ) {
        result = kb (*iter, user);
        if (result == 0)
            break;
        iter++;
    }
    return result;
}

dstorage_ctrl_t* dstorage_clist_get (dstorage_clist_t *clist, unsigned index)
{
    // if no entry nothing to report
    if (clist->array == NULL)
        return NULL;

    // if fewer entries, same
    if (clist->used < index) {
        return NULL;
    }

    // may still  be null
    dstorage_ctrl_t ** p = (dstorage_ctrl_t**)clist->array;
    return p[index];
}

/* FUNCTIONS =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


