/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-ar.c
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "dejavu-ar.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! called when a change in input size was detected and we're allowed to do this
static void adjust_to_size_change (
        aitown_dejavu_ar_t *ar, unsigned width, unsigned height);

//! makes the ar take largest area from an image; also centers ar to image
static inline void fit_ar_to_whole_image (aitown_dejavu_ar_t *ar);


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

void aitown_dejavu_ar_init (
        aitown_dejavu_ar_t *ar,
        unsigned input_cols, unsigned input_rows,
        unsigned ar_cols, unsigned ar_rows)
{

    // save dimensions
    ar->img_cols = input_cols;
    ar->img_rows = input_rows;

    // save grid geometry
#   if AITOWN_DEJAVU_FIX_WIDTH <= 0
    ar->grid_cols = ar_cols;
#   else
    dejavu->width = AITOWN_DEJAVU_FIX_WIDTH;
    if ((ar_cols != 0) && (ar_cols != AITOWN_DEJAVU_FIX_WIDTH)) {
        dbg_message ("Warning! Width is fixed to %d", AITOWN_DEJAVU_FIX_WIDTH);
    }
#   endif

#   if AITOWN_DEJAVU_FIX_HEIGHT <= 0
    ar->grid_rows = ar_rows;
#   else
    dejavu->height = AITOWN_DEJAVU_FIX_HEIGHT;
    if ((ar_rows != 0) && (ar_cols != AITOWN_DEJAVU_FIX_HEIGHT)) {
        dbg_message ("Warning! Height is fixed to %d", AITOWN_DEJAVU_FIX_HEIGHT);
    }
#   endif

    // as the structure is part of a bigger one and is quite large we refrain from wasting time
    // memset (ar, 0, sizeof(aitown_dejavu_ar_t));

    adjust_to_size_change (ar, input_cols, input_rows);

}

void aitown_dejavu_ar_end (aitown_dejavu_ar_t *ar)
{
    // as the structure is part of a bigger one and is quite large we refrain from wasting time
    // memset (ar, 0, sizeof(aitown_dejavu_ar_t));
}

void aitown_dejavu_ar_reinit (
        aitown_dejavu_ar_t *ar, unsigned width, unsigned height)
{
#   ifdef AITOWN_DEJAVU_INPUT_IS_VARIABLE
    adjust_to_size_change(ar, image);
#   else
    dbg_message ("Input image dimensions (%dx%d) do not match fixed values (%dx%d)",
                 width,
                 height,
                 aitown_dejavu_image_cols(ar),
                 aitown_dejavu_image_rows(ar));
#   endif
}

void aitown_dejavu_ar_process (
        aitown_dejavu_ar_t *ar, const aitimage_t * image)
{
    DBG_ASSERT (ar != NULL);
    DBG_ASSERT (image != NULL);


}

static inline void fit_ar_to_whole_image (aitown_dejavu_ar_t *ar)
{
    // compute number of pixels in each cell
    ar->pix_horiz = aitown_dejavu_image_cols(ar) / aitown_dejavu_grid_cols(ar);
    ar->pix_vert = aitown_dejavu_image_rows(ar) / aitown_dejavu_grid_rows(ar);

    // center resulted rectangle
    ar->x = (aitown_dejavu_image_cols(ar) - ar->pix_horiz*aitown_dejavu_grid_cols(ar))/2;
    ar->y = (aitown_dejavu_image_rows(ar) - ar->pix_horiz*aitown_dejavu_grid_rows(ar))/2;
}

static void adjust_to_size_change (aitown_dejavu_ar_t *ar, unsigned w, unsigned h)
{

    // save new size
    ar->img_cols = w;
    ar->img_rows = h;

    // reset attention rectangle to new dimensions
    fit_ar_to_whole_image (ar);

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


