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

/* DEPRECATED */
void dstorage_handle_mng_init (dstorage_handle_mng_t *mng)
{
    memset (mng, 0, sizeof(dstorage_handle_mng_t));
}

void dstorage_handle_mng_end (dstorage_handle_mng_t *mng)
{
    memset (mng, 0, sizeof(dstorage_handle_mng_t));
}

dstorage_handle_t* dstorage_handle_mng_new (
        dstorage_handle_mng_t *mng, dstorage_id_t id)
{
    return dstorage_handle_init (mng, id);
}

void dstorage_handle_mng_resolve (
        dstorage_handle_mng_t *mng, dstorage_handle_t id)
{
    
    
}
/* DEPRECATED */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */




