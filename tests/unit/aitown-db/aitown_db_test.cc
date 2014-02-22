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
void aitown_db_common_test (const char * driver_name, const char * cfg_file)
{
    int is_mysql = !strcmp(driver_name, MYSQL_NAME);

    aitown_db_mng_t dbmng;

    // start the system
    aitown_db_mng_init (&dbmng);

    // prepare to open and open
    aitown_db_t *db_handle;
    aitown_db_open_t open_data;
    open_data.db_mng = &dbmng;
    open_data.flags = AITOWN_DB_KEY_VARIABLE | AITOWN_DB_VALUE_VARIABLE;
    open_data.key_len = 0;
    open_data.value_len = 0;
    open_data.driver = driver_name;
    open_data.db_name = "test1";
    open_data.db_path_hint = is_mysql ? "localhost" : REAL_TREE_DIR_2;
    open_data.cfg_file = cfg_file;
    open_data.cfg = NULL;
    open_data.out = &db_handle;
    func_error_t ret_open = aitown_db_open (&open_data);
    EXPECT_TRUE (ret_open == FUNC_OK);
    EXPECT_FALSE (db_handle == NULL);

    // write something
    char test_1_key[] = "test key 1";
    unsigned test_1_keysz = strlen (test_1_key);
    char test_1_value[] = "test value 1";
    unsigned test_1_value_sz = strlen (test_1_value);
    aitown_db_io_t write_data;
    write_data.db_mng = &dbmng;
    write_data.db = db_handle;
    write_data.key.ptr = (const void*)test_1_key;
    write_data.key_sz = test_1_keysz;
    write_data.val.p = test_1_value;
    write_data.val_sz.d = test_1_value_sz;
    write_data.kb.kb_write = NULL;
    write_data.user = &open_data;
    func_error_t ret_write = aitown_db_cwrite (&write_data);
    EXPECT_TRUE (ret_write == FUNC_OK);

    // read it back
    const void * databack = NULL;
    unsigned databack_size = 0;
    aitown_db_io_t read_data;
    read_data.db_mng = &dbmng;
    read_data.db = db_handle;
    read_data.key.ptr = (const void*)test_1_key;
    read_data.key_sz = test_1_keysz;
    read_data.val.pp = &databack;
    read_data.val_sz.pd = &databack_size;
    read_data.kb.kb_read = NULL;
    read_data.user = &open_data;
    func_error_t ret_read = aitown_db_cread (&read_data);
    EXPECT_TRUE (ret_read == FUNC_OK);
    EXPECT_EQ (databack_size, test_1_value_sz);
    EXPECT_EQ (memcmp (test_1_value, databack, databack_size), 0);

    // free the chunk
    aitown_db_free (&dbmng, db_handle, &databack);
    EXPECT_TRUE (databack == NULL);

    // change the value
    char test_2_value[] = "test value 2";
    unsigned test_2_value_sz = strlen (test_1_value);
    write_data.val.p = test_2_value;
    write_data.val_sz.d = test_2_value_sz;
    ret_write = aitown_db_cwrite (&write_data);
    EXPECT_TRUE (ret_write == FUNC_OK);

    // read it back
    ret_read = aitown_db_cread (&read_data);
    EXPECT_TRUE (ret_read == FUNC_OK);
    EXPECT_EQ (databack_size, test_1_value_sz);
    EXPECT_EQ (memcmp (test_2_value, databack, databack_size), 0);

    // free the chunk
    aitown_db_free (&dbmng, db_handle, &databack);
    EXPECT_TRUE (databack == NULL);


    // close the database
    aitown_db_close (&dbmng, &db_handle);
    EXPECT_TRUE (db_handle == NULL);


    // stop the system
    aitown_db_mng_end (&dbmng);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
char * get_mysql_cfg()
{
    const char * path = REAL_TREE_DIR_2;
    size_t s_path = strlen(path) + sizeof("aitown-mysql-test.cfg") + 1;
    char * ret = (char*)malloc(s_path);
    sprintf(ret, "%s/%s", path, "aitown-mysql-test.cfg");

    if (du_existsfile(ret) == DU_NONE)  {

        /*
         *
         * One needs to open the file and add appropriate credentials
         *
         */

        FILE * f = fopen(ret, "w");
        if (f) {
                fprintf (f, "[buildin-mysql]\n");
                fprintf (f, "username = \n");
                fprintf (f, "password = \n");
        }
        fclose (f);
    }
    return ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(db,drivers) {

#   ifdef KYOTOCABINET_FOUND
    aitown_db_common_test (KYOTO_NAME, NULL);
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    aitown_db_common_test (TOKYO_NAME, NULL);
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    // TOWDO: requires creditentials
    //aitown_db_common_test (MYSQL_NAME, get_mysql_cfg());
#   endif // MYSQL_FOUND

}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
