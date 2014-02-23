/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-image.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_image_h_INCLUDE
#define AITOWN_image_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/pointer_aritmetic.h>

#include <inttypes.h>
#include <stddef.h>

#include <aitown/image_rgba.h>
#include <aitown/image_greyscale.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes an image type
///
/// This is used as a header structure, with actual data following it
typedef enum {

    // the color flag
    AITIMAGE_GREY = 0x0000,
    AITIMAGE_COLOR = 0x0001,

    // big-endian / small-endian flag
    AITIMAGE_BIG_ENDIAN = 0x0000,
    AITIMAGE_SMALL_ENDIAN = 0x0002,

    // the integer/floating point flag
    AITIMAGE_INTEGER = 0x0000,
    AITIMAGE_FPOINT = 0x0004,

    // the transparency flag
    AITIMAGE_ALPHA = 0x0000,
    AITIMAGE_NO_ALPHA = 0x0008,

    // size of the data
    AITIMAGE_SIZE_MASK = 0x00F0,
    AITIMAGE_SIZE_1 = 0x0000,
    AITIMAGE_SIZE_2 = 0x0010,
    AITIMAGE_SIZE_3 = 0x0020,
    AITIMAGE_SIZE_4 = 0x0030,
    AITIMAGE_SIZE_6 = 0x0040,
    AITIMAGE_SIZE_8 = 0x0050,
    AITIMAGE_SIZE_9 = 0x0060,
    AITIMAGE_SIZE_12 = 0x0070,
    AITIMAGE_SIZE_15 = 0x0080,
    AITIMAGE_SIZE_16 = 0x0090,
    AITIMAGE_SIZE_18 = 0x00A0,
    AITIMAGE_SIZE_24 = 0x00B0,
    AITIMAGE_SIZE_32 = 0x00C0,
    AITIMAGE_SIZE_64 = 0x00D0,
    AITIMAGE_SIZE_128 = 0x00E0,
    AITIMAGE_SIZE_256 = 0x00F0,

    // color space http://en.wikipedia.org/wiki/List_of_color_spaces_and_their_uses
    AITIMAGE_SPACE_MASK     = 0x0F00,
    AITIMAGE_SPACE_CMYK     = 0x0000,
    AITIMAGE_SPACE_CIE_1931 = 0x0100,
    AITIMAGE_SPACE_CIELUV   = 0x0200,
    AITIMAGE_SPACE_CIELAB   = 0x0300,
    AITIMAGE_SPACE_CIEUVW   = 0x0400,
    AITIMAGE_SPACE_sRGB     = 0x0500,
    AITIMAGE_SPACE_YUV      = 0x0600,
    AITIMAGE_SPACE_HSV      = 0x0700,
    AITIMAGE_SPACE_HSL      = 0x0800,

    AITIMAGE_SPACE_GREY_EQ          = 0x0000, // grey = R * G * B / max(R~G~B)
    AITIMAGE_SPACE_GREY_INTEGRAL    = 0x0100, // http://opensurf1.googlecode.com/files/opensurf.pdf


    // structure signature
    AITIMAGE_SIGNATURE_MASK = 0xFFFF0000,
    AITIMAGE_SIGNATURE      = 0x7AE50000,

    // predefined values
    AITIMAGE_RGBA8 =    AITIMAGE_COLOR |
                        AITIMAGE_BIG_ENDIAN |
                        AITIMAGE_INTEGER |
                        AITIMAGE_ALPHA |
                        AITIMAGE_SIZE_8 |
                        AITIMAGE_SPACE_sRGB |
                        AITIMAGE_SIGNATURE,

    AITIMAGE_GREY8 =    AITIMAGE_GREY |
                        AITIMAGE_BIG_ENDIAN |
                        AITIMAGE_INTEGER |
                        AITIMAGE_NO_ALPHA |
                        AITIMAGE_SIZE_8 |
                        AITIMAGE_SPACE_GREY_EQ |
                        AITIMAGE_SIGNATURE,

    AITIMAGE_GREY16 =   AITIMAGE_GREY |
                        AITIMAGE_BIG_ENDIAN |
                        AITIMAGE_INTEGER |
                        AITIMAGE_NO_ALPHA |
                        AITIMAGE_SIZE_16 |
                        AITIMAGE_SPACE_GREY_EQ |
                        AITIMAGE_SIGNATURE,

    AITIMAGE_GREY32 =   AITIMAGE_GREY |
                        AITIMAGE_BIG_ENDIAN |
                        AITIMAGE_INTEGER |
                        AITIMAGE_NO_ALPHA |
                        AITIMAGE_SIZE_32 |
                        AITIMAGE_SPACE_GREY_EQ |
                        AITIMAGE_SIGNATURE,

    AITIMAGE_GREYI =   AITIMAGE_GREY |
                        AITIMAGE_BIG_ENDIAN |
                        AITIMAGE_FPOINT |
                        AITIMAGE_NO_ALPHA |
                        AITIMAGE_SIZE_64 |
                        AITIMAGE_SPACE_GREY_INTEGRAL |
                        AITIMAGE_SIGNATURE


} aitimage_type_t;

