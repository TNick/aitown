/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-change.h
  \date			March 2014
  \author		TNick

  This file defines aitown_dejavu_change_t data structure that groups
  information related to detecting sudden changes in an image.

  The data uses two buffers that are switching places on each
  iteration, one being the cache and the other being used to extract data from
  current image.

  The input is divided into equal portions (the number of rows and
  columns is configurable at compile time) and an average is computed
  for each resulting cell. Based on the computed average and minimum-maximum
  values each point is being evaluated. If the change is high enough
  a bounding box is computed for virtual pixels that are above a certain
  treshold.

  A change in input image size induces changes in the size of the
  cells but not on the number of cells, so no reallocation takes place.
  If the size of the image is smaller than the number of rows/columns
  the structure keeps the image in top left corner and zeros the rest.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dejavu_change_h_INCLUDE
#define AITOWN_dejavu_change_h_INCLUDE
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
struct _aitown_cfg_sect_t;

// the number of rows and columns that we store
#ifndef AITOWN_DEJAVU_CHANGE_ROWS
#define AITOWN_DEJAVU_CHANGE_ROWS 16
#endif
#ifndef AITOWN_DEJAVU_CHANGE_COLS
#define AITOWN_DEJAVU_CHANGE_COLS 16
#endif

//! the total number of cells
#define AITOWN_DEJAVU_CHANGE_CELLS \
    ((AITOWN_DEJAVU_CHANGE_ROWS)*(AITOWN_DEJAVU_CHANGE_COLS))

//! total number of bytes taken by a buffer
#define AITOWN_DEJAVU_CHANGE_BUFF_SZ ((AITOWN_DEJAVU_CHANGE_CELLS)*4)

//! converting a RGB value to some sort of grey
#define AITOWN_DEJAVU_CHANGE_TO_GREY(_crt_) \
        ((AITIMAGE_RGBA_GET_R((_crt_)) * \
        AITIMAGE_RGBA_GET_G((_crt_)) * \
        AITIMAGE_RGBA_GET_B((_crt_))) / (1024))


//! maximum grey value; minimum is 0
#define AITOWN_DEJAVU_MAX_GREY (AITOWN_DEJAVU_CHANGE_TO_GREY(0xFFFFFFFF))


//! the callback used to inform attention about interesting spots
typedef void (*aitown_dejavu_change_kb_t) (
        void * payload,
        unsigned average,
        unsigned max
        );

//! the type of processing based on the geometry of our grid and of the image
///
typedef enum _aitown_dejavu_change_geom_t {
    AITOWN_DEJAVU_CHANGE_BOTH_SMALLER,
    AITOWN_DEJAVU_CHANGE_HORIZ_SMALLER,
    AITOWN_DEJAVU_CHANGE_VERT_SMALLER,
    AITOWN_DEJAVU_CHANGE_BOTH_LARGER
} aitown_dejavu_change_geom_t;

//! a square area inside an image; attention rectangle is one such area
typedef struct _aitown_dejavu_change_t {
    unsigned                pix_h; /**< pixels in each cell in horizontal direction */
    unsigned                pix_v; /**< pixels in each cell in vertical direction */
    unsigned                skip_h; /**< number of pixels to set to 0 on horizontal direction */
    unsigned                skip_v; /**< number of pixels to set to 0 on vertical direction */
    int                     geom; /**< is the image larger or smaller than our grid? */
    unsigned                cache_index; /**< tells us the index of the buffer used as cache */
    aitown_dejavu_change_kb_t   kb; /**< the callback used to inform attention */
    void *                      payload; /**< will be trasmitted to attention */
    uint32_t                buf_1[AITOWN_DEJAVU_CHANGE_BUFF_SZ]; /**< the first buffer */
    uint32_t                buf_2[AITOWN_DEJAVU_CHANGE_BUFF_SZ]; /**< the second buffer */
    uint32_t                buf_d[AITOWN_DEJAVU_CHANGE_BUFF_SZ]; /**< difference buffer */
} aitown_dejavu_change_t;


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
/// @param chg      address of the structure to initialise
/// @param width    width of the input image
/// @param height   height of the input image
///
AITOWN_EXPORT func_error_t
aitown_dejavu_change_init (
        struct _aitown_dejavu_change_t *chg,
        struct _aitown_cfg_sect_t * cfg_sect,
        unsigned width,
        unsigned height);


//! terminate
///
/// @param chg   address of the structure to terminate
///
AITOWN_EXPORT void
aitown_dejavu_change_end (
        struct _aitown_dejavu_change_t *chg);


//! informed that the geometry of the image has changed
///
/// @param chg      address of the structure
/// @param width    new width of the input image
/// @param height   new height of the input image
///
AITOWN_EXPORT void
aitown_dejavu_change_reinit (
        struct _aitown_dejavu_change_t *chg,
        unsigned width,
        unsigned height);

//! called to detect changes from previous runs
///
/// @warning the data inside image is never checked and assumed
/// to be of correct type and size.
/// 
/// @param chg      address of the structure
/// @param image    input data
///
AITOWN_EXPORT void
aitown_dejavu_change_detect (
        struct _aitown_dejavu_change_t *chg,
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
#endif /* AITOWN_dejavu_change_h_INCLUDE */
