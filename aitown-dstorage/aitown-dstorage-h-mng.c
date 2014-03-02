/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-h-mng.c
  \date			February 2014
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
#include "aitown-dstorage-h-mng.h"
#include "aitown-dstorage-handle.h"
#include "aitown-dstorage-data.h"
#include "aitown-dstorage-handle.h"
#include "aitown-dstorage-controller.h"

#include <aitown/dbg_assert.h>
#include <aitown/utils.h>
#include <aitown/sglib.h>
#include <aitown/utils_unused.h>

#include <aitown/aitown-cfg.h>
#include <aitown/aitown-db.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-free.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define AITOWN_DSTORAGE_HANDLE_CMPARATOR(x,y) ((x->id)-(y->id))

SGLIB_DEFINE_RBTREE_PROTOTYPES(aitown_dstorage_handle_t, left, right, dstorage_handle_red_status_unsafe, AITOWN_DSTORAGE_HANDLE_CMPARATOR)
SGLIB_DEFINE_RBTREE_FUNCTIONS(aitown_dstorage_handle_t, left, right, dstorage_handle_red_status_unsafe, AITOWN_DSTORAGE_HANDLE_CMPARATOR)


static void aitown_dstorage_handle_free_buffer (aitown_dstorage_handle_t *handle);

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


func_error_t aitown_dstorage_h_mng_init (
        aitown_db_mng_t * dbmng, aitown_dstorage_h_mng_t *mng,
        aitown_cfg_sect_t * dstorage_sect)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // we should have been cleared by the caller
        DBG_ASSERT (mng->tree == NULL);
        DBG_ASSERT (mng->free == NULL);
        DBG_ASSERT (mng->handle_max == 0);
        DBG_ASSERT (mng->free_max == 0);
        DBG_ASSERT (mng->db == NULL);

        // get our section
        aitown_cfg_sect_t * sect_handlers = aitown_cfg_get_sect (
                    dstorage_sect, "handlers");
        if (sect_handlers == NULL) {
            err_message ("DStorage [handlers] section missing from config file");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // the leafs we're interested in
        aitown_cfg_leaf_t * leaf_db = aitown_cfg_get_leaf (
                    sect_handlers, "database");
        aitown_cfg_leaf_t * leaf_handle_max = aitown_cfg_get_leaf (
                    sect_handlers, "handle_max");
        aitown_cfg_leaf_t * leaf_free_max = aitown_cfg_get_leaf (
                    sect_handlers, "free_max");
        aitown_cfg_leaf_t * leaf_cache_max = aitown_cfg_get_leaf (
                    sect_handlers, "cache_max");
        aitown_cfg_leaf_t * leaf_next_id = aitown_cfg_get_leaf (
                    sect_handlers, "next_id");


        // open this database
        if ((leaf_db == NULL) || (leaf_db->value == NULL)) {
            err_message ("[dstorage/handlers] section:  missing 'database' key from config file");
            ret = FUNC_BAD_INPUT;
            break;
        }
        ret = aitown_db_open (dbmng, leaf_db->value, &mng->db);
        if (ret != FUNC_OK) break;

        // set maximum number of handlers
        if ((leaf_handle_max != NULL) && (leaf_handle_max->value != NULL)) {
            mng->handle_max = (unsigned)atoi (leaf_handle_max->value);
        }
        if (mng->handle_max == 0) mng->handle_max = UINT_MAX;

        // set maximum number of free handlers
        if ((leaf_free_max != NULL) && (leaf_free_max->value != NULL)) {
            mng->free_max = (unsigned)atoi (leaf_free_max->value);
        }
        if (mng->free_max == 0) mng->free_max = 256;

        // set maximum number of cached handlers
        if ((leaf_cache_max != NULL) && (leaf_cache_max->value != NULL)) {
            mng->cache_max = (unsigned)atoi (leaf_cache_max->value);
        }
        if (mng->cache_max == 0) mng->cache_max = 256;

        // next id to be used
        if ((leaf_next_id != NULL) && (leaf_next_id->value != NULL)) {
            mng->next_id = (unsigned)atoi (leaf_next_id->value);
        }
        if (mng->next_id == 0) mng->next_id = 1;

        break;
    }


    return ret;
}

