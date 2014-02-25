/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg.h
  \date			September 2013
  \author		TNick

  This file defines aitown_cfg_t data structure that represents a visual
  sensor and related functions.



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_cfg_h_INCLUDE
#define AITOWN_cfg_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/aitown-cfg-config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitimage_t;
struct _aitown_core_t;

//! describes a cfg instance
///
typedef struct _aitown_cfg_t {


} aitown_cfg_t;

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


//! initialize a cfg sensor
///
/// @param cfg   address of the structure to initialise
///
AITOWN_EXPORT void
aitown_cfg_init (
        aitown_cfg_t *cfg);


//! terminate a cfg sensor
///
/// @param cfg   address of the structure to terminate
///
AITOWN_EXPORT void
aitown_cfg_end (
        aitown_cfg_t *cfg);


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
#endif /* AITOWN_cfg_h_INCLUDE */
