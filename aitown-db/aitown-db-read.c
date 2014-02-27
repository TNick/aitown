/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-read.c
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
#include <aitown/aitown-db-read.h>
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

const char * aitown_db_read (aitown_db_t *db, const char * key)
{
    const void * out;
    size_t out_sz;

    aitown_db_read_t param;
    param.db = db;
    param.key = key;
    param.key_sz = strlen (key);
    param.val = &out;
    param.val_sz = &out_sz;
    param.kb = NULL;
    param.user = NULL;

    func_error_t ret = aitown_db_read_ex (&param);
    if (ret == FUNC_OK) {
        return (const char*)out;
    } else {
        return NULL;
    }
}


const void * aitown_db_read_v (
        aitown_db_t *db, const void * key, size_t key_sz)
{
    const void * out;
    size_t out_sz;

    aitown_db_read_t param;
    param.db = db;
    param.key = key;
    param.key_sz = key_sz;
    param.val = &out;
    param.val_sz = &out_sz;
    param.kb = NULL;
    param.user = NULL;

    func_error_t ret = aitown_db_read_ex (&param);
    if (ret == FUNC_OK) {
        return out;
    } else {
        return NULL;
    }
}

func_error_t aitown_db_read_ex (aitown_db_read_t *params)
{

    DBG_ASSERT (params != NULL);

    if ((params->db == NULL) ||
        (params->key == NULL) ||
        (params->key_sz == 0) ||
        (params->val == NULL) ||
        (params->val_sz == NULL)) {
        return FUNC_BAD_INPUT;
    }

    DBG_ASSERT (params->db->driver != NULL);
    DBG_ASSERT (params->db->driver->read != NULL);

    return params->db->driver->read (params);
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
