/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			image_greyscale.c
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

func_error_t aitimage_grey16_from_rgba8_ptr (
        const struct _aitimage_t *source, struct _aitimage_t *destination,
        const uint32_t *p_src, uint16_t *p_dst)
{
    DBG_ASSERT (source != NULL);
    DBG_ASSERT (destination != NULL);

    // make sure input is sane
    if ((source->type != AITIMAGE_RGBA8) ||
        (destination->type != AITIMAGE_GREY16)) {
        return FUNC_BAD_INPUT;
    }
    DBG_ASSERT (AITIMAGE_GET_RGBA8_SIZE (source) >= source->bufsz);
    DBG_ASSERT (AITIMAGE_GET_GREY16_SIZE (destination) >= destination->bufsz);

    // precompute size
    destination->width = source->width;
    destination->height = source->height;
    unsigned len = source->width * source->height;

    /** @todo speed up this loop */
    int i;
    uint32_t crt;
    for (i = 0; i < len; i++) {
        crt = *p_src;
        *p_dst = (AITIMAGE_RGBA_GET_R(crt) * // get it in the range 0 - 65535
                AITIMAGE_RGBA_GET_G(crt) *
                AITIMAGE_RGBA_GET_B(crt)) / 256;

        // step to next entry
        p_src++; p_dst++;
    }

    return FUNC_OK;
}

func_error_t aitimage_grey16_from_rgba8 (
        const struct _aitimage_t *source, struct _aitimage_t *destination)
{
    return aitimage_grey16_from_rgba8_ptr(
                source,
                destination,
                (uint32_t *)AITIMAGE_GET_DATA (source),
                (uint16_t *)AITIMAGE_GET_DATA (destination)
                );
}

func_error_t aitimage_greyi_from_rgba8_ptr (
        const struct _aitimage_t *source, struct _aitimage_t *destination,
        const uint32_t *p_src, double *p_dst)
{
    DBG_ASSERT (source != NULL);
    DBG_ASSERT (source->width > 0);
    DBG_ASSERT (source->height > 0);
    DBG_ASSERT (destination != NULL);
    DBG_ASSERT (p_src != NULL);
    DBG_ASSERT (p_dst != NULL);

    // make sure input is sane
    if ((source->type != AITIMAGE_RGBA8) ||
        (destination->type != AITIMAGE_GREYI)) {
        return FUNC_BAD_INPUT;
    }
    // precompute size
    int width = source->width;
    int height = source->height;
    destination->width = width;
    destination->height = height;

    DBG_ASSERT (AITIMAGE_GET_RGBA8_SIZE (source) >= source->bufsz);
    DBG_ASSERT (AITIMAGE_GET_GREYI_SIZE (destination) >= destination->bufsz);

    // iterators
    double *p_dst_above = p_dst;

    // prepare
    uint64_t rs = 0;
    uint64_t crt;
    int i, j;

#   undef  AITIMAGE_LOC_TO_GREY
    // loosing precision but gaining speed (does not uses div)
#   define AITIMAGE_LOC_TO_GREY(_crt_) \
        ((AITIMAGE_RGBA_GET_R((crt)) * \
        AITIMAGE_RGBA_GET_G((crt)) * \
        AITIMAGE_RGBA_GET_B((crt))) / (1024*16));

    // first row
    for (i = 0; i < width; ++i) {

        crt = *p_src;
        rs += AITIMAGE_LOC_TO_GREY(crt);
        *p_dst = rs;

        // step to next entry
        p_src++; p_dst++;
    }

    // if all the pixels are white (1023), for an image with resolution 1024x1024
    // the sum for bottom right is below 0x4000_0000 => plenty of space in uint64_t

    // remaining cells are sum above and to the left
    for (j = 1; j < height; ++j) {
        rs = 0;
        for (i = 0; i < width; ++i) {

            crt = *p_src;
            rs += AITIMAGE_LOC_TO_GREY(crt);
            *p_dst = rs + *p_dst_above;

            // step to next entry
            p_src++; p_dst++; p_dst_above++;
        }
    }
#   undef  AITIMAGE_LOC_TO_GREY

    return FUNC_OK;
}

func_error_t aitimage_greyi_from_rgba8 (
        const struct _aitimage_t *source, struct _aitimage_t *destination)
{
    return aitimage_greyi_from_rgba8_ptr(
                source,
                destination,
                (uint32_t *)AITIMAGE_GET_DATA (source),
                (double *)AITIMAGE_GET_DATA (destination)
                );
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


