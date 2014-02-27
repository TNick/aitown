/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-lku.c
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

#include "aitown-dstorage-lku.h"
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

func_error_t aitown_dstorage_lku_init (
        aitown_dstorage_t * dstorage, aitown_dstorage_lku_t *lku,
        aitown_cfg_sect_t * sect_dstorage)
{

    func_error_t result = FUNC_OK;
    aitown_cfg_leaf_t * leaf_db;

    // clear
    memset (lku, 0, sizeof(aitown_dstorage_lku_t));

    for (;;) {

        // get the database name
        leaf_db = aitown_cfg_get_leaf (sect_dstorage, "id_database");
        if ((leaf_db == NULL) || (leaf_db->value == NULL)) {
            result = FUNC_GENERIC_ERROR;
            break;
        }

        // attempt to load this database
        result = aitown_db_open (&dstorage->db_mng, leaf_db->value, &lku->db);
        if (result != FUNC_OK)
            break;

        break;
    }

    return result;

}

void aitown_dstorage_lku_end (aitown_dstorage_lku_t *lku)
{

    // close the database
    if (lku->db != NULL) {
        aitown_db_close (&lku->db);
    }

    // clear
    memset (lku, 0, sizeof(aitown_dstorage_lku_t));

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
