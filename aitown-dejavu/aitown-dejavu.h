/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dejavu.h
  \date			September 2013
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

// allow both undefined and 0 to mean the same thing in terms of input size
#ifndef AITOWN_DEJAVU_FIX_WIDTH
#define AITOWN_DEJAVU_FIX_WIDTH 0
#endif
#ifndef AITOWN_DEJAVU_FIX_HEIGHT
#define AITOWN_DEJAVU_FIX_HEIGHT 0
#endif

// if input is variable we can't have fixed sizes
#ifdef AITOWN_DEJAVU_INPUT_IS_VARIABLE
#undef AITOWN_DEJAVU_FIX_WIDTH
#undef AITOWN_DEJAVU_FIX_HEIGHT
#define AITOWN_DEJAVU_FIX_WIDTH 0
#define AITOWN_DEJAVU_FIX_HEIGHT 0
#endif

//! a square area inside an image; attention rectangle is one such area
typedef struct _aitown_dejavu_area_t {
    unsigned                x;          /**< horizontal position */
    unsigned                y;          /**< vertical position */

#ifdef AITOWN_DEJAVU_AR_SQUARE
    unsigned                size ;      /**< horizontal and vertical size (columns and rows) */
    unsigned                pix;        /**< number of pixels in each cell */
#else
    unsigned                width;      /**< number of columns */
    unsigned                height;     /**< number of rows */
    unsigned                pix_w;      /**< number of pixels in each cell */
    unsigned                pix_h;      /**< number of pixels in each cell */
#endif

} aitown_dejavu_area_t;

//! macro that gets the width of the attention rectangle
#ifdef AITOWN_DEJAVU_AR_SQUARE
#define aitown_dejavu_area_get_width(p)  ((p)->width)
#else
#define aitown_dejavu_area_get_width(p)  ((p)->size)
#endif

//! macro that gets the height of the attention rectangle
#ifdef AITOWN_DEJAVU_AR_SQUARE
#define aitown_dejavu_area_get_height(p)  ((p)->height)
#else
#define aitown_dejavu_area_get_height(p)  ((p)->size)
#endif

//! macro that gets the number of pixels in a cell's row
#ifdef AITOWN_DEJAVU_AR_SQUARE
#define aitown_dejavu_area_get_pixel_width(p)  ((p)->pix_w)
#else
#define aitown_dejavu_area_get_pixel_width(p)  ((p)->pix)
#endif

//! macro that gets the number of pixels in a cell's column
#ifdef AITOWN_DEJAVU_AR_SQUARE
#define aitown_dejavu_area_get_pixel_height(p)  ((p)->pix_h)
#else
#define aitown_dejavu_area_get_pixel_height(p)  ((p)->pix)
#endif



//! describes a dejavu instance
///
typedef struct _aitown_dejavu_t {
    unsigned                width;      /**< width of the input image in pixels */
    unsigned                height;     /**< height of the input image in pixels */

    aitown_dejavu_area_t    ar;         /**< attention rectangle \f$\mathcal{AR}\f$ */
    aitown_dejavu_change_t  chg;        /**< structure in charge of tracking changes */
} aitown_dejavu_t;

//! macro that gets the width of the expected input in aitown_dejavu_t
#if AITOWN_DEJAVU_FIX_WIDTH == 0
#define aitown_dejavu_get_width(p)  ((p)->width)
#else
#define aitown_dejavu_get_width(p)  AITOWN_DEJAVU_FIX_WIDTH
#endif

//! macro that gets the height of the expected input in aitown_dejavu_t
#if AITOWN_DEJAVU_FIX_HEIGHT == 0
#define aitown_dejavu_get_height(p)  ((p)->width)
#else
#define aitown_dejavu_get_height(p)  AITOWN_DEJAVU_FIX_HEIGHT
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


//! initialize a dejavu sensor
///
/// @param dejavu   address of the structure to initialise
/// @param width    width of the input
/// @param height   height of the input
///
AITOWN_EXPORT void
aitown_dejavu_init (
        aitown_dejavu_t *dejavu,
        unsigned width,
        unsigned height);



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
