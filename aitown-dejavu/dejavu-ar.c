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

#include "aitown-dejavu.h"

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


//! a color
typedef struct _expanded_color_rgbg_t {
    unsigned                grey;
    unsigned                red;
    unsigned                green;
    unsigned                blue;
} expanded_color_rgbg_t;

//! a color
typedef union _expanded_color_t {
    expanded_color_rgbg_t   grgb;
    unsigned                ary[4];
} expanded_color_t;


static inline void expanded_color_generalise (
        expanded_color_t *source,
        expanded_color_t *destination,
        unsigned factor);


static inline u_int64_t expanded_color_pack_64(
        expanded_color_t *source);


static inline void expanded_color_unpack_64(
        u_int64_t source,
        expanded_color_t *destination);


static inline uint32_t expanded_color_pack_32(
        expanded_color_t *source);


static inline void expanded_color_unpack_32(
        uint32_t source,
        expanded_color_t *destination);


static inline void average_sample (
        const uint32_t* sample_start,
        unsigned sample_cols,
        unsigned sample_rows,
        unsigned row_len,
        expanded_color_t *color);


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
    adjust_to_size_change(ar, width, height);
#   else
    dbg_message ("Input image dimensions (%dx%d) do not match fixed values (%dx%d)",
                 width,
                 height,
                 aitown_dejavu_image_cols(ar),
                 aitown_dejavu_image_rows(ar));
#   endif
}


static func_error_t id_for_color (
        aitown_dejavu_t * dejavu, expanded_color_t *color)
{
    func_error_t ret = FUNC_OK;
    for (;;) {




        break;
    }

    return ret;

}

static inline func_error_t ids_for_cell (aitown_dejavu_t *dejavu, expanded_color_t * color)
{

    // compute generalised colors
    expanded_color_t color_2;
    expanded_color_generalise (color, &color_2, 2);
    expanded_color_t color_4;
    expanded_color_generalise (color, &color_4, 4);
    expanded_color_t color_8;
    expanded_color_generalise (color, &color_8, 8);
    expanded_color_t color_16;
    expanded_color_generalise (color, &color_16, 16);


    func_error_t ret = FUNC_OK;
    for (;;) {

        ret = id_for_color (dejavu, color);
        if (FUNC_OK != ret) break;
        ret = id_for_color (dejavu, &color_2);
        if (FUNC_OK != ret) break;
        ret = id_for_color (dejavu, &color_4);
        if (FUNC_OK != ret) break;
        ret = id_for_color (dejavu, &color_8);
        if (FUNC_OK != ret) break;
        ret = id_for_color (dejavu, &color_16);
        if (FUNC_OK != ret) break;

        ret = FUNC_OK;
        break;
    }
    return ret;
}

void aitown_dejavu_ar_process (
        aitown_dejavu_t *dejavu, const aitimage_t * image)
{
    aitown_dejavu_ar_t *ar = &dejavu->attrect;

    DBG_ASSERT (dejavu != NULL);
    DBG_ASSERT (image != NULL);
    DBG_ASSERT (image->width == ar->img_cols);
    DBG_ASSERT (image->height == ar->img_rows);
    DBG_ASSERT (image->type == AITIMAGE_RGBA8);

    func_error_t ret = FUNC_OK;

    // row and column in the grid
    unsigned r, c;

    // one row in the input image
    unsigned image_cols = ar->img_cols;

    // the width of a cell
    unsigned image_cell_width = ar->pix_horiz;
    unsigned image_cell_height = ar->pix_vert;

    // get a pointer to first row in input image
    // and adjust for offset in image
    const uint32_t *p_image_row = (const uint32_t *)AITIMAGE_GET_DATA(image);
    p_image_row += image_cols * ar->y + ar->x;

    // receives the average of a cell
    expanded_color_t    color;

    // points to the start of a cell in input image
    const uint32_t *p_image_cell;
    for (r = 0; r < ar->grid_rows; ++r) {
        p_image_cell = p_image_row;
        for (c = 0; c < ar->grid_cols; ++c) {

            // compute the average for this cell
            average_sample (
                        p_image_cell,
                        image_cell_width,
                        image_cell_height,
                        image_cols, &color);

            // now look-up this value and get coresponding id
            ret = ids_for_cell (dejavu, &color);
            if (ret != FUNC_OK) {
                // we're low on resources so drop the whole process
                return;
            }

            // step to next column
            p_image_cell += image_cell_width;
        }

        // step to next row
        p_image_row += image_cols;
    }


}


