/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-open.c
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

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-driver.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils.h>

#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define aitown_db_open_get_config(__name__) \
    aitown_cfg_leaf_t * cfgdb_ ## __name__ = \
            aitown_cfg_get_leaf( cfg_sect_ourdb, STRINGIFY(__name__)); \
    if ((cfgdb_ ## __name__ != NULL) && (cfgdb_ ## __name__->value != NULL)) { \
        params.__name__ = cfgdb_ ## __name__->value; \
    }
#define aitown_db_open_get_config_u(__name__) \
    aitown_cfg_leaf_t * cfgdb_ ## __name__ = \
            aitown_cfg_get_leaf( cfg_sect_ourdb, STRINGIFY(__name__)); \
    if ((cfgdb_ ## __name__ != NULL) && (cfgdb_ ## __name__->value != NULL)) { \
        params.__name__ = (unsigned)atoi (cfgdb_ ## __name__->value); \
    }

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


func_error_t aitown_db_open (
        aitown_db_mng_t * db_mng, const char * db_id, aitown_db_t ** out)
{
    DBG_ASSERT (db_mng != NULL);
    DBG_ASSERT (db_id != NULL);
    DBG_ASSERT (out != NULL);

    aitown_db_open_t params;
    aitown_db_preopen (&params, db_mng, db_id, out);

    for (;;) {

        // get root section
        aitown_cfg_sect_t * root_cfg_sect = db_mng->cfg_sect;
        if (root_cfg_sect == NULL)
            break;

        // get databases section
        aitown_cfg_sect_t * cfg_sect_databases =
                aitown_cfg_get_sect( root_cfg_sect, "databases");
        if (cfg_sect_databases == NULL)
            break;

        // get the section for our database
        aitown_cfg_sect_t * cfg_sect_ourdb =
                aitown_cfg_get_sect( cfg_sect_databases, db_id);
        if (cfg_sect_ourdb == NULL)
            break;

        // get the leafs that we find interesting
        aitown_db_open_get_config(driver);
        aitown_db_open_get_config(path_hint);
        aitown_db_open_get_config_u(key_len);
        aitown_db_open_get_config_u(value_len);

        break;
    }

    return aitown_db_open_ovr (&params);
}


func_error_t aitown_db_open_ovr (aitown_db_open_t * params)
{
    func_error_t ret = FUNC_OK;

    DBG_ASSERT (params != NULL);

    for (;;) {
        // check input
        if ((params->db_mng == NULL) ||
            (params->db_name == NULL) ||
            (params->driver == NULL) ||
            (params->out == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // the driver must be loaded
        aitown_db_driver_t * db_driver;
        if (aitown_db_driver_find (
                params->db_mng, params->driver, &db_driver)) {
            err_message ("No database driver named <%s> is loaded", params->driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        ret = db_driver->open (params);
        if (ret != FUNC_OK) {
            break;
        }

        // set manager
        aitown_db_t * db = *params->out;
        DBG_ASSERT (db != NULL);
        db->manager = params->db_mng;
        db->driver = db_driver;

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
