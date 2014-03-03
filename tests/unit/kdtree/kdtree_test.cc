/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include <aitown/aitown-kdtree.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef AITOWN_WIN32
#define REAL_TREE_DIR_1 "C:"
#define OS_TEMPORARY_DIR getenv("TEMP")
#else
#define REAL_TREE_DIR_1 getenv("HOME")
#define OS_TEMPORARY_DIR "/tmp"
#endif


KDTREE_STATIC_DEFINE_PROTOTYPES(3Di, int, 3, 0)
KDTREE_STATIC_DEFINE_PROTOTYPES(3Dd, double, 3, 0)


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

KDTREE_STATIC_DEFINE_FUNCTIONS(3Di, int, 3, 0)
KDTREE_STATIC_DEFINE_FUNCTIONS(3Dd, double, 3, 0)


/* ------------------------------------------------------------------------- */
TEST(kdtree,static_3Di) {

    /* create a kd-tree for "k"-dimensional data */
    kdtree_3Di_t * kdt = kdtree_3Di_create();


    /* insert a node, specifying its position, and optional data */
    int pos1[3] = {1, 2, 3};
    kdtree_3Di_insert (kdt, pos1, pos1);


    /* Find the nearest node from a given point */ \
    int pos2[3] = {-1, -2, -3};
    kdres_3Di * res_pos2 = kdtree_3Di_nearest (kdt, pos2);

    EXPECT_EQ(kdtree_3Di_res_size(res_pos2), 1);
    EXPECT_EQ(kdtree_3Di_res_end(res_pos2), 0);
    EXPECT_TRUE((int*)kdtree_3Di_res_item(res_pos2, NULL) == pos1);
    EXPECT_TRUE((int*)kdtree_3Di_res_item_data(res_pos2) == pos1);
    kdtree_3Di_res_rewind(res_pos2);
    EXPECT_EQ(kdtree_3Di_res_next(res_pos2), 0);

    kdtree_3Di_res_free (res_pos2);


    /* Find any nearest nodes from a given point within a range */
    int pos3[3] = {-1, -2, -3};
    kdres_3Di * res_pos3 = kdtree_3Di_nearest_range (kdt, pos3, 1);

    EXPECT_EQ(kdtree_3Di_res_size(res_pos3), 0);
    EXPECT_NE(kdtree_3Di_res_end(res_pos3), 0);
    //kdtree_3Di_res_rewind(res_pos3);
    //EXPECT_EQ(kdtree_3Di_res_next(res_pos3), 0);

    kdtree_3Di_res_free (res_pos3);


    /* remove all the elements from the tree */
    kdtree_3Di_clear (kdt);

    /* free the struct */
    kdtree_3Di_free (kdt);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(kdtree,static_3d_double) {

    /* create a kd-tree for "k"-dimensional data */
    kdtree_3Dd_t * kdt = kdtree_3Dd_create();


    /* insert a node, specifying its position, and optional data */
    double pos1[3] = {1.1, 2.1, 3.1};
    kdtree_3Dd_insert (kdt, pos1, pos1);


    /* Find the nearest node from a given point */ \
    double pos2[3] = {-1.6, -2.8, -3.0};
    kdres_3Dd * res_pos2 = kdtree_3Dd_nearest (kdt, pos2);

    EXPECT_EQ(kdtree_3Dd_res_size(res_pos2), 1);
    EXPECT_EQ(kdtree_3Dd_res_end(res_pos2), 0);
    EXPECT_TRUE((double*)kdtree_3Dd_res_item(res_pos2, NULL) == pos1);
    EXPECT_TRUE((double*)kdtree_3Dd_res_item_data(res_pos2) == pos1);
    kdtree_3Dd_res_rewind(res_pos2);
    EXPECT_EQ(kdtree_3Dd_res_next(res_pos2), 0);

    kdtree_3Dd_res_free (res_pos2);


    /* Find any nearest nodes from a given point within a range */
    double pos3[3] = {-1, -2, -3};
    kdres_3Dd * res_pos3 = kdtree_3Dd_nearest_range (kdt, pos3, 1);

    EXPECT_EQ(kdtree_3Dd_res_size(res_pos3), 0);
    EXPECT_NE(kdtree_3Dd_res_end(res_pos3), 0);
    kdtree_3Dd_res_rewind(res_pos3);
    //EXPECT_EQ(kdtree_3Dd_res_next(res_pos3), 0);

    kdtree_3Dd_res_free (res_pos3);


    /* remove all the elements from the tree */
    kdtree_3Dd_clear (kdt);

    /* free the struct */
    kdtree_3Dd_free (kdt);

}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
