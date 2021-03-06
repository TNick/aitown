/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dejavu.h
  \date			March 2014
  \author		TNick

  This file defines aitown_dejavu_t data structure that represents a visual
  sensor and related functions.



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dejavu_h_INCLUDE
#define AITOWN_dejavu_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/aitown-dejavu-config.h>
#include <aitown/dejavu-change.h>
#include <aitown/dejavu-ar.h>

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
struct _aitown_cfg_sect_t;

//! describes a dejavu instance
///
typedef struct _aitown_dejavu_t {
    aitown_dejavu_ar_t      attrect;    /**< attention rectangle \f$\mathcal{AR}\f$ handler */
    aitown_dejavu_change_t  chg;        /**< structure in charge of tracking changes */
    struct _aitown_core_t * core;       /**< associated core */
} aitown_dejavu_t;

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




//! initialize sensor with settings from a section
AITOWN_EXPORT func_error_t
aitown_dejavu_init (
        struct _aitown_dejavu_t *dejavu,
        struct _aitown_core_t * core,
        struct _aitown_cfg_sect_t * cfg_sect);

//! initialize sensor with settings from a config file
///
AITOWN_EXPORT func_error_t
aitown_dejavu_finit (
        struct _aitown_dejavu_t * dejavu,
        struct _aitown_core_t * core,
        const char * file);


//! terminate a dejavu sensor
///
/// @param dejavu   address of the structure to terminate
///
AITOWN_EXPORT void
aitown_dejavu_end (
        aitown_dejavu_t *dejavu);


//! feed data to dejavu sensor
///
/// @param dejavu   address of the structure to terminate
/// @param image    input image; the ownership remains with the caller
///
AITOWN_EXPORT void
aitown_dejavu_feed (
        aitown_dejavu_t *dejavu,
        const struct _aitimage_t * image);


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
#endif /* AITOWN_dejavu_h_INCLUDE */
