/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			accumulator.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_accumulator_h_INCLUDE
#define AITOWN_accumulator_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <stddef.h>
#include "error_codes.h"
#include "utils_offset.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! header structure for an accumulator
/// instead of using pointers, one should use offsets with this structure;
/// each allocation is an individual allocation and gets a pointer aligned
/// location; for a string buffer that glues things right after
/// previous content use char_buff_t
typedef struct _accumulator_t {
	void *		data;
	size_t		allocated;
	size_t		used;
	size_t		step;
} accumulator_t;

//! the value of an invalid offset is 0 (first position is reserver)
#define ACCUMULATOR_BAD_OFFSET ((offset_t)0)

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

//! initialize the manager
AITOWN_EXPORT void
accumulator_init (accumulator_t* accum, size_t init_size);

//! terminate the manager
AITOWN_EXPORT void
accumulator_end (accumulator_t* accum);

//! allocate a chunk in the accumulator
AITOWN_EXPORT offset_t
accumulator_alloc (accumulator_t* accum, size_t sz );

//! add a string in the accumulator
/// if the size is 0 the lenght is computed
AITOWN_EXPORT offset_t
accumulator_add_string (accumulator_t* accum, const char * src, size_t sz );

//! free some bytes from the end; be careful with this
AITOWN_EXPORT func_error_t
accumulator_free (accumulator_t* accum, size_t sz);

//! convert an offset into a temporary pointer that is valid until next alloc
static inline void *
accumulator_to_ptr (accumulator_t* accum, offset_t off) {
	return (void*)PTR_ADD (accum->data, off);
}

//! convert an offset into a temporary pointer that is valid until next alloc
static inline char *
accumulator_to_char (accumulator_t* accum, offset_t off) {
	return (char*)PTR_ADD (accum->data, off);
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
#endif // AITOWN_accumulator_h_INCLUDE
