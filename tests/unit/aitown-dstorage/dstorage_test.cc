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

#define KYOTO_NAME  "buildin-kyoto"
#define TOKYO_NAME  "buildin-tokyo"
#define MYSQL_NAME  "buildin-mysql"


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
void prepare_dstorage_run(const char * file_ini) {

    FILE * f = fopen (file_ini, "w");
    EXPECT_TRUE (f != NULL);

    fprintf (f,
             "[drivers]\n"
             "\n"
             "[drivers/" KYOTO_NAME "]\n"
             "\n"
             "\n"
             "[databases]\n"
             "\n"
             "[databases/iddatabase]\n"
             "  \n"
             "  driver = " KYOTO_NAME "\n"
             "  path_hint = %s\n"
             "  key_len = 8\n"
             "  value_len = \n"
             "[databases/ctrl1]\n"
             "  \n"
             "  driver = " KYOTO_NAME "\n"
             "  path_hint = %s\n"
             "  key_len = 8\n"
             "  value_len = \n"
             "\n"
             "\n"
             "[dstorage]\n"
             "  id_database = \"iddatabase\"\n"
             "\n"
             "[dstorage/controllers]\n"
             "  \n"
             "[dstorage/controllers/0]\n"
             "  database = \"ctrl1\"\n"
             , REAL_TREE_DIR_2, REAL_TREE_DIR_2
             );
    fclose (f);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dstorage,handle) {

    char tmp_file[128];
    sprintf (tmp_file, "%s/%s", REAL_TREE_DIR_2, "test_dstore.ini");
    prepare_dstorage_run (tmp_file);

    aitown_dstorage_t dstorage;
    aitown_dstorage_finit (&dstorage, tmp_file);





    aitown_dstorage_end (&dstorage);
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