void aitown_dstorage_h_mng_end (
        aitown_dstorage_h_mng_t *h_mng,
        aitown_cfg_sect_t * dstorage_sect)
{
    DBG_ASSERT (h_mng != NULL);
    VAR_UNUSED (dstorage_sect);

    // iterate in entire tree
    struct sglib_aitown_dstorage_handle_t_iterator iterator;
    aitown_dstorage_handle_t * handle_next;
    aitown_dstorage_handle_t * handle =
            sglib_aitown_dstorage_handle_t_it_init (
                &iterator, h_mng->tree);
    while (handle != NULL) {
        handle_next = sglib_aitown_dstorage_handle_t_it_next (&iterator);

        aitown_dstorage_handle_free_buffer (handle);
        free (handle);

        handle = handle_next;
    }

    // save next ID
    if (dstorage_sect != NULL) {

        aitown_cfg_sect_t * sect_handlers = aitown_cfg_get_or_create_sect (
                    dstorage_sect, "handlers");
        if (sect_handlers != NULL) {

            aitown_cfg_leaf_t * leaf_next_id = aitown_cfg_get_or_create_leaf (
                        sect_handlers, "next_id");
            if (leaf_next_id != NULL) {
                char textbuf[64];
                sprintf (textbuf, "%llu", (long long unsigned int)h_mng->next_id);
                aitown_cfg_set_leaf (leaf_next_id, textbuf);
            }
        }
    }

    // close the database
    if (h_mng->db != NULL) {
        aitown_db_close (&h_mng->db);
    }

}

void aitown_dstorage_h_mng_handle_incref (
        aitown_dstorage_handle_t *handle, void * owner)
{
    DBG_ASSERT (handle != NULL);
    VAR_UNUSED (owner);
    ++handle->ref_count;
}

static void aitown_dstorage_handle_free_buffer (aitown_dstorage_handle_t *handle)
{
    if (handle->data != NULL) {

        // write the data back if is dirty
        if (aitown_dstorage_handle_dirty (handle)) {
            aitown_dstorage_controller_set (handle);
        }

        // either use the database or our own allocator
        if (aitown_dstorage_handle_local_mem (handle)) {
            aitown_dstorage_data_free (&handle->data);
        } else {
            DBG_ASSERT (handle->ctrl != NULL);
            DBG_ASSERT (handle->ctrl->db != NULL);
            aitown_db_free (handle->ctrl->db, (const void**)&handle->data);
        }
    }
    aitown_dstorage_handle_set_clean (handle);
    aitown_dstorage_handle_set_status (handle, DSTORAGE_H_NO_DATA);
}


void aitown_dstorage_h_mng_handle_decref (
        aitown_dstorage_h_mng_t * h_mng, aitown_dstorage_handle_t *handle,
        void * owner)
{
    VAR_UNUSED (owner);
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (handle != NULL);

    // release the reference
    --handle->ref_count;
    if (handle->ref_count <= 0) {

        // keep it in cache, if possible
        if (h_mng->cache_count < h_mng->cache_max) {
            ++h_mng->cache_count;
            return;
        }

        // release associated buffer, if any
        aitown_dstorage_handle_free_buffer (handle);

        // remove it from the tree
        sglib_aitown_dstorage_handle_t_delete (&h_mng->tree, handle);

        if (h_mng->free_count < h_mng->free_max) {

            // free it to pool
            ++h_mng->free_count;
            handle->right = h_mng->free;
            h_mng->free = handle;

        } else {

            // free for real
            free (handle);

        }
        --h_mng->handle_count;
    }
}

void aitown_dstorage_handle_free_cache (
        aitown_dstorage_h_mng_t * h_mng)
{
    if (h_mng->cache_count == 0) {
        return;
    }

    // iterate in entire tree
    struct sglib_aitown_dstorage_handle_t_iterator iterator;
    aitown_dstorage_handle_t * handle =
            sglib_aitown_dstorage_handle_t_it_init (
                &iterator, h_mng->tree);
    while (handle != NULL) {
        // find those handles that have reference count == 0
        if (aitown_dstorage_handle_refcount (handle) <= 0) {

            // release their buffer, if any
            aitown_dstorage_handle_free_buffer (handle);

            // remove it from the tree
            sglib_aitown_dstorage_handle_t_delete (&h_mng->tree, handle);

            // add it to the list of free handles
            handle->right = h_mng->free;
            h_mng->free = handle;

            // adjust counters
            ++h_mng->free_count;
            --h_mng->cache_count;

            // exit as soon as possible
            if  ((h_mng->free_count >= h_mng->free_max) ||
                 (h_mng->cache_count == 0)) {

                return;
            }
        }
        handle = sglib_aitown_dstorage_handle_t_it_next (&iterator);
    }
}

