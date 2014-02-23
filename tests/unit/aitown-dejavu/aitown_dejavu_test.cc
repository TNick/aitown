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



TEST(dejavu,image) {

    aitown_dejavu_t dejavu;
    aitimage_t *img_1616;
    aitimage_t *img_1632;
    aitimage_t *img_2216;
    func_error_t ret;

    ret = aitimage_new (&img_1616, 16, 16, AITIMAGE_RGBA8, 16*16*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_1616), test_image_16x16, 16*16*4);

    ret = aitimage_new (&img_1632, 16, 32, AITIMAGE_RGBA8, 16*32*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_1632), test_image_16x32, 16*32*4);

    ret = aitimage_new (&img_2216, 22, 16, AITIMAGE_RGBA8, 22*16*4);
    EXPECT_TRUE (ret == FUNC_OK);
    memcpy (AITIMAGE_GET_DATA(img_2216), test_image_22x16, 22*16*4);



    aitown_dejavu_init (&dejavu, 16, 16);

    aitown_dejavu_feed (&dejavu, img_1616);
    aitown_dejavu_feed (&dejavu, img_1616);

    aitown_dejavu_end (&dejavu);


    ret = aitimage_free (&img_2216);
    EXPECT_TRUE (ret == FUNC_OK);

    ret = aitimage_free (&img_1632);
    EXPECT_TRUE (ret == FUNC_OK);

    ret = aitimage_free (&img_1616);
    EXPECT_TRUE (ret == FUNC_OK);

}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
