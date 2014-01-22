/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_handle.c
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

#include "dstorage_handle.h"
#include "dstorage_handle_mng.h"

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>
#include <aitown/char_buff.h>
#include <aitown/utils_unused.h>

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

dstorage_handle_t* dstorage_handle_init_empty (dstorage_handle_mng_t *mng)
{
    dstorage_handle_t *ret_p;
    VAR_UNUSED (mng); /* will be used for better memory management */
    ret_p = malloc (sizeof(dstorage_handle_t));
    if (ret_p != NULL)
    {
        memset (ret_p, 0, sizeof(dstorage_handle_t));
        dstorage_handle_mark_uninit (ret_p);
    }
    return ret_p;
}

dstorage_handle_t* dstorage_handle_init (dstorage_handle_mng_t *mng, dstorage_id_t id)
{
    dstorage_handle_t *ret_p;
    VAR_UNUSED (mng); /* will be used for better memory management */
    ret_p = malloc (sizeof(dstorage_handle_t));
    if (ret_p != NULL)
    {
        memset (ret_p, 0, sizeof(dstorage_handle_t));
        ret_p->id = id;
        dstorage_handle_mark_init (ret_p);
    }
    return ret_p;
}

void dstorage_handle_end (dstorage_handle_mng_t *mng, dstorage_handle_t** h)
{
    VAR_UNUSED (mng); /* will be used for better memory management */
    free (*h);
    *h = NULL;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


