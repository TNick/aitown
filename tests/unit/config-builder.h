/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			config-builder.h
  \date			September 2013
  \author		TNick

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_config_builder_h_INCLUDE
#define AITOWN_config_builder_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! how are we goiing to build the file
typedef enum {

    CFGTEST_DRIVERS = 0x000001,
    CFGTEST_DRIVERS_KYOTO = CFGTEST_DRIVERS | 0x000002,
    CFGTEST_DRIVERS_TOKYO = CFGTEST_DRIVERS | 0x000004,
    CFGTEST_DRIVERS_MYSQL = CFGTEST_DRIVERS | 0x000008,
    CFGTEST_DATABASES = 0x000010,
    CFGTEST_DATABASE_1 = CFGTEST_DATABASES | 0x000020 | CFGTEST_DRIVERS_KYOTO,
    CFGTEST_DATABASE_2 = CFGTEST_DATABASES | 0x000040 | CFGTEST_DRIVERS_TOKYO,
    CFGTEST_DATABASE_ID = CFGTEST_DATABASES | 0x000080 | CFGTEST_DRIVERS_KYOTO,
    CFGTEST_DSTORAGE = 0x000100,
    CFGTEST_DSTORAGE_CTRL = CFGTEST_DSTORAGE | 0x000200,
    CFGTEST_DSTORAGE_CTRL_1 = CFGTEST_DSTORAGE_CTRL | 0x000400,
    CFGTEST_DSTORAGE_CTRL_2 = CFGTEST_DSTORAGE_CTRL | 0x000800,
    CFGTEST_DSTORAGE_HANDLES = CFGTEST_DSTORAGE | 0x001000 | CFGTEST_DATABASE_ID,

    CFGTEST_ALL = 0xFFFFFFFF
} config_file_flags_t;


#define KYOTO_NAME  "buildin-kyoto"
#define TOKYO_NAME  "buildin-tokyo"
#define MYSQL_NAME  "buildin-mysql"


#ifdef AITOWN_WIN32
#define REAL_TREE_DIR_1 "C:"
#define OS_TEMPORARY_DIR getenv("TEMP")
#else
#define REAL_TREE_DIR_1 getenv("HOME")
#define OS_TEMPORARY_DIR "/tmp"
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

const char * write_config_file(
        const char * name,
        int flags);

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
#endif /* AITOWN_config_builder_h_INCLUDE */
