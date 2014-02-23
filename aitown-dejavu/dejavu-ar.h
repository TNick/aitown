/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-ar.h
  \date			February 2014
  \author		TNick

  This file defines aitown_dejavu_ar_t data structure that groups
  information related to processing an area of interest in an image.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dejavu_ar_h_INCLUDE
#define AITOWN_dejavu_ar_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/aitown-image.h>

#include <inttypes.h>

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
typedef struct _aitown_dejavu_ar_t {
    unsigned                x;          /**< horizontal position (offset in real pixels) */
    unsigned                y;          /**< vertical position (offset in real pixels) */
    unsigned                img_cols;   /**< width of the input image in real pixels */
    unsigned                img_rows;   /**< height of the input image in real pixels */

    unsigned                grid_cols;  /**< number of columns in grid */
    unsigned                grid_rows;  /**< number of rows in grid */
    unsigned                pix_horiz;  /**< number of pixels in each cell in horizontal direction (width) */
    unsigned                pix_vert;   /**< number of pixels in each cell in vertical direction (height) */

} aitown_dejavu_ar_t;


//! macro that gets the number of columns the grid of the attention rectangle
#ifdef AITOWN_DEJAVU_GRID_IS_SQUARE
#define aitown_dejavu_grid_cols(p)  ((p)->grid_cols)
#else
#define aitown_dejavu_grid_cols(p)  ((p)->grid_cols)
#endif

//! macro that gets the number of rows in the grid of attention rectangle
#ifdef AITOWN_DEJAVU_GRID_IS_SQUARE
#define aitown_dejavu_grid_rows(p)  ((p)->grid_rows)
#else
#define aitown_dejavu_grid_rows(p)  ((p)->grid_cols)
#endif

//! macro that gets the number of pixels in a cell's row
#ifdef AITOWN_DEJAVU_GRID_IS_SQUARE
#define aitown_dejavu_area_get_pixel_width(p)  ((p)->pix_horiz)
#else
#define aitown_dejavu_area_get_pixel_width(p)  ((p)->pix_horiz)
#endif

//! macro that gets the number of pixels in a cell's column
#ifdef AITOWN_DEJAVU_GRID_IS_SQUARE
#define aitown_dejavu_area_get_pixel_height(p)  ((p)->pix_vert)
#else
#define aitown_dejavu_area_get_pixel_height(p)  ((p)->pix_vert)
#endif

//! macro that gets the width of the expected input in aitown_dejavu_ar_t
#if AITOWN_DEJAVU_FIX_WIDTH == 0
#define aitown_dejavu_image_cols(p)  ((p)->img_cols)
#else
#define aitown_dejavu_image_cols(p)  AITOWN_DEJAVU_FIX_WIDTH
#endif

//! macro that gets the height of the expected input in aitown_dejavu_ar_t
#if AITOWN_DEJAVU_FIX_HEIGHT == 0
#define aitown_dejavu_image_rows(p)  ((p)->img_rows)
#else
#define aitown_dejavu_image_rows(p)  AITOWN_DEJAVU_FIX_HEIGHT
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
/// @param ar      address of the structure to initialise
/// @param width    width of the input image
/// @param height   height of the input image
///
AITOWN_EXPORT void
aitown_dejavu_ar_init (
        aitown_dejavu_ar_t *ar,
        unsigned input_cols,
        unsigned input_rows,
        unsigned ar_cols,
        unsigned ar_rows);


//! terminate
///
/// @param chg   address of the structure to terminate
///
AITOWN_EXPORT void
aitown_dejavu_ar_end (
        aitown_dejavu_ar_t *ar);


//! informed that the geometry of the image has changed
///
/// @param ar       address of the structure
/// @param width    new width of the input image
/// @param height   new height of the input image
///
AITOWN_EXPORT void
aitown_dejavu_ar_reinit (
        aitown_dejavu_ar_t *chg,
        unsigned width,
        unsigned height);

//! called to process the data
///
/// @warning the data inside image is never checked and assumed
/// to be of correct type and size.
/// 
/// @param ar       address of the structure
/// @param image    input data
///
AITOWN_EXPORT void
aitown_dejavu_ar_process (
        aitown_dejavu_ar_t *ar,
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
#endif /* AITOWN_dejavu_ar_h_INCLUDE */