static inline void average_sample (
        const uint32_t* sample_start,
        unsigned sample_cols, unsigned sample_rows,
        unsigned row_len, expanded_color_t *color)
{

    int r, c, count = 0;
    const uint32_t* row_iter;
    uint32_t rgba;
    expanded_color_t crtcol;
    expanded_color_t medcol;

    medcol.grgb.grey = 0;
    medcol.grgb.red = 0;
    medcol.grgb.green = 0;
    medcol.grgb.blue = 0;

    for (r=0;r<sample_rows;r++) { // loop in rows

        // first pixel in this row
        row_iter = sample_start;

        for (c=0;c<sample_cols;c++) { // loop in columns

            // get current color
            rgba = *row_iter;
            crtcol.grgb.red = AITIMAGE_RGBA_GET_R(rgba);
            crtcol.grgb.green = AITIMAGE_RGBA_GET_G(rgba);
            crtcol.grgb.blue = AITIMAGE_RGBA_GET_B(rgba);
            crtcol.grgb.grey = (crtcol.grgb.red + crtcol.grgb.green + crtcol.grgb.blue)/3;

            // accumulate the values
            medcol.grgb.red += crtcol.grgb.red;
            medcol.grgb.green += crtcol.grgb.green;
            medcol.grgb.blue += crtcol.grgb.blue;
            medcol.grgb.grey += crtcol.grgb.grey;

            // step to next pixel
            count++; row_iter++;
        }

        // step to next row
        sample_start += row_len;
    }

    // compute the average for all these
    expanded_color_generalise (&medcol, color, count);

}

static inline void fit_ar_to_whole_image (aitown_dejavu_ar_t *ar)
{
    // compute number of pixels in each cell
    ar->pix_horiz = aitown_dejavu_image_cols(ar) / aitown_dejavu_grid_cols(ar);
    ar->pix_vert = aitown_dejavu_image_rows(ar) / aitown_dejavu_grid_rows(ar);

    // center resulted rectangle
    ar->x = (aitown_dejavu_image_cols(ar) - ar->pix_horiz*aitown_dejavu_grid_cols(ar))/2;
    ar->y = (aitown_dejavu_image_rows(ar) - ar->pix_vert*aitown_dejavu_grid_rows(ar))/2;
}

static void adjust_to_size_change (aitown_dejavu_ar_t *ar, unsigned w, unsigned h)
{

    // save new size
    ar->img_cols = w;
    ar->img_rows = h;

    // reset attention rectangle to new dimensions
    fit_ar_to_whole_image (ar);

}

static inline void expanded_color_generalise (
        expanded_color_t *source, expanded_color_t *destination, unsigned factor)
{
    destination->grgb.red = source->grgb.red / factor;
    destination->grgb.green = source->grgb.green / factor;
    destination->grgb.blue = source->grgb.blue / factor;
    destination->grgb.grey = source->grgb.grey / factor;
}

static inline u_int64_t expanded_color_pack_64(expanded_color_t *source)
{
    return
            ((u_int64_t)(source->grgb.grey & 0xFFFF) << 48) |
            ((u_int64_t)(source->grgb.red & 0xFFFF) << 32) |
            ((u_int64_t)(source->grgb.green & 0xFFFF) << 16) |
            ((u_int64_t)(source->grgb.blue & 0xFFFF) << 0);
}

static inline void expanded_color_unpack_64(u_int64_t source, expanded_color_t *destination)
{
    destination->grgb.grey = (source >> 48) & 0xFFFF;
    destination->grgb.red = (source >> 32) & 0xFFFF;
    destination->grgb.green = (source >> 16) & 0xFFFF;
    destination->grgb.blue = (source >> 0) & 0xFFFF;
}

static inline uint32_t expanded_color_pack_32(expanded_color_t *source)
{
    return
            ((uint32_t)(source->grgb.grey & 0xFF) << 24) |
            ((uint32_t)(source->grgb.red & 0xFF) << 16) |
            ((uint32_t)(source->grgb.green & 0xFF) << 8) |
            ((uint32_t)(source->grgb.blue & 0xFF) << 0);
}

static inline void expanded_color_unpack_32(uint32_t source, expanded_color_t *destination)
{
    destination->grgb.grey = (source >> 24) & 0xFF;
    destination->grgb.red = (source >> 16) & 0xFF;
    destination->grgb.green = (source >> 8) & 0xFF;
    destination->grgb.blue = (source >> 0) & 0xFF;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


