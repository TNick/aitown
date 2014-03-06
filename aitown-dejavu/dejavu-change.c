/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-change.c
  \date			March 2014
  \author		TNick


  @todo set change callback to point to attention
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

#include "dejavu-change.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>
#include <aitown/utils_unused.h>

#include <aitown/aitown-cfg.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

func_error_t aitown_dejavu_change_init (
        aitown_dejavu_change_t *chg, aitown_cfg_sect_t * cfg_sect,
        unsigned width, unsigned height)
{
    VAR_UNUSED(cfg_sect);

    // as the structure is part of a bigger one and is quite large we refrain from wasting time
    // memset (chg, 0, sizeof(aitown_dejavu_change_t));

    aitown_dejavu_change_reinit (chg, width, height);

    return FUNC_OK;
}


void aitown_dejavu_change_end (aitown_dejavu_change_t *chg)
{
    // as the structure is part of a bigger one and is quite large we refrain from wasting time
    // memset (chg, 0, sizeof(aitown_dejavu_change_t));
}

void aitown_dejavu_change_reinit (
        aitown_dejavu_change_t *chg, unsigned width, unsigned height)
{
    // how the grid fits with the window
    if (width > AITOWN_DEJAVU_CHANGE_COLS) {
        if (height > AITOWN_DEJAVU_CHANGE_ROWS) {
            chg->geom = AITOWN_DEJAVU_CHANGE_BOTH_LARGER;
            chg->pix_h = width / AITOWN_DEJAVU_CHANGE_COLS;
            chg->pix_v = height / AITOWN_DEJAVU_CHANGE_ROWS;
            chg->skip_h = width - (AITOWN_DEJAVU_CHANGE_COLS * chg->pix_h);
            chg->skip_v = height - (AITOWN_DEJAVU_CHANGE_ROWS * chg->pix_v);
        } else {
            chg->geom = AITOWN_DEJAVU_CHANGE_VERT_SMALLER;
            chg->pix_h = width / AITOWN_DEJAVU_CHANGE_COLS;
            chg->pix_v = 1;
            chg->skip_h = height - (AITOWN_DEJAVU_CHANGE_ROWS * chg->pix_v);
            chg->skip_v = AITOWN_DEJAVU_CHANGE_ROWS - height;
        }
    } else {
        if (height > AITOWN_DEJAVU_CHANGE_ROWS) {
            chg->geom = AITOWN_DEJAVU_CHANGE_HORIZ_SMALLER;
            chg->pix_h = 1;
            chg->pix_v = height / AITOWN_DEJAVU_CHANGE_ROWS;
            chg->skip_h = AITOWN_DEJAVU_CHANGE_COLS - width;
            chg->skip_v = height - (AITOWN_DEJAVU_CHANGE_ROWS * chg->pix_v);
        } else {
            chg->geom = AITOWN_DEJAVU_CHANGE_BOTH_SMALLER;
            chg->pix_v = 1;
            chg->pix_h = 1;
            chg->skip_h = AITOWN_DEJAVU_CHANGE_COLS - width;
            chg->skip_v = AITOWN_DEJAVU_CHANGE_ROWS - height;
        }
    }
}

static uint32_t average_cell(const uint32_t * p_src, unsigned img_w, unsigned pix_h, unsigned pix_v)
{
    int c, r;
    int cnt = 0;


    uint64_t sum = 0;
    const uint32_t * p_row = p_src;
    uint32_t grey;
    uint32_t rgba;
    for(r=0; r<pix_v; r++) {
        for(c=0; c<pix_h; c++) {
            rgba = *p_src;
            grey = AITOWN_DEJAVU_CHANGE_TO_GREY(rgba);
            sum += grey;
            p_src++; cnt++;
        }
        p_row += img_w;
        p_src = p_row;
    }
    return sum/cnt;
}

