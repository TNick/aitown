/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl_local_db.c
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

#include "dstorage_ctrl_local_db.h"

#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/aitown-dstorage.h>

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

static void ctrl_ending (dstorage_ctrl_t* ctrl)
{
    free (ctrl);
}

static void ctrl_read (dstorage_ctrl_param_t* req)
{

}

static void ctrl_write (dstorage_ctrl_param_t* req)
{

}

static dstorage_ctrl_t * creator(dstorage_t *dstorage, struct_ini_sect_t *settings)
{
    VAR_UNUSED (settings);
    dstorage_ctrl_local_db_t * ret = (dstorage_ctrl_local_db_t*)malloc(
                sizeof(dstorage_ctrl_local_db_t));
    dstorage_ctrl_init(&ret->header, dstorage);
    ret->header.ending = ctrl_ending;
    ret->header.read = ctrl_read;
    ret->header.write = ctrl_write;
    return (dstorage_ctrl_t *)ret;
}

void dstorage_ctrl_local_db_init ()
{
    dstorage_controllers_add_callback (creator, "localdb");
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


