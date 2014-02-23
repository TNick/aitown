/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl_local_dir.c
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

#include "dstorage_ctrl_local_dir.h"

#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/aitown-dstorage.h>
#include <aitown/dir_utils.h>

#include <stdio.h>
#include <inttypes.h>

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
    dstorage_ctrl_local_dir_t * ret = (dstorage_ctrl_local_dir_t*)ctrl;
    free (ret->p_path);

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

    int64_t t_start = z_clock();

    // set new status
    dstorage_handle_mark_waiting (req->handle);
    dstorage_ctrl_local_dir_t * ret = (dstorage_ctrl_local_dir_t*)req->ctrl;
    sprintf(ret->p_file, "%" PRIu64, req->handle->id);

    long filesz = 0;
    dstorage_ctrl_sts_t stat;
    for (;;) {

        // open this file or fail
        FILE *f = fopen (ret->p_path, "r");
        if (f == NULL) {
            dbg_message ("localdir has no such id %" PRIu64, req->handle->id);
            stat = DSTORAGE_CTRL_DATA_LOST;
            dstorage_handle_mark_temp_errors (req->handle);
            break;
        }

        // get the size
        fseek (f,0,SEEK_END);
        filesz = ftell (f);
        fseek (f,0,SEEK_SET);

        // get a chunk large enough
        dstorage_chunk_t * p_chunk =
                dstorage_alloc_chunk (req->ctrl->dstorage, filesz);
        if (p_chunk == NULL) {
            stat = DSTORAGE_CTRL_UNREACHABLE;
            break;
        }
        void * p_buff = dstorage_chunk_user(p_chunk);

        // read inside this chunk
        size_t read_sz = fread (p_buff, 1, filesz, f);
        if (read_sz != filesz) {
            stat = DSTORAGE_CTRL_UNREACHABLE;
            dstorage_handle_mark_temp_errors (req->handle);
            dstorage_chunk_mng_free (&req->ctrl->dstorage->ckmng, &p_chunk);
            break;
        }

        // we heve retrieved the data so set new chunk in handler
        stat = DSTORAGE_CTRL_OK;
        dstorage_handle_mark_resolved (req->handle);
        req->handle->p_data = p_chunk;

        break;
    }

    // inform the caller
    if (req->kb != NULL) {
        req->kb(stat, req);
    }

    // recompute efficiency
    int64_t eff = (((z_clock() - t_start) / filesz)*4 + 12 * ret->header.efficiency)/16;
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

    // assume it's us
    dstorage_ctrl_local_dir_t * ret = (dstorage_ctrl_local_dir_t*)req->ctrl;
    int64_t t_start = z_clock();
    dstorage_chunk_t *ck = req->handle->p_data;
    size_t ck_sz = ck->user_sz;
    dstorage_ctrl_sts_t stat = DSTORAGE_CTRL_UNREACHABLE;

    for (;;) {

        // see if the buffer is resolved, no need to bother otherwise
        if (!dstorage_handle_is_resolved(req->handle)) {
            err_message ("localdir requested to write an unresolved id %" PRIu64, req->handle->id);
            stat = DSTORAGE_CTRL_GENERIC_ERR;
            break;
        }

        // get the name of the file in buffer and open it for writing
        sprintf(ret->p_file, "%" PRIu64, req->handle->id);
        FILE *f = fopen (ret->p_path, "w");
        if (f == NULL) {
            dbg_message ("localdir can't create id %" PRIu64, req->handle->id);
            stat = DSTORAGE_CTRL_CANT_WRITE;
            break;
        }

        // actual write
        if ( 1 != fwrite (
                    dstorage_chunk_user (ck),
                    ck_sz, 1, f))
        {
            dbg_message ("localdir can't write id %" PRIu64, req->handle->id);
            stat = DSTORAGE_CTRL_CANT_WRITE;
            fclose (f); f = NULL;
            break;
        }

        // we're done with this file
        fclose (f); f = NULL;

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
    // get the path from settings
    struct_ini_value_t * entry;
    entry = struct_ini_find_value_0 (settings, "path");
    if ((entry == NULL) ||
        (entry->value == NULL) ||
        (du_mkpath(entry->value) != FUNC_OK)) {

        err_message ("dstore controller: localdir needs rw target path in settings");
        return NULL;
    }

    // create a buffer large enough
    size_t path_sz = strlen (entry->value);
    char * path = (char*)malloc (path_sz + 32);
    if (path == NULL) {
        err_message ("dstore controller: localdir memory allocation error");
        return NULL;
    }

    // and copy the path inside
    memcpy (path, entry->value, path_sz);
    if (path[path_sz-1] != '/') {
        path[path_sz] = '/';
        path_sz++;
    }
    path[path_sz] = 0;

    // create the instance
    dstorage_ctrl_local_dir_t * ret = (dstorage_ctrl_local_dir_t*)malloc(
                sizeof(dstorage_ctrl_local_dir_t));

    // prepare main structure
    dstorage_ctrl_init(&ret->header, dstorage);
    ret->header.ending = ctrl_ending;
    ret->header.read = ctrl_read;
    ret->header.write = ctrl_write;

    // buffer
    ret->p_path = path;
    ret->p_file = path+path_sz;
    *ret->p_file = 0;

    return (dstorage_ctrl_t *)ret;
}

void dstorage_ctrl_local_dir_init ()
{
    dstorage_controllers_add_callback (creator, "localdir");
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


