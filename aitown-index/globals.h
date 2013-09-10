/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file            globals.h
  \date            September 2013
  \author        TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_globals_h_INCLUDE
#define AITOWN_globals_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif 

//! generic exit codes
typedef enum {
	INDEX_OK = 0,
	INDEX_GENERIC_ERROR,
	INDEX_MEMORY_ERROR
} index_error;

//! our assert
#ifdef AITOWN_INDEX_DEBUG
#  define INDEX_ASSERT(a) assert(a)
#else
#  define INDEX_ASSERT(a)
#endif

#define INDEX_UNUSED(expr) do { (void)(expr); } while (0)

#ifndef NULL
#define NULL ((void*)0)
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
#endif /* AITOWN_globals_h_INCLUDE */
