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
#ifndef AITOWN_server_globals_h_INCLUDE
#define AITOWN_server_globals_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! generic exit codes
typedef enum {
	AISERVER_OK = 0,
	AISERVER_GENERIC_ERROR,
	AISERVER_MEMORY_ERROR
} aiserver_error;

//! our assert
#ifdef AITOWN_AISERVER_DEBUG
#  define AISERVER_ASSERT(a) assert(a)
#else
#  define AISERVER_ASSERT(a)
#endif

#define AISERVER_UNUSED(expr) do { (void)(expr); } while (0)

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
#endif /* AITOWN_server_globals_h_INCLUDE */
