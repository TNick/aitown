/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dejavu.c
  \date			March 2014
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

#include "aitown-dejavu.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>
#include <aitown/aitown-core.h>

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

func_error_t aitown_dejavu_init (
        aitown_dejavu_t *dejavu, aitown_core_t * core,
        aitown_cfg_sect_t * cfg_sect)
{

    func_error_t ret = FUNC_OK;
    int cfg_err = 1;

    aitown_cfg_leaf_t * leaf_ar_cols;
    aitown_cfg_leaf_t * leaf_ar_rows;
    aitown_cfg_leaf_t * leaf_input_cols;
    aitown_cfg_leaf_t * leaf_input_rows;
    aitown_cfg_sect_t * sect_dejavu;

    unsigned input_cols;
    unsigned input_rows;
    unsigned ar_cols;
    unsigned ar_rows;

    for (;;) {

        // initialise
        memset (dejavu, 0, sizeof(aitown_dejavu_t));

        for (;;) {
            sect_dejavu =
                    aitown_cfg_get_sect (cfg_sect, "dejavu");
            if (sect_dejavu == NULL) break;
            leaf_input_cols =
                    aitown_cfg_get_leaf (sect_dejavu, "input_cols");
            if ( (leaf_input_cols == NULL) || (leaf_input_cols->value == NULL) )break;
            leaf_input_rows =
                    aitown_cfg_get_leaf (sect_dejavu, "input_rows");
            if ( (leaf_input_rows == NULL) || (leaf_input_rows->value == NULL) )break;
            leaf_ar_cols =
                    aitown_cfg_get_leaf (sect_dejavu, "ar_cols");
            if ( (leaf_ar_cols == NULL) || (leaf_ar_cols->value == NULL) )break;
            leaf_ar_rows =
                    aitown_cfg_get_leaf (sect_dejavu, "ar_rows");
            if ( (leaf_ar_rows == NULL) || (leaf_ar_rows->value == NULL) )break;

            cfg_err = 0;
            break;
        }
        if (cfg_err) {
            err_message ("[dejavu] section with input_cols, input_rows, ar_cols, ar_rows required");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // parameters from config file
        input_cols = (unsigned)atoi (leaf_input_cols->value);
        input_rows = (unsigned)atoi (leaf_input_rows->value);
        ar_cols = (unsigned)atoi (leaf_ar_cols->value);
        ar_rows = (unsigned)atoi (leaf_ar_rows->value);

        // initialize the sub-structures
        ret = aitown_dejavu_change_init (
                    &dejavu->chg,
                    sect_dejavu,
                    input_cols, input_rows);
        if (ret != FUNC_OK) break;
        aitown_dejavu_ar_init (
                    &dejavu->attrect,
                    &core->dstore.db_mng,
                    sect_dejavu,
                    input_cols, input_rows,
                    ar_cols, ar_rows);
        if (ret != FUNC_OK) break;

        dejavu->core = core;

        break;
    }

    return ret;
}

func_error_t aitown_dejavu_finit (
        aitown_dejavu_t * dejavu, aitown_core_t * core, const char * file)
{
    func_error_t ret;

    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, file);
    if (ret != FUNC_OK) {
        return ret;
    }

    // load manager from the root section
    ret = aitown_dejavu_init (dejavu, core, &cfg->root);

    // release our reference (works both if previous call succeded or not)
    aitown_cfg_decref (cfg, cfg);

    core->manage_cfg = 1;

    return ret;
}


void aitown_dejavu_end (aitown_dejavu_t *dejavu)
{

    // terminate the sub-structures
    aitown_dejavu_ar_end (&dejavu->attrect);
    aitown_dejavu_change_end (&dejavu->chg);

    // clear all, including substructures
    memset (dejavu, 0, sizeof(aitown_dejavu_t));
}


void aitown_dejavu_feed (aitown_dejavu_t *dejavu, const aitimage_t * image)
{
    DBG_ASSERT (dejavu != NULL);
    DBG_ASSERT (image != NULL);

    // check dimensions
    if ((aitown_dejavu_image_cols(&dejavu->attrect) != image->width) ||
        (aitown_dejavu_image_rows(&dejavu->attrect) != image->height)) {

        // inform sub-structures
        aitown_dejavu_change_reinit (&dejavu->chg, image->width, image->height);
        aitown_dejavu_change_reinit (&dejavu->chg, image->width, image->height);

    }

    // detect changes:
    aitown_dejavu_change_detect (&dejavu->chg, image);
    
    // interpret the content of the AR
    aitown_dejavu_ar_process (dejavu, image);

}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


