/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-handle.c
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

#include "aitown-dstorage.h"
#include "aitown-dstorage-handle.h"
#include "aitown-dstorage-h-mng.h"

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


func_error_t aitown_dstorage_handle_init (
        aitown_dstorage_handle_t *handle, aitown_dstorage_id_t id)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // clear it
        memset (handle, 0, sizeof(aitown_dstorage_handle_t));
        // status is DSTORAGE_H_UNINITIALISED

        aitown_dstorage_handle_set_status (handle, DSTORAGE_H_NO_DATA);

        // set the ID
        handle->id = id;

        break;
    }

    return ret;
}

void aitown_dstorage_handle_end (
        aitown_dstorage_handle_t *handle)
{
    DBG_ASSERT (ctrl != NULL);

    // clear it
    memset (handle, 0, sizeof(aitown_dstorage_handle_t));
    // status is DSTORAGE_H_UNINITIALISED
}



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
