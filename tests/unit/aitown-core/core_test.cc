/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/aitown-core.h>

#include <aitown/cfgpath.h>
#include <aitown/dir_utils.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define TEST_MAX_PATH   32

#ifdef AITOWN_WIN32
#define REAL_TREE_DIR_1 "C:"
#define REAL_TREE_DIR_2 getenv("TEMP")
#else
#define REAL_TREE_DIR_1 getenv("HOME")
#define REAL_TREE_DIR_2 "/tmp"
#endif

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

/* ------------------------------------------------------------------------- */
TEST(core_testing,aitown_core) {
    unsigned dims[2];

    aitown_core_t   core;
    aitown_core_init (&core);


    core_sensor_t * s0;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor_0 (&core, "a", "b", &s0));
    EXPECT_TRUE(s0->enabled);

    core_sensor_t * s0_1;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor (&core, "a", "b", 0, NULL, &s0_1));
    EXPECT_TRUE(s0_1->enabled);



    core_sensor_t * s1;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor_1 (&core, "a", "b", 5, &s1));
    EXPECT_TRUE(s1->enabled);

    core_sensor_t * s1_1;
    dims[0] = 5;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor (&core, "a", "b", 1, dims, &s1_1));
    EXPECT_TRUE(s1_1->enabled);



    core_sensor_t * s2;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor_2 (&core, "a", "b", 5, 6, &s2));

    core_sensor_t * s2_1;
    dims[0] = 5;
    dims[1] = 6;
    EXPECT_EQ(FUNC_OK, aitown_core_add_sensor (&core, "a", "b", 2, dims, &s2_1));



    core_actuator_t * a0;
    EXPECT_EQ(FUNC_OK, aitown_core_add_actuator (&core, "a", "b", 0, NULL, &a0));

    core_actuator_t * a1;
    dims[0] = 5;
    EXPECT_EQ(FUNC_OK, aitown_core_add_actuator (&core, "a", "b", 1, dims, &a1));

    core_actuator_t * a2;
    dims[0] = 5;
    dims[1] = 6;
    EXPECT_EQ(FUNC_OK, aitown_core_add_actuator (&core, "a", "b", 2, dims, &a2));



    aitown_core_rem_sensor (&core, &s0);
    aitown_core_rem_sensor (&core, &s1);
    aitown_core_rem_actuator (&core, &a0);
    aitown_core_rem_actuator (&core, &a1);

    aitown_core_disable_sensor (&core, s0_1);
    EXPECT_FALSE(s0_1->enabled);
    aitown_core_enable_sensor (&core, s0_1);
    EXPECT_TRUE(s0_1->enabled);

    aitown_core_disable_actuator (&core, a2);
    EXPECT_FALSE(a2->enabled);
    aitown_core_enable_actuator (&core, a2);
    EXPECT_TRUE(a2->enabled);


    aitown_core_end (&core);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(core_testing,brain) {
    core_brain_t   b;
    core_brain_init (&b);

    core_brain_end (&b);
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
TEST(core_testing,math) {





}
/* ========================================================================= */




/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
