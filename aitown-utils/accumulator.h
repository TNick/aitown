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
/// instead of using pointers, one should use offsets with this structure
typedef struct _accumulator_t {
	void *		data;
	size_t		allocated;
	size_t		used;
	size_t		step;
} accumulator_t;

#ifndef OFFSET_TYPE_DEFINED
typedef size_t offset_t;
#define OFFSET_TYPE_DEFINED 1
#endif

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
AITOWN_EXPORT void *
accumulator_alloc (accumulator_t* accum, size_t sz );

//! add a string in the accumulator
/// if the size is 0 the lenght is computed
AITOWN_EXPORT char *
accumulator_add_string (accumulator_t* accum, const char * src, size_t sz );


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