static void aitown_dejavu_change_detect_l (
        aitown_dejavu_change_t *chg, const aitimage_t * image,
        const uint32_t * p_cache, uint32_t * p_dest)
{
    /** @warning naive implementation */

    uint32_t * p_dif = chg->buf_d;
    uint32_t grey;
    uint32_t other_grey;
    unsigned d_grey_min = UINT_MAX;
    unsigned d_grey_max = 0;
    uint64_t d_grey_sum = 0;
    const uint32_t * p_image_row = (uint32_t *)AITIMAGE_GET_DATA(image);
    const uint32_t * p_image = p_image_row;

    int c, r;
    unsigned img_w = (chg->pix_h * AITOWN_DEJAVU_CHANGE_COLS) + chg->skip_h;
    unsigned c_lim = AITOWN_DEJAVU_CHANGE_COLS;
    unsigned r_lim = AITOWN_DEJAVU_CHANGE_ROWS;
    for (r=0; r<r_lim; ++r) {
        for (c=0; c<c_lim; ++c) {

            // compute the offset to first pixel in this cell
            grey = average_cell(p_image, img_w, chg->pix_h, chg->pix_v);

            other_grey = *p_cache;
            *p_dest = grey;
            grey = (other_grey > grey ? other_grey - grey : grey - other_grey);
            d_grey_min = (d_grey_min < grey ? d_grey_min : grey);
            d_grey_max = (d_grey_max > grey ? d_grey_max : grey);
            d_grey_sum += grey;
            *p_dif = grey;

            p_image += chg->pix_h;
            p_dest++; p_cache++; p_dif++;
        }
        p_image_row += img_w*chg->pix_v;
        p_image = p_image_row;
    }

    // compute the average
    unsigned d_average = (unsigned)(d_grey_sum / AITOWN_DEJAVU_CHANGE_CELLS);

    // and the spread
    unsigned spread = d_grey_max - d_grey_min;

    // only if the change is large we go on to compute a bounding rectangle
    if (spread < (AITOWN_DEJAVU_MAX_GREY/4) ) {
        return;
    }

    // compute bounding rectangle by selecting only those above average
    int r_min = INT_MAX;
    int r_max = -INT_MAX;
    int c_min = INT_MAX;
    int c_max = -INT_MAX;
    p_dif = chg->buf_d;
    for (c=0; c<c_lim; ++c) {
        for (r=0; r<r_lim; ++r) {
            if (*p_dif > d_average) {
                c_min = (c_min > r ? r : c_min);
                c_max = (c_max < r ? r : c_max);
                r_min = (r_min > c ? c : r_min);
                r_max = (r_max < c ? c : r_max);
            }
            p_dif++;
        }
    }

    // we need to multiply to get
    // real coordinates
    r_min *= chg->pix_v;
    r_max *= chg->pix_v;
    c_min *= chg->pix_h;
    c_max *= chg->pix_h;

    // inform attention module about it
    d_grey_max -= d_grey_min;
    d_average -= d_grey_min;
    if (chg->kb != NULL) {
        chg->kb (chg->payload, d_average, d_grey_max);
    }

}