func_error_t aitown_dstorage_handle_new (
        aitown_dstorage_h_mng_t *h_mng, aitown_dstorage_id_t id,
        aitown_dstorage_handle_t ** handle_ptr)
{
    DBG_ASSERT (h_mng != NULL);
    DBG_ASSERT (id != AITOWN_DSTORAGE_ID_INVALID);
    DBG_ASSERT ((h_mng->free == NULL) == (h_mng->free_count == 0));
    DBG_ASSERT ((h_mng->tree == NULL) ? 1 : (h_mng->handle_count > 0));
    DBG_ASSERT (h_mng->handle_count >= h_mng->free_count);
    DBG_ASSERT (h_mng->handle_count >= h_mng->cache_count);

    aitown_dstorage_handle_t * handle = NULL;
    func_error_t ret = FUNC_OK;

    for (;;) {

        // prefer the ones in the pool
        if (h_mng->free != NULL) {
            handle = h_mng->free;
            h_mng->free = h_mng->free->right;
            --h_mng->free_count;

            // get some from cache
        } else if (h_mng->cache_count > 0) {
            aitown_dstorage_handle_free_cache (h_mng);
            DBG_ASSERT (h_mng->free_count > 0);

            handle = h_mng->free;
            h_mng->free = h_mng->free->right;
            --h_mng->free_count;

            // allocate a new one
        } else if (h_mng->handle_count < h_mng->handle_max) {
            handle = (aitown_dstorage_handle_t*)malloc (
                        sizeof(aitown_dstorage_handle_t));
            if (handle == NULL) {
                ret = FUNC_MEMORY_ERROR;
                break;
            }
            ++h_mng->handle_count;
        }

        // reinit the handle
        aitown_dstorage_handle_init (handle, id);

        // add it in the tree
        sglib_aitown_dstorage_handle_t_add (&h_mng->tree, handle);

        break;
    }

    *handle_ptr = handle;
    return ret;
}

aitown_dstorage_handle_t * aitown_dstorage_h_mng_get_handle (
        aitown_dstorage_h_mng_t *h_mng, aitown_dstorage_id_t id)
{
    DBG_ASSERT (h_mng != NULL);
    DBG_ASSERT (id != AITOWN_DSTORAGE_ID_INVALID);

    aitown_dstorage_handle_t * handle = NULL;
    func_error_t ret = FUNC_OK;

    for (;;) {

        // find it among the ones already loaded
        aitown_dstorage_handle_t fake; fake.id = id;
        handle = sglib_aitown_dstorage_handle_t_find_member(h_mng->tree, &fake);
        if (handle != NULL) {
            aitown_dstorage_h_mng_handle_incref (handle, handle);
            break;
        }

        // allocate a new handle
        ret = aitown_dstorage_handle_new (h_mng, id, &handle);
        if (ret != FUNC_OK) {
            err_message ("dstorage failed to allocate a new handle");
            break;
        }

        // read information from id database
        size_t data_sz;
        const void * data;
        aitown_db_read_t param;
        param.db = h_mng->db;
        param.key = &handle->id;
        param.key_sz = sizeof(aitown_dstorage_id_t);
        param.val = &data;
        param.val_sz = &data_sz;
        param.kb = NULL;
        param.user = NULL;
        ret = aitown_db_read_ex (&param);
        if (ret != FUNC_OK) {
            aitown_dstorage_handle_set_status (
                        handle, DSTORAGE_H_TEMP_FAILURE);
        } else {
            aitown_dstorage_handle_set_status (
                        handle, DSTORAGE_H_NO_DATA);
        }
        aitown_dstorage_h_mng_handle_incref (handle, handle);

        break;
    }
    return handle;
}

aitown_dstorage_handle_t *aitown_dstorage_h_mng_new_handle(
        aitown_dstorage_h_mng_t *h_mng, aitown_dstorage_controller_t * ctrl,
        unsigned size)
{
    DBG_ASSERT (h_mng != NULL);

    if (ctrl == NULL) {
        err_message ("DStorage: could not create new handle; "
                     "appropriate controller not found");
        return NULL;
    }

    aitown_dstorage_handle_t * handle = NULL;
    func_error_t ret = FUNC_OK;
    aitown_dstorage_id_t id = h_mng->next_id++;

    for (;;) {

        // allocate a new handle
        ret = aitown_dstorage_handle_new (h_mng, id, &handle);
        if (ret != FUNC_OK) {
            err_message ("dstorage failed to allocate a new handle");
            break;
        }
        // already inserted in chain

        // set it up
        handle->ctrl = ctrl;
        aitown_dstorage_handle_set_status (
                    handle, DSTORAGE_H_NO_DATA);
        aitown_dstorage_h_mng_handle_incref (handle, handle);
        aitown_dstorage_handle_set_dirty (handle);
        aitown_dstorage_handle_set_mem_local (handle);

        // allocate a buffer
        ret = aitown_dstorage_data_new  (&handle->data, size);
        if (ret == FUNC_OK) {
            aitown_dstorage_handle_set_status (
                        handle, DSTORAGE_H_WITH_DATA);
        } else {
            aitown_dstorage_h_mng_handle_decref (h_mng, handle, handle);
            handle = NULL;
            break;
        }

        break;
    }
    return handle;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */



