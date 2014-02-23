/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			image_greyscale.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_image_greyscale_h_INCLUDE
#define AITOWN_image_greyscale_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>

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

//! convert a color image to its greyscale representation
///
/// Source image is assumed to be in Red Green Blue Alpha representation
/// (AITIMAGE_RGBA8) with each component represented as
/// a byte (8-bits) unsigned integer.
///
/// The output is stored as an array of 16-bits (2 byte) unsigned integers
/// (AITIMAGE_GREY16).
/// The grey value is computed as R*G*B/256 (not proper for display)
AITOWN_EXPORT func_error_t
aitimage_grey16_from_rgba8_ptr (
        const struct _aitimage_t *source,
        struct _aitimage_t *destination,
        const uint32_t *p_src,
        uint16_t *p_dst);

//! convert a color image to its greyscale representation
///
/// Source image is assumed to be in Red Green Blue Alpha representation
/// (AITIMAGE_RGBA8) with each component represented as
/// a byte (8-bits) unsigned integer.
///
/// The output is stored as an array of 16-bits (2 byte) unsigned integers
/// (AITIMAGE_GREY16).
/// The grey value is computed as R*G*B/256 (not proper for display)
AITOWN_EXPORT func_error_t
aitimage_grey16_from_rgba8 (
        const struct _aitimage_t *source,
        struct _aitimage_t *destination);


//! convert a color image to its greyscale integral representation
///
/// Source image is assumed to be in Red Green Blue Alpha representation
/// (AITIMAGE_RGBA8) with each component represented as
/// a byte (8-bits) unsigned integer.
///
/// The output is stored as an array of 16-bits (2 byte) unsigned integers
/// (AITIMAGE_GREYI).
///
/// See <a href="http://opensurf1.googlecode.com/files/opensurf.pdf">OpenSURF</a>
/// for details on integral images.
AITOWN_EXPORT func_error_t
aitimage_greyi_from_rgba8_ptr (
        const struct _aitimage_t *source,
        struct _aitimage_t *destination,
        const uint32_t *p_src,
        double *p_dst);

//! convert a color image to its greyscale integral representation
///
/// Source image is assumed to be in Red Green Blue Alpha representation
/// (AITIMAGE_RGBA8) with each component represented as
/// a byte (8-bits) unsigned integer.
///
/// The output is stored as an array of 16-bits (2 byte) unsigned integers
/// (AITIMAGE_GREYI).
///
/// See <a href="http://opensurf1.googlecode.com/files/opensurf.pdf">OpenSURF</a>
/// for details on integral images.
AITOWN_EXPORT func_error_t
aitimage_greyi_from_rgba8 (
        const struct _aitimage_t *source,
        struct _aitimage_t *destination);




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
#endif /* AITOWN_image_greyscale_h_INCLUDE */
