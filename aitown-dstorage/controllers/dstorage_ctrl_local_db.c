/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl_local_db.c
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

#include "dstorage_ctrl_local_db.h"

#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/aitown-dstorage.h>
#include <aitown/dir_utils.h>
#include <aitown/utils.h>

#include <kclangc.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifndef PRIu64
#define PRIu64 "llu"
#endif

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

static void ctrl_ending (dstorage_ctrl_t* ctrl)
{
    // assume it's us
    dstorage_ctrl_local_db_t *ret = (dstorage_ctrl_local_db_t*)ctrl;

#   ifdef  USE_KYOTO_CABINET

    // close the database
    KCDB* db = (KCDB*)ret->database;
    if (db != NULL) {
        if (!kcdbclose (db)) {
            err_message("localdb database close error: %s", kcecodename(kcdbecode(db)));
        }
        kcdbdel (db);
    }

#   endif


    // release memory for our controller
    free (ctrl);
}

static void ctrl_read (dstorage_ctrl_param_t* req)
{
    // a dirty or used chunk should not have reach us
    DBG_ASSERT (req != NULL);
    DBG_ASSERT (req->handle != NULL);
    DBG_ASSERT (req->ctrl != NULL);
    DBG_ASSERT (req->ctrl_data != NULL);
    DBG_ASSERT (req->handle->p_data == NULL);
    DBG_ASSERT (!dstorage_handle_is_resolved(req->handle));

    // assume it's us
    dstorage_ctrl_local_db_t *ret = (dstorage_ctrl_local_db_t*)req->ctrl;
    int64_t t_start = z_clock();
    size_t buf_sz;

    // set new status
    dstorage_handle_mark_waiting (req->handle);

    dstorage_ctrl_sts_t stat;
    for (;;) {

#       ifdef  USE_KYOTO_CABINET

        // querry the database
        KCDB* db = (KCDB*)ret->database;
        if (db == NULL) {
            dstorage_handle_mark_temp_errors (req->handle);
            stat = DSTORAGE_CTRL_UNREACHABLE;
            break;
        }

        // request this key from the database
        char * ret_b = kcdbget (
                    db, (const char*)&req->handle->id,
                    sizeof(dstorage_id_t), &buf_sz );
        if (ret_b == NULL) {
            err_message("localdb requested an id we don't have: %u; %s",
                        req->handle->id, kcecodename(kcdbecode(db)));
            dstorage_handle_mark_data_lost (req->handle);
            stat = DSTORAGE_CTRL_DATA_LOST;
            break;
        }

        // get a chunk large enough
        dstorage_chunk_t * p_chunk =
                dstorage_alloc_chunk (req->ctrl->dstorage, buf_sz);
        if (p_chunk == NULL) {
            stat = DSTORAGE_CTRL_UNREACHABLE;
            kcfree (ret_b);
            break;
        }
        void * p_buff = dstorage_chunk_user(p_chunk);

        // copy the data inside and free the database buffer
        memcpy (p_buff, ret_b, buf_sz);
        kcfree (ret_b);

        // we heve retrieved the data so set new chunk in handler
        stat = DSTORAGE_CTRL_OK;
        dstorage_handle_mark_resolved (req->handle);
        req->handle->p_data = p_chunk;

#       endif
        break;
    }

    // inform the caller
    if (req->kb != NULL) {
        req->kb(stat, req);
    }

    // recompute efficiency
    int64_t eff = (((z_clock() - t_start) / buf_sz)*4 + 12 * ret->header.efficiency)/16;
    ret->header.efficiency = eff;
    ret->header.dstorage->clist.pri_dirty = 1;
}

static void ctrl_write (dstorage_ctrl_param_t* req)
{
    DBG_ASSERT (req != NULL);
    DBG_ASSERT (req->handle != NULL);
    DBG_ASSERT (req->ctrl != NULL);
    DBG_ASSERT (req->ctrl_data != NULL);
    DBG_ASSERT (req->handle->p_data != NULL);
    DBG_ASSERT (dstorage_handle_is_resolved(req->handle));

    dstorage_ctrl_sts_t stat = DSTORAGE_CTRL_OK;

    // assume it's us
    dstorage_ctrl_local_db_t *ret = (dstorage_ctrl_local_db_t*)req->ctrl;
    int64_t t_start = z_clock();
    dstorage_chunk_t *ck = req->handle->p_data;
    size_t ck_sz = ck->user_sz;

    for (;;) {
        // see if the buffer is resolved, no need to bother otherwise
        if (!dstorage_handle_is_resolved(req->handle)) {
            err_message ("localdir requested to write an unresolved id %" PRIu64, req->handle->id);
            stat = DSTORAGE_CTRL_GENERIC_ERR;
            break;
        }

#       ifdef  USE_KYOTO_CABINET

        // querry the database
        KCDB* db = (KCDB*)ret->database;
        if (db == NULL) {
            stat = DSTORAGE_CTRL_UNREACHABLE;
            break;
        }

        // and save
        if (!kcdbset (
                db, (const char *)&req->handle->id, sizeof(dstorage_id_t),
                (const char *)dstorage_chunk_user(ck),
                ck_sz )) {
            err_message("localdb error writing id %u: %s",
                        (unsigned)req->handle->id, kcecodename(kcdbecode(db)));
            stat = DSTORAGE_CTRL_CANT_WRITE;
            break;
        }
#       endif

        // clear dirty flag for this handle
        dstorage_handle_mark_clean (req->handle);

        break;
    }

    // inform the caller
    if (req->kb != NULL) {
        req->kb(stat, req);
    }

    // recompute efficiency
    int64_t eff = (((z_clock() - t_start) / ck_sz)*4 + 12 * ret->header.efficiency)/16;
    ret->header.efficiency = eff;
    ret->header.dstorage->clist.pri_dirty = 1;
}

static dstorage_ctrl_t * creator(dstorage_t *dstorage, struct_ini_sect_t *settings)
{
    // get the database path from settings
    struct_ini_value_t * entry;
    entry = struct_ini_find_value_0 (settings, "path");
    if ((entry == NULL) ||
        (entry->value == NULL)) {

        err_message ("dstore controller: localdir needs rw target path in settings");
        return NULL;
    }

    // create the instance
    dstorage_ctrl_local_db_t * ret = (dstorage_ctrl_local_db_t*)malloc(
                sizeof(dstorage_ctrl_local_db_t));

    // prepare main structure
    dstorage_ctrl_init(&ret->header, dstorage);
    ret->header.ending = ctrl_ending;
    ret->header.read = ctrl_read;
    ret->header.write = ctrl_write;

#   ifdef  USE_KYOTO_CABINET

    // open the database
    KCDB* db = kcdbnew();
    ret->database = db;
    if (!kcdbopen (db, entry->value, KCOWRITER | KCOCREATE)) {
        err_message("open error: %s", kcecodename(kcdbecode(db)));
        kcdbdel (db);
        dstorage_ctrl_end (&ret->header);
        free (ret);
        return NULL;
    }

#   endif



    return (dstorage_ctrl_t *)ret;
}

void dstorage_ctrl_local_db_init ()
{
    dstorage_controllers_add_callback (creator, "localdb");
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


