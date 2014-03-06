/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "test_image_16x16.h"
#include "test_image_16x32.h"
#include "test_image_22x16.h"
#include "test_image.h"
#include "test_image_1.h"

#include <aitown/aitown-image.h>
#include <aitown/aitown-dejavu.h>
#include <aitown/aitown-core.h>

#include "../config-builder.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

double getRealTime();

#ifdef __cplusplus
}
#endif

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

//gimp_image_t * gi1g = getTestImage(1);

/*
test_image_16x16;
test_image_16x32;
test_image_22x16;
*/

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

static void kb_aitown_dejavu_change_kb (
        void * payload, unsigned average, unsigned max)
{
    aitimage_t * img = (aitimage_t *)payload;

    if (img->width == 16) {
        if (img->height == 16) {
            EXPECT_EQ(average, 2019);
            EXPECT_EQ(max, 14114);
        } else if (img->height == 32) {
            EXPECT_EQ(average, 0);
            EXPECT_EQ(max, 0);

        }
    } else if (img->width == 22) {
        EXPECT_EQ(average, 0);
        EXPECT_EQ(max, 0);

    } else if (img->width == 1200) {
        /*
        EXPECT_EQ(average, 0);
        EXPECT_EQ(max, 0);
        */
    }
}

TEST(dejavu,image) {

    aitown_core_t core;
    aitown_dejavu_t dejavu;
    aitimage_t *img_1616;
    aitimage_t *img_1632;
    aitimage_t *img_2216;
    aitimage_t *img_gi1g;
    func_error_t ret;

    const char * tmp_file = write_config_file ("test_dejavu", CFGTEST_ALL);

    aitown_core_finit (&core, tmp_file);

    ret = aitimage_new (&img_1616, 16, 16, AITIMAGE_RGBA8, 16*16*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_1616), test_image_16x16, 16*16*4);

    ret = aitimage_new (&img_1632, 16, 32, AITIMAGE_RGBA8, 16*32*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_1632), test_image_16x32, 16*32*4);

    ret = aitimage_new (&img_2216, 22, 16, AITIMAGE_RGBA8, 22*16*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_2216), test_image_22x16, 22*16*4);

    gimp_image_t * gi1g = getTestImage(1);
    size_t sz_gi1g = gi1g->width*gi1g->height*gi1g->bytes_per_pixel;
    ret = aitimage_new (&img_gi1g,
                        gi1g->width, gi1g->height,
                        AITIMAGE_RGBA8,
                        sz_gi1g);
    EXPECT_TRUE (gi1g->bytes_per_pixel == 4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_gi1g), gi1g->pixel_data, sz_gi1g);


    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, tmp_file);
    EXPECT_TRUE (ret == FUNC_OK);

    aitown_cfg_leaf_t * leaf_ar_cols;
    aitown_cfg_leaf_t * leaf_ar_rows;
    aitown_cfg_leaf_t * leaf_input_cols;
    aitown_cfg_leaf_t * leaf_input_rows;
    aitown_cfg_sect_t * sect_dejavu;
    sect_dejavu =
            aitown_cfg_get_sect (&cfg->root, "dejavu");
    leaf_input_cols =
            aitown_cfg_get_leaf (sect_dejavu, "input_cols");
    leaf_input_rows =
            aitown_cfg_get_leaf (sect_dejavu, "input_rows");
    leaf_ar_cols =
            aitown_cfg_get_leaf (sect_dejavu, "ar_cols");
    leaf_ar_rows =
            aitown_cfg_get_leaf (sect_dejavu, "ar_rows");


    aitown_cfg_set_leaf (leaf_ar_cols, "16");
    aitown_cfg_set_leaf (leaf_ar_rows, "16");
    aitown_cfg_set_leaf (leaf_input_cols, "16");
    aitown_cfg_set_leaf (leaf_input_rows, "16");
    ret = aitown_dejavu_init (&dejavu, &core, &cfg->root);
    EXPECT_TRUE (ret == FUNC_OK);

    dejavu.chg.kb = kb_aitown_dejavu_change_kb;
    dejavu.chg.payload = img_1616;

    aitown_dejavu_feed (&dejavu, img_1616);
    aitown_dejavu_feed (&dejavu, img_1616);

    aitown_dejavu_end (&dejavu);




    aitown_cfg_set_leaf (leaf_ar_cols, "16");
    aitown_cfg_set_leaf (leaf_ar_rows, "16");
    aitown_cfg_set_leaf (leaf_input_cols, "16");
    aitown_cfg_set_leaf (leaf_input_rows, "32");
    ret = aitown_dejavu_init (&dejavu, &core, &cfg->root);
    EXPECT_TRUE (ret == FUNC_OK);

    dejavu.chg.kb = kb_aitown_dejavu_change_kb;
    dejavu.chg.payload = img_1632;

    aitown_dejavu_feed (&dejavu, img_1632);
    aitown_dejavu_feed (&dejavu, img_1632);

    aitown_dejavu_end (&dejavu);



    aitown_cfg_set_leaf (leaf_ar_cols, "16");
    aitown_cfg_set_leaf (leaf_ar_rows, "16");
    aitown_cfg_set_leaf (leaf_input_cols, "22");
    aitown_cfg_set_leaf (leaf_input_rows, "16");
    ret = aitown_dejavu_init (&dejavu, &core, &cfg->root);
    EXPECT_TRUE (ret == FUNC_OK);

    dejavu.chg.kb = kb_aitown_dejavu_change_kb;
    dejavu.chg.payload = img_2216;

    aitown_dejavu_feed (&dejavu, img_2216);
    aitown_dejavu_feed (&dejavu, img_2216);

    aitown_dejavu_end (&dejavu);


    char buf_w[16];
    char buf_h[16];
    sprintf (buf_w, "%u", img_gi1g->width);
    sprintf (buf_h, "%u", img_gi1g->height);
    aitown_cfg_set_leaf (leaf_ar_cols, "16");
    aitown_cfg_set_leaf (leaf_ar_rows, "16");
    aitown_cfg_set_leaf (leaf_input_cols, buf_w);
    aitown_cfg_set_leaf (leaf_input_rows, buf_h);
    ret = aitown_dejavu_init (&dejavu, &core, &cfg->root);
    EXPECT_TRUE (ret == FUNC_OK);

    dejavu.chg.kb = kb_aitown_dejavu_change_kb;
    dejavu.chg.payload = img_gi1g;

    aitown_dejavu_feed (&dejavu, img_gi1g);
    aitown_dejavu_feed (&dejavu, img_gi1g);

    aitown_dejavu_end (&dejavu);


    aitown_cfg_decref(cfg, cfg);

    ret = aitimage_free (&img_gi1g);
    EXPECT_TRUE (ret == FUNC_OK);

    ret = aitimage_free (&img_2216);
    EXPECT_TRUE (ret == FUNC_OK);

    ret = aitimage_free (&img_1632);
    EXPECT_TRUE (ret == FUNC_OK);

    ret = aitimage_free (&img_1616);
    EXPECT_TRUE (ret == FUNC_OK);

    aitown_core_end (&core);


}
/* ========================================================================= */

