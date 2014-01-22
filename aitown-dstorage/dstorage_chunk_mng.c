/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_chunk_mng.c
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

#include "dstorage_chunk.h"
#include "dstorage_chunk_mng.h"

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
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

void dstorage_chunk_mng_init (dstorage_chunk_mng_t *mng)
{
    DBG_ASSERT (mng != NULL);
    memset (mng, 0, sizeof(dstorage_chunk_mng_t));
}

void dstorage_chunk_mng_end (dstorage_chunk_mng_t *mng)
{
    DBG_ASSERT (mng != NULL);

    if (mng->allocated > 0) {
        err_message ("dstorage chunk manager terminated while %u bytes still allocated", (unsigned)mng->allocated);
    }

    memset (mng, 0, sizeof(dstorage_chunk_mng_t));
}

dstorage_chunk_t* dstorage_chunk_mng_alloc (
        dstorage_chunk_mng_t*mng, size_t sz)
{
    DBG_ASSERT (mng != NULL);

    // check the limits
    if (sz + mng->allocated > mng->limit) {
        dbg_message ("dstorage chunk limit reached (%u)", (unsigned)mng->limit);
        return NULL;
    }

    // allocate from system
    dstorage_chunk_t* ret = (dstorage_chunk_t*)malloc(dstorage_chunk_size(sz));
    if ( ret == NULL) {
        dbg_message ("dstorage chunk failed because of system lacking memory (%u requested)", (unsigned)sz);
        return NULL;
    }

    // update allocated size
    mng->allocated += sz;

    // initialise the header
    dstorage_chunk_init (mng, ret, sz);

    return ret;
}

void dstorage_chunk_mng_free (
        dstorage_chunk_mng_t *mng, dstorage_chunk_t** chunk)
{
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (chunk != NULL);
    dstorage_chunk_t * actual_ck = *chunk;

    // attempt to free NULL pointer
    if (actual_ck == NULL) {
        DBG_ASSERT (0);
        return;
    }
    size_t usz = actual_ck->user_sz;

    DBG_ASSERT (dstorage_chunk_mng_belongs (mng, actual_ck));

    // inform header that is being retired and free memory
    dstorage_chunk_end (mng, actual_ck);
    free (actual_ck);
    mng->allocated -= usz;

    // mark user variable free
    *chunk = NULL;
}

int dstorage_chunk_mng_belongs (
        dstorage_chunk_mng_t *mng, dstorage_chunk_t* chunk)
{
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (chunk != NULL);

    VAR_UNUSED (mng);
    VAR_UNUSED (chunk);

    /** @todo implement */

    return 1;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


