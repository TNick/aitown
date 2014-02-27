/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.c
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
#include "aitown-dstorage-ctrl.h"
#include <aitown/aitown-cfg.h>

#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/pivotal_gmtime.h>
#include <aitown/utils.h>

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
        aitown_dstorage_t * dstorage, aitown_cfg_sect_t *cfg_sect)
{
    func_error_t ret = FUNC_OK;
    aitown_cfg_sect_t * sect_dstorage;
    aitown_cfg_sect_t * sect_ctrl_list;

    // clear the structure
    memset (dstorage, 0, sizeof(aitown_dstorage_t));

    for (;;) {
        // cache settings section
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

        // get relevant sections from config
        int ctrl_ok = 0;
        for (;;) {

            // find the section regarding us
            sect_dstorage = aitown_cfg_get_sect (cfg_sect, "dstorage");
            if (sect_dstorage == NULL) break;

            // find the section with list of controllers
            sect_ctrl_list = aitown_cfg_get_sect (sect_dstorage, "controllers");
            if (sect_ctrl_list == NULL) break;

            // at least one leaf
            if (sect_ctrl_list->first_sect == NULL) break;

            ctrl_ok = 1;
            break;
        }
        if (ctrl_ok == 0) {
            err_message ("dstorage module needs at least one controller in config file");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // start up database module
        ret = aitown_db_mng_init (&dstorage->db_mng, cfg_sect);
        if (ret != FUNC_OK) {
            break;
        }

        // start up ID lookup
        ret = aitown_dstorage_lku_init (dstorage, &dstorage->lku, sect_dstorage);
        if (ret != FUNC_OK) {
            break;
        }

        // load controllers
        ret = aitown_dstorage_ctrl_load_list (dstorage, sect_ctrl_list);
        if (ret != FUNC_OK)
            break;


        break;
    }

    return ret;
}

func_error_t aitown_dstorage_finit (aitown_dstorage_t * dstorage, const char * file)
{
    func_error_t ret;

    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, file);
    if (ret != FUNC_OK) {
        return ret;
    }

    // load manager from the rot section
    ret = aitown_dstorage_init (dstorage, &cfg->root);

    // release our reference (works both if previous call succeded or not)
    aitown_cfg_decref (cfg, cfg);

    dstorage->manage_cfg = 1;

    return ret;
}

void aitown_dstorage_end (aitown_dstorage_t *dstorage)
{

    // terminate database manager
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


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