//! compute the size of the buffer for common types of images
//! //@{
#define AITIMAGE_RGBA8_SIZE(w,h) ((w)*(h)*4)
#define AITIMAGE_GREY8_SIZE(w,h) ((w)*(h)*1)
#define AITIMAGE_GREY16_SIZE(w,h) ((w)*(h)*2)
#define AITIMAGE_GREY32_SIZE(w,h) ((w)*(h)*4)
#define AITIMAGE_GREYI_SIZE(w,h) ((w)*(h)*sizeof(double))
//! //@}


//! compute the size of the buffer for common types of images
//! //@{
#define AITIMAGE_GET_RGBA8_SIZE(_p_)    AITIMAGE_RGBA8_SIZE ((_p_)->width, (_p_)->height)
#define AITIMAGE_GET_GREY8_SIZE(_p_)    AITIMAGE_GREY8_SIZE ((_p_)->width, (_p_)->height)
#define AITIMAGE_GET_GREY16_SIZE(_p_)   AITIMAGE_GREY16_SIZE ((_p_)->width, (_p_)->height)
#define AITIMAGE_GET_GREY32_SIZE(_p_)   AITIMAGE_GREY32_SIZE ((_p_)->width, (_p_)->height)
#define AITIMAGE_GET_GREYI_SIZE(_p_)    AITIMAGE_GREYI_SIZE ((_p_)->width, (_p_)->height)
//! //@}


//! describes an image
///
/// This is used as a header structure, with actual data following it
typedef struct _aitimage_t {
    unsigned        width;  /**< width, in pixels */
    unsigned        height; /**< height, in pixels */
    aitimage_type_t type;   /**< combination of flags */
    unsigned        bufsz;  /**< size of the allocated buffer, excluding the header */
} aitimage_t;


//! compute total size of the buffer (header+data)
#define AITIMAGE_FULL_SIZE(sz) ((sz)+sizeof(aitimage_t))

//! compute total size of the buffer (header+data) from a structure
#define AITIMAGE_GET_FULL_SIZE(p) ((p)->bufsz+sizeof(aitimage_t))

//! check if a structure has proper kind
#define AITIMAGE_CHECK(p) (((p)->type & AITIMAGE_SIGNATURE_MASK)==AITIMAGE_SIGNATURE)

//! get a pointer to data
#define AITIMAGE_GET_DATA(p)    PTR_ADD((p),sizeof(aitimage_t))

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

//! prepare a preallocated structure
///
/// a buffer is assumed to follow the structure in memory
AITOWN_EXPORT void
aitimage_init (
        aitimage_t *img,
        unsigned width,
        unsigned height,
        aitimage_type_t type,
        unsigned bufsz);

//! allocate an image structure including the buffer
AITOWN_EXPORT func_error_t
aitimage_new (
        aitimage_t **img,
        unsigned width,
        unsigned height,
        aitimage_type_t type,
        unsigned bufsz);

//! free a memory structure
AITOWN_EXPORT func_error_t
aitimage_free (
        aitimage_t **img);

//! de-initialize
AITOWN_EXPORT void
aitimage_end (
        aitimage_t *img);

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
#endif /* AITOWN_image_h_INCLUDE */
