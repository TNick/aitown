/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-image.c
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

#include "aitown-image.h"
#include <aitown/dbg_assert.h>

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

void aitimage_init (
        aitimage_t *img, unsigned width, unsigned height,
        aitimage_type_t type, unsigned bufsz)
{
    DBG_ASSERT (img != NULL);

    // full size
    unsigned sz = AITIMAGE_FULL_SIZE(bufsz);

    // clear the structure
    memset (img, 0, sz);

    // set known members
    img->width = width;
    img->height = height;
    img->type = type;
    img->bufsz = bufsz;

}

func_error_t aitimage_new (
        aitimage_t **img, unsigned width, unsigned height,
        aitimage_type_t type, unsigned bufsz)
{
    DBG_ASSERT (img != NULL);

    // full size
    unsigned sz = AITIMAGE_FULL_SIZE(bufsz);

    // allocate it
    aitimage_t *img_ret = (aitimage_t *)malloc(sz);
    if (img_ret == NULL) {
        return FUNC_MEMORY_ERROR;
    }

    // and prepare it
    aitimage_init (img_ret, width, height, type, bufsz);

    *img = img_ret;
    return FUNC_OK;
}

func_error_t aitimage_free (aitimage_t **img)
{
    DBG_ASSERT (img != NULL);
    DBG_ASSERT (*img != NULL);

    // make sure is a proper image
    if (AITIMAGE_CHECK(*img) == 0) {
        return FUNC_GENERIC_ERROR;
    }

    // terminate it
    aitimage_end (*img);

    // release the memory
    free (*img);

    // and that's that
    *img = NULL;
    return FUNC_OK;
}

void aitimage_end (aitimage_t *img)
{
    DBG_ASSERT (img != NULL);

    // clear the structure
    memset (img, 0, AITIMAGE_GET_FULL_SIZE(img));
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


