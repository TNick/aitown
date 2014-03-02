/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-controller.c
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
#include "aitown-dstorage-ctrl-mng.h"
#include "aitown-dstorage-data.h"

#include <aitown/aitown-cfg.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils.h>

#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>

#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define AITOWN_DSTORAGE_CONTROLLER_TERRIBLE 0

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


func_error_t aitown_dstorage_controller_init (
        aitown_db_mng_t *mng, aitown_dstorage_controller_t *ctrl,
        aitown_cfg_sect_t * cfg_sect, unsigned index)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // assume we're cleared by caller
        DBG_ASSERT (ctrl->performance == 0);
        DBG_ASSERT (ctrl->db == NULL);

        // get interesting leafs
        aitown_cfg_leaf_t * leaf_database =
            aitown_cfg_get_leaf (cfg_sect, "database");
        aitown_cfg_leaf_t * leaf_performance =
            aitown_cfg_get_leaf (cfg_sect, "performance");
        aitown_cfg_leaf_t * leaf_status =
            aitown_cfg_get_leaf (cfg_sect, "status");

        // make sure required leafs are in place
        if ((leaf_database == NULL) || (leaf_database->value == NULL) ||
            (leaf_performance == NULL) || (leaf_performance->value == NULL)) {

            err_message ("[dstorage/controllers/%d] section must contain 'database' and 'performance' keys", index);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // evaluate performance
        ctrl->performance = atoi (leaf_performance->value);

        // evaluate status
        ctrl->sts = AITOWN_DSTORAGE_CONTROLLER_STATUS_OK;
        if ((leaf_status != NULL) && (leaf_status->value != NULL)) {
            if (strcmp (leaf_status->value, "OK") == 0) {
                // ok
            } else if (strcmp (leaf_status->value, "TEMP_ERR") == 0) {
                ctrl->sts = AITOWN_DSTORAGE_CONTROLLER_STATUS_TMP_UNAVAIL;
                ctrl->performance = AITOWN_DSTORAGE_CONTROLLER_TERRIBLE;
            } else if (strcmp (leaf_status->value, "PERM_ERR") == 0) {
                ctrl->sts = AITOWN_DSTORAGE_CONTROLLER_STATUS_PERM_REMOVED;
                ctrl->performance = AITOWN_DSTORAGE_CONTROLLER_TERRIBLE;
            } else {
                err_message ("[dstorage/controllers/%d] section: "
                             "the status must be one of 'PERM_ERR', 'TEMP_ERR', 'OK'",
                             index);
                ret = FUNC_BAD_INPUT;
                break;
            }
        }

        // the databases are ready to go as aitown-db was already init
        if (ctrl->sts == AITOWN_DSTORAGE_CONTROLLER_STATUS_OK) {
            ret = aitown_db_open (mng, leaf_database->value, &ctrl->db);
            if (ret != FUNC_OK) {
                log_message ("[dstorage/controllers/%d] section: "
                             "database '%s' could not be loaded",
                             index, leaf_database->value);
                ctrl->sts = AITOWN_DSTORAGE_CONTROLLER_STATUS_TMP_UNAVAIL;
            }
        }

        break;
    }


    return ret;
}

void aitown_dstorage_controller_end (
        aitown_dstorage_controller_t *ctrl, aitown_cfg_sect_t * cfg_sect)
{
    DBG_ASSERT (ctrl != NULL);

    // close the database
    if (ctrl->db != NULL) {
        aitown_db_close (&ctrl->db);
    }

    if (cfg_sect != NULL) {

        // leafs
        // aitown_cfg_leaf_t * leaf_database =
        //    aitown_cfg_get_or_create_sect (cfg_sect, "database");
        aitown_cfg_leaf_t * leaf_performance =
            aitown_cfg_get_or_create_leaf (cfg_sect, "performance");
        aitown_cfg_leaf_t * leaf_status =
            aitown_cfg_get_or_create_leaf (cfg_sect, "status");

        // save performance
        char textbuf[64];
        sprintf (textbuf, "%u", ctrl->performance);
        aitown_cfg_set_leaf (leaf_performance, textbuf);

        // save status (don't change temporary error status)
        switch (ctrl->sts) {
        case AITOWN_DSTORAGE_CONTROLLER_STATUS_OK: {
            aitown_cfg_set_leaf (leaf_status, "OK");
            break; }
        case AITOWN_DSTORAGE_CONTROLLER_STATUS_PERM_REMOVED: {
            aitown_cfg_set_leaf (leaf_status, "PERM_ERR");
            break; }
        default: break;
        }

    }
}


func_error_t
aitown_dstorage_controller_get (
        aitown_dstorage_handle_t *handle)
{
    func_error_t ret = FUNC_OK;

    for (;;) {
        aitown_dstorage_handle_set_status (handle, DSTORAGE_H_WAITING_DATA);

        // read data
        size_t data_sz;
        aitown_db_read_t param;
        param.db = handle->ctrl->db;
        param.key = &handle->id;
        param.key_sz = sizeof(aitown_dstorage_id_t);
        param.val = (const void **)&handle->data;
        param.val_sz = &data_sz;
        param.kb = NULL;
        param.user = NULL;
        ret = aitown_db_read_ex (&param);
        if (ret != FUNC_OK) {
            aitown_dstorage_handle_set_status (handle, DSTORAGE_H_TEMP_FAILURE);
            return ret;
        }

        // set statuses
        aitown_dstorage_handle_set_clean (handle);
        aitown_dstorage_handle_set_status (handle, DSTORAGE_H_WITH_DATA);

        break;
    }

    return ret;
}

func_error_t aitown_dstorage_controller_set (
        aitown_dstorage_handle_t *handle)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // write data
        aitown_db_write_t param;
        param.db = handle->ctrl->db;
        param.key = &handle->id;
        param.key_sz = sizeof(aitown_dstorage_id_t);
        param.val = handle->data;
        param.val_sz = aitown_dstorage_data_buffer_size (handle->data->size);
        param.kb = NULL;
        param.user = NULL;
        ret = aitown_db_write_ex (&param);
        if (ret != FUNC_OK) {
            return ret;
        }

        // now clean
        aitown_dstorage_handle_set_clean (handle);

        break;
    }

    return ret;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
