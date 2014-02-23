/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dejavu.c
  \date			September 2013
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

#include "aitown-dejavu.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>

#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

void aitown_dejavu_init (aitown_dejavu_t *dejavu, unsigned width, unsigned height)
{
    memset (dejavu, 0, sizeof(aitown_dejavu_t));

    // save dimensions
#   if AITOWN_DEJAVU_FIX_WIDTH <= 0
    dejavu->width = width;
#   else
    dejavu->width = AITOWN_DEJAVU_FIX_WIDTH;
    if (width != 0) {
        dbg_message ("Warning! Width is fixed to %d", AITOWN_DEJAVU_FIX_WIDTH);
    }
#   endif

#   if AITOWN_DEJAVU_FIX_HEIGHT <= 0
    dejavu->height = height;
#   else
    dejavu->height = AITOWN_DEJAVU_FIX_HEIGHT;
    if (height != 0) {
        dbg_message ("Warning! Height is fixed to %d", AITOWN_DEJAVU_FIX_HEIGHT);
    }
#   endif

    // initialize the structure tracking changes
    aitown_dejavu_change_init (&dejavu->chg, dejavu->width, dejavu->height);
}


void aitown_dejavu_end (aitown_dejavu_t *dejavu)
{

    // terminate the structure tracking changes
    aitown_dejavu_change_end (&dejavu->chg);

    memset (dejavu, 0, sizeof(aitown_dejavu_t));
}


//! makes the ar take largest area from an image; also centers ar to image
static inline void fit_ar_to_whole_image (aitown_dejavu_t *dejavu)
{
    aitown_dejavu_area_t *ar = &dejavu->ar;

#ifdef AITOWN_DEJAVU_AR_SQUARE
    unsigned side;
    unsigned pix;
    // the ar must remain square; we take shortest side and ignore
    // some margins on one direction
    if (dejavu->width > dejavu->height) {
        side = dejavu->height;
        ar->x = (dejavu->width - side)/2;
        ar->y = 0;
    } else {
        ar->x = 0;
        side = dejavu->width;
        ar->y = (dejavu->height - side)/2;
    }
    //pix = side / ar->size;
    /** @todo check this routine */
#else
    // the resulted cells are non-square; we can adapt to any shape
    ar->x = 0;
    ar->y = 0;
    ar->pix_w = dejavu->width / ar->width;
    ar->pix_h = dejavu->height / ar->width;
#endif
}


# ifdef AITOWN_DEJAVU_INPUT_IS_VARIABLE
//! called when a change in input size was detected and we're allowed to do this
static void adjust_to_size_change (aitown_dejavu_t *dejavu, const aitimage_t * image)
{

    // save new size
    dejavu->width = image->width;
    dejavu->height = image->height;

    // reset attention rectangle to new dimensions
    fit_ar_to_whole_image (dejavu);
    
    // inform tracking structure
    aitown_dejavu_change_reinit (&dejavu->chg, image->width, image->height);
    
}
#endif


void aitown_dejavu_feed (aitown_dejavu_t *dejavu, const aitimage_t * image)
{
    DBG_ASSERT (dejavu != NULL);
    DBG_ASSERT (image != NULL);

    // check dimensions
    if ((aitown_dejavu_get_width(dejavu) != image->width) ||
        (aitown_dejavu_get_height(dejavu) != image->height)) {

#       ifdef AITOWN_DEJAVU_INPUT_IS_VARIABLE
        adjust_to_size_change(dejavu, image);
#       else
        dbg_message ("Input image dimensions (%dx%d) do not match fixed values (%dx%d)",
                     image->width,
                     image->height,
                     aitown_dejavu_get_width(dejavu),
                     aitown_dejavu_get_height(dejavu));
        return;
#       endif
    }

    // detect changes:
    aitown_dejavu_change_detect (&dejavu->chg, image);
    
    // interpret the content of the





}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