static void aitown_dejavu_change_detect_s (
        aitown_dejavu_change_t *chg, const aitimage_t * image,
        const uint32_t * p_cache, uint32_t * p_dest)
{
    /** @warning naive implementation */

    uint32_t * p_dif = chg->buf_d;
    unsigned advance = chg->skip_h;
    uint32_t rgba;
    uint32_t grey;
    uint32_t other_grey;
    const uint32_t * p_image = (uint32_t *)AITIMAGE_GET_DATA(image);
    unsigned d_grey_min = UINT_MAX;
    unsigned d_grey_max = 0;
    uint64_t d_grey_sum = 0;
    int i, j;

    
    // each pixel represents a cell, so there's no averaging required
    // there is some unused space at the end of each row and
    // there are some rows that are unused in the bottom portion
    unsigned i_lim = AITOWN_DEJAVU_CHANGE_COLS - chg->skip_h;
    unsigned j_lim = AITOWN_DEJAVU_CHANGE_ROWS - chg->skip_v;
    for (j=0; j<j_lim; ++j) {
        for (i=0; i<i_lim; ++i) {
            rgba = *p_image;
            grey = AITOWN_DEJAVU_CHANGE_TO_GREY(rgba);
            other_grey = *p_cache;
            *p_dest = grey;
            grey = (other_grey > grey ? other_grey - grey : grey - other_grey);
            d_grey_min = (d_grey_min < grey ? d_grey_min : grey);
            d_grey_max = (d_grey_max > grey ? d_grey_max : grey);
            d_grey_sum += grey;
            *p_dif = grey;
            p_dest++; p_cache++; p_dif++; p_image++;
        }
        p_dest += advance;
        p_cache += advance;
        p_dif += advance;
    }

    // compute the average
    unsigned d_average = (unsigned)(d_grey_sum / AITOWN_DEJAVU_CHANGE_CELLS);

    // and the spread
    unsigned spread = d_grey_max - d_grey_min;

    // only if the change is large we go on to compute a bounding rectangle
    if (spread < (AITOWN_DEJAVU_MAX_GREY/4) ) {
        return;
    }

    // compute bounding rectangle by selecting only those above average
    int r_min = INT_MAX;
    int r_max = -INT_MAX;
    int c_min = INT_MAX;
    int c_max = -INT_MAX;
    p_dif = chg->buf_d;
    for (j=0; j<j_lim; ++j) {
        for (i=0; i<i_lim; ++i) {
            if (*p_dif > d_average) {
                c_min = (c_min > i ? i : c_min);
                c_max = (c_max < i ? i : c_max);
                r_min = (r_min > j ? j : r_min);
                r_max = (r_max < j ? j : r_max);
            }
            p_dif++;
        }
        p_dif += advance;
    }

    // since each cell is a pixel we don't need to multiply to get
    // real coordinates

    // inform attention module about it
    d_grey_max -= d_grey_min;
    d_average -= d_grey_min;
    if (chg->kb != NULL) {
        chg->kb (chg->payload, d_average, d_grey_max);
    }

}

static void aitown_dejavu_change_detect_hs (
        aitown_dejavu_change_t *chg, const aitimage_t * image,
        const uint32_t * p_cache, uint32_t * p_dest)
{
    /** @todo implement aitown_dejavu_change_detect_hs() */
}

static void aitown_dejavu_change_detect_vs (
        aitown_dejavu_change_t *chg, const aitimage_t * image,
        const uint32_t * p_cache, uint32_t * p_dest)
{
    /** @todo implement aitown_dejavu_change_detect_vs() */
}

void aitown_dejavu_change_detect (
        aitown_dejavu_change_t *chg, const aitimage_t * image)
{
    DBG_ASSERT (chg != NULL);
    DBG_ASSERT (image != NULL);

    // switch buffers
    uint32_t * p_cache;
    uint32_t * p_dest;
    if (chg->cache_index == 0) {
        p_cache = chg->buf_1;
        p_dest = chg->buf_2;
        chg->cache_index = 1;
    } else {
        p_dest = chg->buf_1;
        p_cache = chg->buf_2;
        chg->cache_index = 0;
    }

    // select the type of processing
    switch (chg->geom) {
    case AITOWN_DEJAVU_CHANGE_BOTH_LARGER:
        aitown_dejavu_change_detect_l (chg, image, p_cache, p_dest);
        break;
    case AITOWN_DEJAVU_CHANGE_VERT_SMALLER:
        aitown_dejavu_change_detect_vs (chg, image, p_cache, p_dest);
        break;
    case AITOWN_DEJAVU_CHANGE_HORIZ_SMALLER:
        aitown_dejavu_change_detect_hs (chg, image, p_cache, p_dest);
        break;
    case AITOWN_DEJAVU_CHANGE_BOTH_SMALLER:
        aitown_dejavu_change_detect_s (chg, image, p_cache, p_dest);
        break;
    }

}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


