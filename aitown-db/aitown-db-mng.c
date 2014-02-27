/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-mng.c
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

#include "aitown-db-mng.h"

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-driver.h>
#include <aitown/dbg_assert.h>

#include <string.h>


#ifdef KYOTOCABINET_FOUND
#include "buildin/kyoto.h"
#endif // KYOTOCABINET_FOUND

#ifdef TOKYOCABINET_FOUND
#include "buildin/tokyo.h"
#endif // TOKYOCABINET_FOUND

#ifdef MYSQL_FOUND
#include "buildin/mysql-local.h"
#endif // MYSQL_FOUND

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

func_error_t aitown_db_mng_init (
        aitown_db_mng_t *db_mng, aitown_cfg_sect_t * cfg_sect)
{
    func_error_t ret = FUNC_OK;

    // clear the structure
    memset (db_mng, 0, sizeof(aitown_db_mng_t));

    // save settings section
    db_mng->cfg_sect = cfg_sect;
    if (cfg_sect != NULL) {
        if (cfg_sect->node.root == NULL) {
            ret = FUNC_BAD_INPUT;
        } else {
            // we will kep a reference as long as the manager is alive
            aitown_cfg_incref (cfg_sect->node.root, db_mng);
        }
    }

    for (;;) {

    // add build-in implementations
#   ifdef KYOTOCABINET_FOUND
    ret = aitown_db_kyoto_init (db_mng);
    if (ret != FUNC_OK) break;
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    ret = aitown_db_tokyo_init (db_mng);
    if (ret != FUNC_OK) break;
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    ret = aitown_db_mysql_init (db_mng);
    if (ret != FUNC_OK) break;
#   endif // MYSQL_FOUND

    break;
    }

    return ret;
}

func_error_t aitown_db_mng_finit ( aitown_db_mng_t *db_mng, const char * file)
{
    func_error_t ret;

    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, file);
    if (ret != FUNC_OK) {
        return ret;
    }

    // load manager from the rot section
    ret = aitown_db_mng_init (db_mng, &cfg->root);

    // release our reference (works both if previous call succeded or not)
    aitown_cfg_decref (cfg, cfg);

    db_mng->manage_cfg = 1;

    return ret;
}

void aitown_db_mng_end (aitown_db_mng_t *db_mng)
{
    DBG_ASSERT (db_mng != NULL);


    // remove build-in implementations
#   ifdef KYOTOCABINET_FOUND
    aitown_db_kyoto_init (db_mng);
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    aitown_db_tokyo_end (db_mng);
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    aitown_db_mysql_end (db_mng);
#   endif // MYSQL_FOUND

    // release our reference of the configuration file
    if (db_mng->cfg_sect != NULL) {
        if (db_mng->cfg_sect->node.root != NULL) {
            if (db_mng->manage_cfg != 0 ) {
                aitown_cfg_save (db_mng->cfg_sect->node.root, NULL);
            }
            aitown_cfg_decref (db_mng->cfg_sect->node.root, db_mng);
        }
    }

    // clear the structure
    memset (db_mng, 0, sizeof(aitown_db_mng_t));
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