TEST(dejavu,speed) {
    aitimage_t *img_gi1g;
    double startTime, endTime;
    aitown_dejavu_t dejavu;

    const char * tmp_file = write_config_file ("test_core", CFGTEST_ALL);

    aitown_core_t core;
    aitown_core_finit (&core, tmp_file);

    gimp_image_t * gi1g = getTestImage(1);
    func_error_t ret;
    size_t sz_gi1g = gi1g->width*gi1g->height*gi1g->bytes_per_pixel;
    ret = aitimage_new (&img_gi1g,
                        gi1g->width, gi1g->height,
                        AITIMAGE_RGBA8,
                        sz_gi1g);
    EXPECT_TRUE (gi1g->bytes_per_pixel == 4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_gi1g), gi1g->pixel_data, sz_gi1g);




    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, tmp_file);
    EXPECT_TRUE (ret == FUNC_OK);

    aitown_cfg_leaf_t * leaf_ar_cols;
    aitown_cfg_leaf_t * leaf_ar_rows;
    aitown_cfg_leaf_t * leaf_input_cols;
    aitown_cfg_leaf_t * leaf_input_rows;
    aitown_cfg_sect_t * sect_dejavu;
    sect_dejavu =
            aitown_cfg_get_sect (&cfg->root, "dejavu");
    leaf_input_cols =
            aitown_cfg_get_leaf (sect_dejavu, "input_cols");
    leaf_input_rows =
            aitown_cfg_get_leaf (sect_dejavu, "input_rows");
    leaf_ar_cols =
            aitown_cfg_get_leaf (sect_dejavu, "ar_cols");
    leaf_ar_rows =
            aitown_cfg_get_leaf (sect_dejavu, "ar_rows");



    char buf_w[16];
    char buf_h[16];
    sprintf (buf_w, "%u", img_gi1g->width);
    sprintf (buf_h, "%u", img_gi1g->height);
    aitown_cfg_set_leaf (leaf_ar_cols, "16");
    aitown_cfg_set_leaf (leaf_ar_rows, "16");
    aitown_cfg_set_leaf (leaf_input_cols, buf_w);
    aitown_cfg_set_leaf (leaf_input_rows, buf_h);


    ret = aitown_dejavu_init (&dejavu, &core, &cfg->root);
    EXPECT_TRUE (ret == FUNC_OK);

    startTime = getRealTime();
    int i = 0;
    for(i=0;i<100;++i){
        aitown_dejavu_feed (&dejavu, img_gi1g);
    }
    endTime = getRealTime();

    aitown_dejavu_end (&dejavu);
    printf ("- Feed %d times took in %lf\n", i, (endTime - startTime) );



    aitown_cfg_decref(cfg, cfg);


    ret = aitimage_free (&img_gi1g);
    EXPECT_TRUE (ret == FUNC_OK);

    aitown_core_end (&core);

}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
