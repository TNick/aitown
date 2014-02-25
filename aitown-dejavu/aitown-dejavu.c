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
#include <aitown/aitown-core.h>

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

void aitown_dejavu_init (
        aitown_dejavu_t *dejavu,
        aitown_core_t * core,
        unsigned input_cols, unsigned input_rows,
        unsigned ar_cols, unsigned ar_rows)
{
    DBG_ASSERT (dejavu != NULL);
    DBG_ASSERT (core != NULL);

    // clear all, including substructures
    memset (dejavu, 0, sizeof(aitown_dejavu_t));

    // initialize the sub-structures
    aitown_dejavu_change_init (&dejavu->chg, input_cols, input_rows);
    aitown_dejavu_ar_init (
                &dejavu->attrect,
                input_cols, input_rows,
                ar_cols, ar_rows);

    dejavu->core = core;
}


void aitown_dejavu_end (aitown_dejavu_t *dejavu)
{

    // terminate the sub-structures
    aitown_dejavu_ar_end (&dejavu->attrect);
    aitown_dejavu_change_end (&dejavu->chg);

    // clear all, including substructures
    memset (dejavu, 0, sizeof(aitown_dejavu_t));
}


void aitown_dejavu_feed (aitown_dejavu_t *dejavu, const aitimage_t * image)
{
    DBG_ASSERT (dejavu != NULL);
    DBG_ASSERT (image != NULL);

    // check dimensions
    if ((aitown_dejavu_image_cols(&dejavu->attrect) != image->width) ||
        (aitown_dejavu_image_rows(&dejavu->attrect) != image->height)) {

        // inform sub-structures
        aitown_dejavu_change_reinit (&dejavu->chg, image->width, image->height);
        aitown_dejavu_change_reinit (&dejavu->chg, image->width, image->height);

    }

    // detect changes:
    aitown_dejavu_change_detect (&dejavu->chg, image);
    
    // interpret the content of the AR
    aitown_dejavu_ar_process (dejavu, image);

}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


