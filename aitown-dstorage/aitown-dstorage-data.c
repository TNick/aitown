/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-data.c
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
#include "aitown-dstorage-data.h"
#include "aitown-dstorage-h-mng.h"

#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>

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


func_error_t aitown_dstorage_data_new (
        aitown_dstorage_data_t **data_ptr, unsigned size)
{
    func_error_t ret = FUNC_OK;
    aitown_dstorage_data_t *data = NULL;

    for (;;) {

        // allocate
        unsigned sz = aitown_dstorage_data_buffer_size (size);
        data = (aitown_dstorage_data_t *) malloc(sz+1);
        if (data == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // support null-terminated strings
        char * p_end = PTR_ADD(data, sz);
        *p_end = 0;

        // set size
        data->size = size;

        break;
    }

    *data_ptr = data;
    return ret;
}

void aitown_dstorage_data_free (
        aitown_dstorage_data_t **data_ptr)
{
    DBG_ASSERT (data_ptr != NULL);

    aitown_dstorage_data_t * data = *data_ptr;
    if (data == NULL) return;

    // and we're done
    free (data);
    *data_ptr = NULL;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
