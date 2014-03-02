/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-data.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_data_h_INCLUDE
#define AITOWN_dstorage_data_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/pointer_aritmetic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a dstorage instance
///
typedef struct _aitown_dstorage_data_t {

    uint64_t    size; /**< the size of the buffer, excluding the header */

} aitown_dstorage_data_t;

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


//! initialize a data structure using specified size
///
/// @param data_ptr receives the pointer
/// @param size     the size of the buffer, excluding the header
///
func_error_t
aitown_dstorage_data_new (
        struct _aitown_dstorage_data_t ** data_ptr,
        unsigned size);


//! terminate a data structure
///
void
aitown_dstorage_data_free (
        struct _aitown_dstorage_data_t ** data);


//! the size of the buffer needed
///
static inline unsigned
aitown_dstorage_data_buffer_size (unsigned size) {
    return sizeof(aitown_dstorage_data_t) + size;
}


//! the size of the buffer needed
///
static inline void *
aitown_dstorage_data_buffer (
        struct _aitown_dstorage_data_t * data) {
    return (void*)(data+1);
}

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
#endif /* AITOWN_dstorage_data_h_INCLUDE */
