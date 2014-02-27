/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-ctrl.c
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

#include "aitown-dstorage-ctrl.h"
#include "aitown-dstorage.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

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

func_error_t aitown_dstorage_ctrl_init (
        aitown_dstorage_t * dstorage, const char * database,
        aitown_dstorage_ctrl_t **ctrl)
{
    func_error_t result = FUNC_OK;
    // allocate
    aitown_dstorage_ctrl_t * ret = (aitown_dstorage_ctrl_t *) malloc (
                sizeof (aitown_dstorage_ctrl_t));
    if (ret == NULL) {
        return FUNC_MEMORY_ERROR;
    }

    // clear
    memset (ret, 0, sizeof(aitown_dstorage_ctrl_t));

    // attempt to load this database
    result = aitown_db_open (&dstorage->db_mng, database, &ret->db);
    if ( result != FUNC_OK ) {
        free (ret);
        return result;
    }

    // insert it in chain as last member
    if (dstorage->first_ctrl == NULL) {
        DBG_ASSERT (dstorage->last_ctrl == NULL);
        dstorage->first_ctrl = ret;
    } else {
        DBG_ASSERT (dstorage->last_ctrl != NULL);
        dstorage->last_ctrl->next = ret;
    }
    dstorage->last_ctrl = ret;

    // we're good
    *ctrl = ret;
    return FUNC_OK;
}

void aitown_dstorage_ctrl_end (aitown_dstorage_ctrl_t **ctrl_ptr)
{
    DBG_ASSERT (ctrl_ptr);
    aitown_dstorage_ctrl_t *ctrl = *ctrl_ptr;
    if (ctrl == NULL) {
        return;
    }

    // close the database
    if (ctrl->db != NULL) {
        aitown_db_close (&ctrl->db);
    }

    // clear it
    memset (ctrl, 0, sizeof(aitown_dstorage_ctrl_t));
    *ctrl_ptr = NULL;
}

func_error_t aitown_dstorage_ctrl_load_list (
        aitown_dstorage_t * dstorage, aitown_cfg_sect_t * sect_dstorage)
{
    func_error_t ret = FUNC_OK;
    int i;
    char name_buff[64];
    aitown_cfg_sect_t * sect_ctrl;
    aitown_cfg_leaf_t * leaf_db;
    aitown_dstorage_ctrl_t * ctrl;


    for (i = 0; i < INT_MAX; i++) {

        // find the section for this id
        sprintf (name_buff, "%d", i);
        sect_ctrl = aitown_cfg_get_sect (sect_dstorage, name_buff);
        if (sect_ctrl == NULL)
            break;

        // get the database name
        leaf_db = aitown_cfg_get_leaf (sect_ctrl, "database");
        if ((leaf_db == NULL) || (leaf_db->value == NULL)) {
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // load this controller
        ret = aitown_dstorage_ctrl_init (
                    dstorage, leaf_db->value, &ctrl);
        if (ret != FUNC_OK) {
            break;
        }

    }

    // if we found no controller treat it as an error
    if ((ret == FUNC_OK) && (i == 0)) {
        ret = FUNC_GENERIC_ERROR;
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
