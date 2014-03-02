/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.c
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
#include "aitown-dstorage-handle.h"
#include "aitown-dstorage-controller.h"

#include <aitown/aitown-cfg.h>
#include <aitown/dbg_assert.h>

#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>
#include <aitown/aitown-db-free.h>

#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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


func_error_t aitown_dstorage_init (
        aitown_dstorage_t *dstorage, aitown_cfg_sect_t * cfg_sect)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // clear the structure
        memset (dstorage, 0, sizeof(aitown_dstorage_t));

        // save settings section
        dstorage->cfg_sect = cfg_sect;
        if (cfg_sect != NULL) {
            if (cfg_sect->node.root == NULL) {
                ret = FUNC_BAD_INPUT;
                break;
            } else {
                // we will kep a reference as long as the manager is alive
                aitown_cfg_incref (cfg_sect->node.root, dstorage);
            }
        }

        // start-up the database
        ret = aitown_db_mng_init (&dstorage->db_mng, cfg_sect);
        if (ret != FUNC_OK) break;

        // controllers
        ret = aitown_dstorage_ctrl_mng_init (
                    &dstorage->db_mng, &dstorage->ctrl_mng, cfg_sect);
        if (ret != FUNC_OK) break;

        // handlers
        ret = aitown_dstorage_h_mng_init (
                    &dstorage->db_mng, &dstorage->h_mng, cfg_sect);
        if (ret != FUNC_OK) break;

        // handles
        ret = aitown_dstorage_h_mng_init (
                    &dstorage->db_mng, &dstorage->h_mng, cfg_sect);
        if (ret != FUNC_OK) break;



        break;
    }


    return ret;
}

func_error_t aitown_dstorage_finit ( aitown_dstorage_t *dstorage, const char * file)
{
    func_error_t ret;

    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, file);
    if (ret != FUNC_OK) {
        return ret;
    }

    // load manager from the root section
    ret = aitown_dstorage_init (dstorage, &cfg->root);

    // release our reference (works both if previous call succeded or not)
    aitown_cfg_decref (cfg, cfg);

    dstorage->manage_cfg = 1;

    return ret;
}

void aitown_dstorage_end (aitown_dstorage_t *dstorage)
{
    DBG_ASSERT (dstorage != NULL);

    // handlers
    aitown_dstorage_h_mng_end (&dstorage->h_mng, dstorage->cfg_sect);

    // controllers
    aitown_dstorage_ctrl_mng_end (&dstorage->ctrl_mng, dstorage->cfg_sect);

    // handles
    aitown_dstorage_h_mng_end (&dstorage->h_mng, dstorage->cfg_sect);

    // stop database
    aitown_db_mng_end (&dstorage->db_mng);

    // release our reference of the configuration file
    if (dstorage->cfg_sect != NULL) {
        if (dstorage->cfg_sect->node.root != NULL) {
            if (dstorage->manage_cfg != 0 ) {
                aitown_cfg_save (dstorage->cfg_sect->node.root, NULL);
            }
            aitown_cfg_decref (dstorage->cfg_sect->node.root, dstorage);
        }
    }

    // clear the structure
    memset (dstorage, 0, sizeof(aitown_dstorage_t));
}

func_error_t aitown_dstorage_resolve_handle (
        aitown_dstorage_t * mng, aitown_dstorage_handle_t * handle)
{
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (handle != NULL);
    DBG_ASSERT (handle->ctrl != NULL);
    DBG_ASSERT (handle->ctrl->db != NULL);

    func_error_t ret = FUNC_OK;

    // do we need to do this?
    if (handle->data != NULL) {
        return FUNC_OK;
    }

    // retreive it
    ret = aitown_dstorage_controller_get (handle);

    return ret;
}

func_error_t aitown_dstorage_set_raw (
        aitown_dstorage_handle_t * handle,
        const void * data, unsigned data_size)
{
    func_error_t ret = FUNC_OK;

    // allocate a new chunk
    aitown_dstorage_data_t * data_ptr;
    ret = aitown_dstorage_data_new (&data_ptr, data_size);
    if (ret != FUNC_OK) {
        return ret;
    }

    // copy data inside
    memcpy (aitown_dstorage_data_buffer (data_ptr), data, data_size);

    if (handle->data != NULL) {
        DBG_ASSERT (aitown_dstorage_handle_status(handle) == DSTORAGE_H_WITH_DATA);

        // either use the database or our own allocator
        if (aitown_dstorage_handle_local_mem (handle)) {
            aitown_dstorage_data_free (&handle->data);
        } else {
            DBG_ASSERT (handle->ctrl != NULL);
            DBG_ASSERT (handle->ctrl->db != NULL);
            aitown_db_free (handle->ctrl->db, (const void**)&handle->data);
        }
    }

    // set new data
    handle->data = data_ptr;
    aitown_dstorage_handle_set_dirty (handle);
    aitown_dstorage_handle_set_mem_local (handle);
    aitown_dstorage_handle_set_status (handle, DSTORAGE_H_WITH_DATA);

    return ret;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
