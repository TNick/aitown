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
#include <aitown/aitown-dstorage.h>

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
TEST(dstorage_testing,handle) {

    dstorage_handle_mng_t h_mng;
    dstorage_handle_mng_init (&h_mng);

    dstorage_handle_t *h1;
    h1 = dstorage_handle_init_empty (&h_mng);
    EXPECT_FALSE(h1 == NULL);
    EXPECT_TRUE(!dstorage_handle_is_init(h1));
    dstorage_handle_end (&h_mng, &h1);
    EXPECT_TRUE(h1 == NULL);

    dstorage_id_t id = 15;
    dstorage_handle_t *h2;
    h2 = dstorage_handle_init (&h_mng, id);
    EXPECT_FALSE(h2 == NULL);
    EXPECT_TRUE(id == h2->id);
    EXPECT_TRUE(!dstorage_handle_is_resolved(h2));
    dstorage_handle_end (&h_mng, &h2);
    EXPECT_TRUE(h2 == NULL);

    dstorage_handle_mng_end (&h_mng);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dstorage_testing,lookup) {
/*
    dstorage_lookup_t lku;
    dstorage_lookup_init (&lku);

    dstorage_lookup_end (&lku);
*/
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
void all_response (
        dstorage_ctrl_sts_t sts, struct _dstorage_ctrl_param_t* req)
{

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dstorage_testing,all) {

    char gtest_path[TEST_MAX_PATH];
    gtest_path[0] = 0;
    strcat (gtest_path, REAL_TREE_DIR_2);
    strcat (gtest_path, "/gtest");
    du_mkdir (REAL_TREE_DIR_2, "gtest");

    // get temp file as config
    char cfgf[TEST_MAX_PATH];
    cfgf[0] = 0;
    strcat (cfgf, REAL_TREE_DIR_2);
    strcat (cfgf, "/gtest/main.cfg");

    // get database file for id
    char dbff[TEST_MAX_PATH];
    dbff[0] = 0;
    strcat (dbff, REAL_TREE_DIR_2);
    strcat (dbff, "/gtest/id.kch");
    du_rm (dbff);

    // the path for localdir controller
    du_mkdir (gtest_path, "localdir");

    // the database for localdb controller
    char locdbf[TEST_MAX_PATH];
    locdbf[0] = 0;
    strcat (locdbf, REAL_TREE_DIR_2);
    strcat (locdbf, "/gtest/localdb.kch");
    du_rm (locdbf);


    // write config to this file
    FILE *f_cfg_file = fopen(cfgf, "w");

    fprintf (f_cfg_file, "[database]\n");
    fprintf (f_cfg_file, "file=%s\n", dbff);
    fprintf (f_cfg_file, "\n");

    fprintf (f_cfg_file, "[chunks]\n"); // 1048576 = 1024*1024*1
    fprintf (f_cfg_file, "limit=1048576\n");
    fprintf (f_cfg_file, "\n");

    fprintf (f_cfg_file, "[controller/2]\n");
    fprintf (f_cfg_file, "name=localdir\n");
    fprintf (f_cfg_file, "path=%s/localdir\n", gtest_path);
    fprintf (f_cfg_file, "\n");

    fprintf (f_cfg_file, "[controller/1]\n");
    fprintf (f_cfg_file, "name=localdb\n");
    fprintf (f_cfg_file, "path=%s\n", locdbf);
    fprintf (f_cfg_file, "\n");

    fprintf (f_cfg_file, "[controller/3]\n");
    fprintf (f_cfg_file, "plugin=abcd\n");
    fprintf (f_cfg_file, "name=abcd\n");
    fprintf (f_cfg_file, "\n");

    fclose (f_cfg_file);

#define BUFF_CONTENT "test string"
#define BUFF_SIZE sizeof(BUFF_CONTENT)

    // now start storage based on those settings
    dstorage_t ds;
    dstorage_init (&ds, "gtest", cfgf);

    // create a new ID
    dstorage_handle_t* h1 = dstorage_new (&ds, BUFF_SIZE);
    dstorage_id_t id = h1->id;
    EXPECT_FALSE (h1 == NULL);
    EXPECT_FALSE (h1->p_data == NULL);
    EXPECT_FALSE (h1->id == 0);
    EXPECT_FALSE (h1->tstamp == 0);
    EXPECT_TRUE (dstorage_handle_is_init(h1));
    EXPECT_FALSE (dstorage_handle_is_waiting(h1));
    EXPECT_TRUE (dstorage_handle_is_resolved(h1));
    EXPECT_FALSE (dstorage_handle_temp_errors(h1));
    EXPECT_FALSE (dstorage_handle_data_lost(h1));
    EXPECT_TRUE (dstorage_handle_is_dirty(h1));
    EXPECT_TRUE (dstorage_handle_ref_count(h1) == 1);
    memcpy (dstorage_chunk_user(h1->p_data), BUFF_CONTENT, BUFF_SIZE);

    // request another handle for same ID (should be the same with increased ref count)
    dstorage_handle_t* h2 = dstorage_handle(&ds, h1->id, &ds);
    EXPECT_TRUE (h1 == h2);
    EXPECT_TRUE (dstorage_handle_ref_count(h1) == 2);

    // release both references
    dstorage_handle_done (&ds, &h1, &h1);
    EXPECT_TRUE (dstorage_handle_ref_count(h2) == 1);
    EXPECT_TRUE (h1 == NULL);
    dstorage_handle_done (&ds, &h2, &h2);
    EXPECT_TRUE (h2 == NULL);

    // get the handle back
    h1 = dstorage_handle(&ds, id, &ds);
    EXPECT_TRUE (h1->id == id);
    EXPECT_TRUE (dstorage_handle_ref_count(h1) == 1);
    EXPECT_TRUE (h1->p_data == NULL);

    // resolve the data
    dstorage_handle_resolve (&ds, h1, all_response);
    EXPECT_FALSE (h1->p_data == NULL);
    EXPECT_TRUE (memcmp(dstorage_chunk_user(h1->p_data),
                             BUFF_CONTENT, BUFF_SIZE) == 0);

    // alocate a lot of chunks, each time freeing the memory
    int i;
    for (i=0; i<1000; i++) {
        dstorage_handle_t* h3 = dstorage_new (&ds, BUFF_SIZE);
        EXPECT_FALSE (h3 == NULL);
        EXPECT_FALSE (h3->p_data == NULL);
        EXPECT_FALSE (h3->id == 0);
        EXPECT_FALSE (h3->tstamp == 0);
        EXPECT_TRUE (dstorage_handle_is_init(h3));
        EXPECT_FALSE (dstorage_handle_is_waiting(h3));
        EXPECT_TRUE (dstorage_handle_is_resolved(h3));
        EXPECT_FALSE (dstorage_handle_temp_errors(h3));
        EXPECT_FALSE (dstorage_handle_data_lost(h3));
        EXPECT_TRUE (dstorage_handle_is_dirty(h3));
        EXPECT_TRUE (dstorage_handle_ref_count(h3) == 1);
        memcpy (dstorage_chunk_user(h3->p_data), BUFF_CONTENT, BUFF_SIZE);
        dstorage_handle_done (&ds, &h3, &h3);
    }



    dstorage_end (&ds);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
dstorage_ctrl_t * dstorage_ctrl_creator_stub (
        dstorage_t * dstorage, struct_ini_sect_t *settings)
{
    return NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dstorage_testing,controllers_callback_list) {

    dstorage_controllers_init();

    EXPECT_TRUE(dstorage_controllers_find_callback ("a") == NULL);
    dstorage_controllers_add_callback (dstorage_ctrl_creator_stub, "a" );
    EXPECT_TRUE(dstorage_controllers_find_callback ("a") != NULL);
    EXPECT_TRUE(dstorage_controllers_find_callback ("b") == NULL);
    dstorage_controllers_rem_callback ( "a" );
    EXPECT_TRUE(dstorage_controllers_find_callback ("a") == NULL);
    EXPECT_TRUE(dstorage_controllers_find_callback ("b") == NULL);

    char buff[256];
    size_t i;

    for (i=0; i < 1000; i++) {
        sprintf(buff, "%u", (unsigned)i);
        dstorage_controllers_add_callback (dstorage_ctrl_creator_stub, buff );
    }

    for (i=0; i < 1000; i++) {
        sprintf(buff, "%u", (unsigned)i);
        EXPECT_TRUE(dstorage_controllers_find_callback (buff) != NULL);
    }

    for (i=0; i < 1000; i += 2) {
        sprintf(buff, "%u", (unsigned)i);
        dstorage_controllers_rem_callback (buff);
    }

    for (i=0; i < 1000; i++) {
        sprintf(buff, "%u", (unsigned)i);
        dstorage_controllers_add_callback (dstorage_ctrl_creator_stub, buff );
    }

    for (i=0; i < 1000; i++) {
        sprintf(buff, "%u", (unsigned)i);
        EXPECT_TRUE(dstorage_controllers_find_callback (buff) != NULL);
    }

    dstorage_controllers_end();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dstorage_testing, dstorage_chunk) {

    dstorage_chunk_t *ck1;
    dstorage_chunk_t *ck2;
    dstorage_chunk_t *ck3;

    // start the manager
    dstorage_chunk_mng_t mng;
    dstorage_chunk_mng_init (&mng);

    // check initial state
    EXPECT_EQ (mng.allocated, 0);
    EXPECT_EQ (mng.limit, 0);

    // can't allocate when 0
    EXPECT_TRUE(dstorage_chunk_mng_alloc (&mng, 1) == NULL);
    EXPECT_TRUE(dstorage_chunk_mng_alloc (&mng, 10) == NULL);

    // set the new limit
    dstorage_chunk_mng_set_limit (&mng, 100);
    EXPECT_EQ(mng.limit, 100);

    // get some memory
    ck1 = dstorage_chunk_mng_alloc (&mng, 1);
    EXPECT_TRUE(ck1 != NULL);
    EXPECT_EQ(mng.allocated, 1);

    ck2 = dstorage_chunk_mng_alloc (&mng, 10);
    EXPECT_TRUE(ck2 != NULL);
    EXPECT_EQ(mng.allocated, 11);

    ck3 = dstorage_chunk_mng_alloc (&mng, 99);
    EXPECT_TRUE(ck3 == NULL);
    EXPECT_EQ(mng.allocated, 11);

    // free some memory to make room
    dstorage_chunk_mng_free (&mng, &ck2);
    EXPECT_TRUE(ck2 == NULL);

    // now try again
    ck3 = dstorage_chunk_mng_alloc (&mng, 99);
    EXPECT_TRUE(ck3 != NULL);
    EXPECT_EQ(mng.allocated, 100);

    // free them all
    dstorage_chunk_mng_free (&mng, &ck1);
    EXPECT_TRUE(ck1 == NULL);
    EXPECT_EQ(mng.allocated, 99);

    dstorage_chunk_mng_free (&mng, &ck3);
    EXPECT_TRUE(ck3 == NULL);
    EXPECT_EQ(mng.allocated, 0);

    // terminate
    dstorage_chunk_mng_end (&mng);
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
