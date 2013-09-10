/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown_global.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_aitown_global_h_INCLUDE
#define AITOWN_aitown_global_h_INCLUDE
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


/// borrowed from zmq
#if defined AITOWN_WIN32
#   if defined AITOWN_STATIC
#       define AITOWN_EXPORT
#   elif defined AITOWN_SHARED
#       define AITOWN_EXPORT __declspec(dllexport)
#   else
#       define AITOWN_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define AITOWN_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define AITOWN_EXPORT __attribute__ ((visibility("default")))
#   else
#       define AITOWN_EXPORT
#   endif
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
#endif // AITOWN_aitown_global_h_INCLUDE
