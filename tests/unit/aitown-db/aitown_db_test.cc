/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>
#include <aitown/aitown-db-free.h>

#include <aitown/dir_utils.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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
void prepare_db_run(const char * file_ini) {

    FILE * f = fopen(file_ini, "w");
    EXPECT_TRUE (f != NULL);

    fprintf (f,
             "[drivers]\n"
             "\n"
             "[drivers/" KYOTO_NAME "]\n"
             "\n"
             "[drivers/" TOKYO_NAME "]\n"
             "\n"
             "[drivers/" MYSQL_NAME "]\n"
             "\n"
             "\n"
             "[databases]\n"
             "\n"
             "[databases/test1]\n"
             "  \n"
             "  driver = " KYOTO_NAME "\n"
             "  path_hint = %s\n"
             "  key_len = \n"
             "  value_len = \n"
             , REAL_TREE_DIR_2
             );
    fclose (f);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void aitown_db_common_test (const char * driver_name, const char * cfg_file)
{
    int is_mysql = !strcmp(driver_name, MYSQL_NAME);

    aitown_db_mng_t dbmng;

    // start the system
    func_error_t ret_init = aitown_db_mng_finit (&dbmng, cfg_file);
    EXPECT_TRUE (ret_init == FUNC_OK);

    // prepare to open and open
    aitown_db_t *db_handle;
    aitown_db_open_t ovr;

    aitown_db_preopen (&ovr, &dbmng, "test1", &db_handle);
    ovr.driver = driver_name;
    ovr.path_hint = is_mysql ? "localhost" : REAL_TREE_DIR_2;
    func_error_t ret_open = aitown_db_open_ovr (&ovr);
    EXPECT_TRUE (ret_open == FUNC_OK);
    EXPECT_FALSE (db_handle == NULL);


//    aitown_db_open_t open_data;
//    open_data.db_mng = &dbmng;
//    open_data.flags = AITOWN_DB_KEY_VARIABLE | AITOWN_DB_VALUE_VARIABLE;
//    open_data.key_len = 0;
//    open_data.value_len = 0;
//    open_data.driver = driver_name;
//    open_data.db_name = "test1";
//    open_data.db_path_hint = is_mysql ? "localhost" : REAL_TREE_DIR_2;
//    open_data.cfg_file = cfg_file;
//    open_data.cfg = NULL;
//    open_data.out = &db_handle;

    // write something
    char test_1_key[] = "test key 1";
    char test_1_value[] = "test value 1";
    func_error_t ret_write = aitown_db_write (db_handle, test_1_key, test_1_value);
    EXPECT_TRUE (ret_write == FUNC_OK);


//    unsigned test_1_keysz = strlen (test_1_key);
//    unsigned test_1_value_sz = strlen (test_1_value);
//    aitown_db_io_t write_data;
//    write_data.db_mng = &dbmng;
//    write_data.db = db_handle;
//    write_data.key.ptr = (const void*)test_1_key;
//    write_data.key_sz = test_1_keysz;
//    write_data.val.p = test_1_value;
//    write_data.val_sz.d = test_1_value_sz;
//    write_data.kb.kb_write = NULL;
//    write_data.user = &open_data;


    // read it back
    const char * databack = aitown_db_read (db_handle, test_1_key);
    EXPECT_FALSE (databack == NULL);
    EXPECT_TRUE (strcmp(databack, test_1_value) == 0);


//    const void * databack = NULL;
//    unsigned databack_size = 0;
//    aitown_db_io_t read_data;
//    read_data.db_mng = &dbmng;
//    read_data.db = db_handle;
//    read_data.key.ptr = (const void*)test_1_key;
//    read_data.key_sz = test_1_keysz;
//    read_data.val.pp = &databack;
//    read_data.val_sz.pd = &databack_size;
//    read_data.kb.kb_read = NULL;
//    read_data.user = &open_data;
//    func_error_t ret_read = aitown_db_cread (&read_data);
//    EXPECT_TRUE (ret_read == FUNC_OK);
//    EXPECT_EQ (databack_size, test_1_value_sz);
//    EXPECT_EQ (memcmp (test_1_value, databack, databack_size), 0);

    // free the chunk
    aitown_db_free (db_handle, (const void**)&databack);
    EXPECT_TRUE (databack == NULL);

    // change the value
    char test_2_value[] = "test value 2";
//    unsigned test_2_value_sz = strlen (test_1_value);
//    write_data.val.p = test_2_value;
//    write_data.val_sz.d = test_2_value_sz;
//    ret_write = aitown_db_cwrite (&write_data);
//    EXPECT_TRUE (ret_write == FUNC_OK);
    ret_write = aitown_db_write (db_handle, test_1_key, test_2_value);
    EXPECT_TRUE (ret_write == FUNC_OK);


//    // read it back
//    ret_read = aitown_db_cread (&read_data);
//    EXPECT_TRUE (ret_read == FUNC_OK);
//    EXPECT_EQ (databack_size, test_1_value_sz);
//    EXPECT_EQ (memcmp (test_2_value, databack, databack_size), 0);
    databack = aitown_db_read (db_handle, test_1_key);
    EXPECT_FALSE (databack == NULL);
    EXPECT_TRUE (strcmp(databack, test_2_value) == 0);

    // free the chunk
    aitown_db_free (db_handle, (const void**)&databack);
    EXPECT_TRUE (databack == NULL);

    // close the database
    aitown_db_close (&db_handle);
    EXPECT_TRUE (db_handle == NULL);


    // stop the system
    aitown_db_mng_end (&dbmng);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
//char * get_mysql_cfg()
//{
//    const char * path = REAL_TREE_DIR_2;
//    size_t s_path = strlen(path) + sizeof("aitown-mysql-test.cfg") + 1;
//    char * ret = (char*)malloc(s_path);
//    sprintf(ret, "%s/%s", path, "aitown-mysql-test.cfg");

//    if (du_existsfile(ret) == DU_NONE)  {

//        /*
//         *
//         * One needs to open the file and add appropriate credentials
//         *
//         */

//        FILE * f = fopen(ret, "w");
//        if (f) {
//                fprintf (f, "[buildin-mysql]\n");
//                fprintf (f, "username = \n");
//                fprintf (f, "password = \n");
//        }
//        fclose (f);
//    }
//    return ret;
//}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(db,drivers) {

    char tmp_file[128];
    sprintf (tmp_file, "%s/%s", REAL_TREE_DIR_2, "test_db.ini");
    prepare_db_run (tmp_file);

#   ifdef KYOTOCABINET_FOUND
    aitown_db_common_test (KYOTO_NAME, tmp_file);
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    aitown_db_common_test (TOKYO_NAME, tmp_file);
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    // TOWDO: requires creditentials
    //aitown_db_common_test (MYSQL_NAME, tmp_file);
#   endif // MYSQL_FOUND

}
/* ========================================================================= */



/* ------------------------------------------------------------------------- */
TEST(db,aitown_db_open) {

    char tmp_file[128];
    sprintf (tmp_file, "%s/%s", REAL_TREE_DIR_2, "test_db.ini");
    prepare_db_run (tmp_file);

    aitown_db_mng_t manager;
    aitown_db_mng_finit (&manager, tmp_file);

    aitown_db_t * db_handle;
    func_error_t ret = aitown_db_open (&manager, "test1", &db_handle);
    EXPECT_TRUE (ret == FUNC_OK);


    aitown_db_close (&db_handle);


    aitown_db_mng_end (&manager);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(db,aitown_db_open_ovr) {

    char tmp_file[128];
    sprintf (tmp_file, "%s/%s", REAL_TREE_DIR_2, "test_db.ini");
    prepare_db_run (tmp_file);

    aitown_db_mng_t manager;
    aitown_db_mng_finit (&manager, tmp_file);



    aitown_db_t * db_handle;
    aitown_db_open_t ovr;

    aitown_db_preopen (&ovr, &manager, "test1", &db_handle);
    ovr.key_len = 8;
    ovr.driver = KYOTO_NAME;
    ovr.path_hint = REAL_TREE_DIR_2;
    func_error_t ret = aitown_db_open_ovr (&ovr);
    EXPECT_TRUE (ret == FUNC_OK);
    if (ret == FUNC_OK) {
        aitown_db_close (&db_handle);
    }


    aitown_db_mng_end (&manager);

}
/* ========================================================================= */



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
