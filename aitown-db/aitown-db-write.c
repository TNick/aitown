/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-write.c
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

#include "aitown-db.h"
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-write.h>
#include <aitown/aitown-db-driver.h>
#include <aitown/dbg_assert.h>

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

func_error_t aitown_db_write (
        aitown_db_t *db, const char * key, const char * value)
{
    aitown_db_write_t param;
    param.db = db;
    param.key = key;
    param.key_sz = strlen (key);
    param.val = value;
    param.val_sz = (value == NULL ? 0 : strlen (value));
    param.kb = NULL;
    param.user = NULL;
    return aitown_db_write_ex (&param);
}

func_error_t aitown_db_write_v (
        aitown_db_t *db, const void * key, size_t key_sz,
        const void * value, size_t value_len)
{
    aitown_db_write_t param;
    param.db = db;
    param.key = key;
    param.key_sz = key_sz;
    param.val = value;
    param.val_sz = value_len;
    param.kb = NULL;
    param.user = NULL;

    return aitown_db_write_ex (&param);
}

func_error_t aitown_db_write_ex ( aitown_db_write_t *params)
{
    DBG_ASSERT (params != NULL);

    if ((params->db == NULL) ||
        (params->key == NULL) ||
        (params->key_sz == 0)) {
        return FUNC_BAD_INPUT;
    }

    DBG_ASSERT (params->db->driver != NULL);
    DBG_ASSERT (params->db->driver->write != NULL);

    return params->db->driver->write (params);
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
