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

#include "dstorage_lookup.h"
#include <aitown/dstorage_handle.h>

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>
#include <aitown/char_buff.h>
#include <aitown/sglib.h>
#include <aitown/utils_unused.h>

#include <kclangc.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define USE_KYOTO_CABINET 1


#define CMPARATOR(x,y) ((x->id)-(y->id))

SGLIB_DEFINE_RBTREE_PROTOTYPES(dstorage_handle_t, left, right, dstorage_handle_red_status, CMPARATOR)
SGLIB_DEFINE_RBTREE_FUNCTIONS(dstorage_handle_t, left, right, dstorage_handle_red_status, CMPARATOR)

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

void dstorage_lookup_init (dstorage_lookup_t *lku)
{
    memset (lku, 0, sizeof(dstorage_lookup_t));

#ifdef  USE_KYOTO_CABINET
    KCDB* db = kcdbnew();
    lku->database = db;
#endif
}

void dstorage_lookup_init_open (dstorage_lookup_t *lku, const char * database)
{
    dstorage_lookup_init (lku);
    dbg_message ("Initialise lookup dstorage from %s", database);
    for (;;) {
#       ifdef  USE_KYOTO_CABINET

        // attempt to open it
        KCDB* db = lku->database;
        if (!kcdbopen (db, database, KCOWRITER | KCOCREATE)) {
            err_message("open error: %s\n", kcecodename(kcdbecode(db)));
            break;
        }

        // get the number of records to see if we have to initialise it
        dstorage_id_t id = 0;
        if (kcdbcount (db) == 0) {

            // set initial ID to be assigned (1)
            dstorage_lookup_set_crt_id (lku, 1);
            if (!kcdbset (db, &id, sizeof(dstorage_id_t), &lku->header, sizeof(dstorage_cdata_t) )) {
                err_message("database error writing record 0 (header): %s",
                      kcecodename(kcdbecode(db)));
                kcdbclose (db);
                kcdbdel (db);
                db = NULL;
                break;
            }

        } else {

            // read basic informations from record 0 (0 is not a valid ID)
            size_t vsiz;

            // querry the database for header record
            dstorage_cdata_t *vbuf = (dstorage_cdata_t*)kcdbget (
                        db, (const char*)&id, sizeof(dstorage_id_t), &vsiz);
            if (vbuf == NULL) {
                err_message("database error getting record 0 (header): %s",
                      kcecodename(kcdbecode(db)));
                kcdbclose (db);
                kcdbdel (db);
                db = NULL;
                break;
            }

            // copy the data inside our header and discard database memory
            memcpy (&lku->header, vbuf, sizeof(dstorage_cdata_t));
            kcfree (vbuf);

        }
#       endif
    }
}

void dstorage_lookup_end (dstorage_lookup_t *lku)
{

    if ( lku->database != NULL) {

        // close & delete the database
#       ifdef  USE_KYOTO_CABINET
        KCDB* db = (KCDB*)lku->database;
        if (!kcdbclose (db)) {
            err_message("dstorage database close error: %s\n", kcecodename(kcdbecode(db)));
        }
        kcdbdel (db);
#       endif

    }

    // clear the structure
    memset (lku, 0, sizeof(dstorage_lookup_t));
}


dstorage_handle_t * dstorage_lookup_handle (
        dstorage_lookup_t *lku, dstorage_id_t id)
{
    dstorage_handle_t h;
    h.id = id;
    return sglib_dstorage_handle_t_find_member (lku->tree, &h);
}

dstorage_id_t dstorage_lookup_id (
        dstorage_lookup_t *lku, dstorage_handle_t *handle)
{
    dstorage_handle_t *te;
    struct sglib_dstorage_handle_t_iterator  it;
    for( te=sglib_dstorage_handle_t_it_init_inorder(&it,lku->tree); te!=NULL; te=sglib_dstorage_handle_t_it_next(&it)) {
        if (te->id == handle->id) {
            return te->id;
        }
    }
    return 0;
}

void dstorage_lookup_add (
        dstorage_lookup_t *lku, dstorage_handle_t *handle)
{
#   ifdef AITOWN_DEBUG
    DBG_ASSERT (dstorage_lookup_handle (lku, handle->id) == NULL);
#   endif

    sglib_dstorage_handle_t_add (&lku->tree, handle);
}

void dstorage_lookup_rem (
        dstorage_lookup_t *lku, dstorage_handle_t *handle)
{
#   ifdef AITOWN_DEBUG
    DBG_ASSERT (dstorage_lookup_handle (lku, handle->id) == handle);
#   endif

    sglib_dstorage_handle_t_delete (&lku->tree, handle);
}


dstorage_cdata_t * dstorage_lookup_cdata (
        dstorage_lookup_t *lku, dstorage_handle_t *handle)
{
    for (;;) {
#       ifdef  USE_KYOTO_CABINET
        KCDB* db = (KCDB*)lku->database;
        size_t vsiz;

        // querry the database for this id
        dstorage_cdata_t *vbuf = (dstorage_cdata_t*)kcdbget (
                    db, (const char*)&handle->id, sizeof(dstorage_id_t), &vsiz);
        if (vbuf == NULL) {
            err_message("database get error: %s",
                  kcecodename(kcdbecode(db)));
            dstorage_handle_mark_temp_errors (handle);
            break;
        }

        // make sure database is not corrupted
        if (vsiz != sizeof(dstorage_cdata_t)) {
            dstorage_handle_mark_temp_errors (handle);
            err_message("database error: unholy size");
            kcfree (vbuf);
            break;
        }

        return vbuf;

#       endif
    }

    return NULL;
}

void dstorage_lookup_free_cdata (dstorage_lookup_t *lku, dstorage_cdata_t *cdata)
{
#   ifdef  USE_KYOTO_CABINET
    VAR_UNUSED (lku);
    kcfree (cdata);
#   endif
}

dstorage_id_t dstorage_lookup_new_id (dstorage_lookup_t *lku)
{
    // get a new id (make sure to skip 0)
    dstorage_id_t id = 0;
    while (id == 0)
        id = dstorage_lookup_get_a_new_id (lku);


    /** @todo assign a controller */


    // save this to database right away
    if (!kcdbset (db, &id, sizeof(dstorage_id_t), &lku->header, sizeof(dstorage_cdata_t) )) {
        err_message("database error writing record 0 (header): %s",
              kcecodename(kcdbecode(db)));
        id = 0;
    }

    return id;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


