/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_chunk.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_chunk_h_INCLUDE
#define AITOWN_dstorage_chunk_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/utils.h>
#include <aitown/error_codes.h>
#include <aitown/pivotal_gmtime.h>
#include <aitown/pointer_aritmetic.h>
#include <aitown/dstorage_id.h>
#include <aitown/dstorage_func.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _dstorage_chunk_mng_t;


//! describes a chunk of memory
///
/// Each chunk has a header structure represented by this type.
/// The chunks are managed (allocated, freed) by a dstorage_chunk_mng_t
/// structure.
typedef struct _dstorage_chunk_t {
    size_t      user_sz;    /**< size of the user dedicated block following us */
} dstorage_chunk_t;

//! macro to compute the full size of a chunk of memory (userspace + header)
#define dstorage_chunk_size(usz) (usz + sizeof(dstorage_chunk_t))

//! macro to get the pointer to user area
#define dstorage_chunk_user(p_ck)  PTR_ADD(p_ck,sizeof(dstorage_chunk_t))

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


//! used by dstorage_chunk_mng_t to initialise this structure
void
dstorage_chunk_init (
        struct _dstorage_chunk_mng_t *mng,
        dstorage_chunk_t *ctrl,
        size_t user_sz);

//! used by dstorage_chunk_mng_t to terminate this structure
void
dstorage_chunk_end (struct _dstorage_chunk_mng_t *mng, dstorage_chunk_t *ctrl);

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
#endif // AITOWN_dstorage_chunk_h_INCLUDE
