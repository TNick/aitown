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
#include <aitown/utils_unused.h>

#include <aitown/aitown-cfg.h>
#include <aitown/aitown-dstorage.h>
#include <aitown/sglib.h>

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>
#include <aitown/aitown-db-free.h>

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
    uint64_t                packed;
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


static inline uint64_t expanded_color_pack_64(
        expanded_color_t *source);


static inline void expanded_color_unpack_64(
        uint64_t source,
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


//! an entry describing a level II item and its probability
typedef struct _aitown_dejavu_level_II_t {
    aitown_dstorage_id_t    id;
    uint64_t probab_sum;
    uint64_t probab_count;
    int red;
    struct _aitown_dejavu_level_II_t * left;
    struct _aitown_dejavu_level_II_t * right;
} aitown_dejavu_level_II_t;

#define AITOWN_DEJAVU_LEVELII_CMPARATOR(x,y) ((x->id)-(y->id))

SGLIB_DEFINE_RBTREE_PROTOTYPES(aitown_dejavu_level_II_t, left, right, red, AITOWN_DEJAVU_LEVELII_CMPARATOR)
SGLIB_DEFINE_RBTREE_FUNCTIONS(aitown_dejavu_level_II_t, left, right, red, AITOWN_DEJAVU_LEVELII_CMPARATOR)


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

func_error_t aitown_dejavu_ar_init (
        aitown_dejavu_ar_t * ar,
        aitown_db_mng_t * db_mng,
        aitown_cfg_sect_t * cfg_sect,
        unsigned input_cols, unsigned input_rows,
        unsigned ar_cols, unsigned ar_rows)
{
    VAR_UNUSED(cfg_sect);
    func_error_t ret = FUNC_OK;

    // save grid geometry
#   if AITOWN_DEJAVU_INPUT_FIX_WIDTH > 0
    if ((input_cols != 0) && (input_cols != AITOWN_DEJAVU_INPUT_FIX_WIDTH)) {
        dbg_message ("Warning! Width is fixed to %d", AITOWN_DEJAVU_INPUT_FIX_WIDTH);
    }
    input_cols = AITOWN_DEJAVU_INPUT_FIX_WIDTH;
#   endif

#   if AITOWN_DEJAVU_INPUT_FIX_HEIGHT > 0
    if ((input_rows != 0) && (input_rows != AITOWN_DEJAVU_INPUT_FIX_HEIGHT)) {
        dbg_message ("Warning! Height is fixed to %d", AITOWN_DEJAVU_INPUT_FIX_HEIGHT);
    }
    input_rows = AITOWN_DEJAVU_INPUT_FIX_HEIGHT;
#   endif

    // save attention rectangle dimensions
    ar->grid_cols = ar_cols;
    ar->grid_rows = ar_rows;

    // as the structure is part of a bigger one and is quite large we refrain from wasting time
    // memset (ar, 0, sizeof(aitown_dejavu_ar_t));

    adjust_to_size_change (ar, input_cols, input_rows);


    for (;;) {

        ret = dejavu_level_I_mng_init (&ar->level_I_mng, db_mng, cfg_sect);
        if (ret == FUNC_OK) break;
        ret = dejavu_level_II_mng_init (&ar->level_II_mng, db_mng, cfg_sect);
        if (ret == FUNC_OK) break;

        break;
    }

    return ret;
}

void aitown_dejavu_ar_end (aitown_dejavu_ar_t *ar)
{
    // stop level managers
    dejavu_level_I_mng_end (&ar->level_I_mng);
    dejavu_level_II_mng_end (&ar->level_II_mng);

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

    //        // read the list
    //        const void * data;
    //        size_t datasz;
    //        aitown_db_read_t param;
    //        param.db = dejavu->attrect.db;
    //        param.key = &color->grgb.packed;
    //        param.key_sz = sizeof(uint64_t);
    //        param.val = &data;
    //        param.val_sz = &datasz;
    //        param.kb = NULL;
    //        param.user = NULL;
    //        ret = aitown_db_read_ex (&param);
    //        if (ret == FUNC_NOT_FOUND) {
    //            // no record for this, yet
    //        } else if (ret != FUNC_OK) {
    //            break;
    //        }

    //        // we're expecting a list of id-probability pairs
    //        aitown_dstorage_id_t * level_II_id = (aitown_dstorage_id_t*)data;
    //        uint64_t * probab = (uint64_t *)PTR_ADD(data, sizeof(aitown_dstorage_id_t));
    //        size_t step = sizeof(aitown_dstorage_id_t) + sizeof(uint64_t);
    //        size_t data_parsed = 0;
    //        aitown_dejavu_level_II_t * loc_it = NULL;
    //        aitown_dejavu_level_II_t * loc_it_used = NULL;
    //        aitown_dstorage_id_t crt_levelII_id;
    //        while (data_parsed < datasz) {

    //            // allocate a new structure for this entry
    //            crt_levelII_id = *level_II_id;
    //            if (loc_it == NULL) {
    //                loc_it = new_level_II (&dejavu->attrect, crt_levelII_id);
    //                if (loc_it == NULL) {
    //                    ret = FUNC_MEMORY_ERROR;
    //                    break;
    //                }
    //            } else {
    //                loc_it->id = crt_levelII_id;
    //            }

    //            // insert it only if not already there
    //            if (0 == sglib_aitown_dejavu_level_II_t_add_if_not_member (
    //                        &dejavu->attrect.tree_level_II, loc_it, &loc_it_used) ) {
    //                // was not inserted because i already in the tree
    //                DBG_ASSERT (loc_it_used->probab_sum > 0);
    //                DBG_ASSERT (loc_it_used->probab_count > 0);
    //            } else {
    //                // was inserted
    //                loc_it = NULL; // on next pass we'll have to get a new one
    //                DBG_ASSERT (loc_it_used->probab_sum == 0);
    //                DBG_ASSERT (loc_it_used->probab_count == 0);
    //            }

    //            // add the probability for this id
    //            loc_it_used->probab_sum += *probab;
    //            loc_it_used->probab_count += 1;


    //            // step to next pair
    //            level_II_id = (aitown_dstorage_id_t*)PTR_ADD(level_II_id, step);
    //            probab = (uint64_t *)PTR_ADD(probab, step);
    //            data_parsed += step;
    //        }

    //        // free the chunk that we have received from database
    //        aitown_db_free (dejavu->attrect.db, &data);


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
/** @todo what else ? */

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
    destination->grgb.packed = expanded_color_pack_64 (source);
}

static inline uint64_t expanded_color_pack_64(expanded_color_t *source)
{
    return
            ((uint64_t)(source->grgb.grey & 0xFFFF) << 48) |
            ((uint64_t)(source->grgb.red & 0xFFFF) << 32) |
            ((uint64_t)(source->grgb.green & 0xFFFF) << 16) |
            ((uint64_t)(source->grgb.blue & 0xFFFF) << 0);
}

static inline void expanded_color_unpack_64(uint64_t source, expanded_color_t *destination)
{
    destination->grgb.grey = (source >> 48) & 0xFFFF;
    destination->grgb.red = (source >> 32) & 0xFFFF;
    destination->grgb.green = (source >> 16) & 0xFFFF;
    destination->grgb.blue = (source >> 0) & 0xFFFF;
    destination->grgb.packed = source;
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
    destination->grgb.packed = expanded_color_pack_64 (destination);
}



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


