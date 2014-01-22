/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_chunk_mng.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_chunk_mng_h_INCLUDE
#define AITOWN_dstorage_chunk_mng_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/dstorage_func.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _dstorage_chunk_t;

//! manages a set of chunks, allocation of new ones
///
/// This structure keeps track of cummulated size of allocated chunks.
/// It also honors the limit imposed and will refuse to allocate
/// more memory if that limit is reached
typedef struct _dstorage_chunk_mng_t {
    size_t      allocated;
    size_t      limit;
} dstorage_chunk_mng_t;


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

//! initialize a manager structure;
DSTORAGE_FUNC void
dstorage_chunk_mng_init (dstorage_chunk_mng_t *mng);

//! terminate the manager;
DSTORAGE_FUNC void
dstorage_chunk_mng_end (dstorage_chunk_mng_t *mng);

//! set the new limit
static inline void
dstorage_chunk_mng_set_limit (dstorage_chunk_mng_t *mng, size_t limit)
{
    mng->limit = limit;
}


//! allocate a memory chunk
DSTORAGE_FUNC struct _dstorage_chunk_t*
dstorage_chunk_mng_alloc (dstorage_chunk_mng_t *mng, size_t sz);

//! free a memory chunk
DSTORAGE_FUNC void
dstorage_chunk_mng_free (
        dstorage_chunk_mng_t *mng,
        struct _dstorage_chunk_t** chunk);

//! check that a memory chunk belongs here (0 false, 1 true)
DSTORAGE_FUNC int
dstorage_chunk_mng_belongs (
        dstorage_chunk_mng_t *mng,
        struct _dstorage_chunk_t* chunk);


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
#endif // AITOWN_dstorage_chunk_mng_h_INCLUDE
