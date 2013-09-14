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
#include <aitown/dir_utils.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef AITOWN_WIN32
#define REAL_TREE_DIR_1 "C:"
#define REAL_TREE_DIR_2 "C:\Documents and Settings"
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
TEST(dir_utils,du_pwd) {

	size_t sz;
	const char * crtd = du_pwd (&sz);
	EXPECT_TRUE(crtd != NULL);
	EXPECT_GT(sz, 0);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_utils,du_cd) {

	func_error_t err_code;
	
	err_code = du_cd (REAL_TREE_DIR_1);
	EXPECT_EQ(err_code, FUNC_OK);
	EXPECT_EQ(strcmp (du_pwd (NULL), REAL_TREE_DIR_1), 0);
	
	char * cpy = strdup (REAL_TREE_DIR_2);
	EXPECT_TRUE(cpy != NULL);
	err_code = du_cd_ptr (cpy);
	EXPECT_EQ(err_code, FUNC_OK);
	EXPECT_EQ(strcmp (du_pwd (NULL), REAL_TREE_DIR_2), 0);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_utils,du_mkdir_rmdir) {
	
	func_error_t err_code;
	size_t sz;
	const char * crtd = du_pwd (&sz);
	
	err_code = du_mkdir (crtd, "test_dir_utils_001");
	EXPECT_EQ(err_code, FUNC_OK);
	err_code = du_mkdir (crtd, "test_dir_utils_001");
	EXPECT_NE(err_code, FUNC_OK);
	
	char * one_buff = (char*)malloc(sz+64);
	EXPECT_TRUE(one_buff != NULL);
	
	sprintf(one_buff, "%s" DU_PATH_SEP "%s", crtd, "test_dir_utils_002" );
	err_code = du_mkdir_p (one_buff);
	EXPECT_EQ(err_code, FUNC_OK);
	err_code = du_mkdir_p (one_buff);
	EXPECT_NE(err_code, FUNC_OK);
	
	err_code = du_rm(one_buff);
	EXPECT_EQ(err_code, FUNC_OK);
	
	sprintf(one_buff, "%s" DU_PATH_SEP "%s", crtd, "test_dir_utils_001" );
	err_code = du_rm(one_buff);
	EXPECT_EQ(err_code, FUNC_OK);
	
	free (one_buff);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_utils,du_mkpath_rmdir) {

	func_error_t err_code;
	access_rights_t ar;
	size_t sz;
	const char * crtd = du_pwd (&sz);

	char * one_buff = (char*)malloc(sz+64);
	EXPECT_TRUE(one_buff != NULL);
	sprintf(one_buff, 
	    "%s" DU_PATH_SEP 
	    "a" DU_PATH_SEP 
	    "b" DU_PATH_SEP 
	    "c" DU_PATH_SEP 
	    "d", crtd );
	err_code = du_mkpath (one_buff);
	EXPECT_EQ(err_code, FUNC_OK);
	err_code = du_mkpath (one_buff);
	EXPECT_EQ(err_code, FUNC_OK);
	ar = du_existsdir (one_buff);
	EXPECT_NE(ar & DU_READABLE, 0);
	EXPECT_NE(ar & DU_WRITABLE, 0);
	ar = du_existsfile (one_buff);
	EXPECT_EQ(ar & DU_READABLE, 0);
	EXPECT_EQ(ar & DU_WRITABLE, 0);

	sprintf(one_buff, 
	    "%s" DU_PATH_SEP 
	    "a", crtd);
	err_code = du_rm(one_buff);
	EXPECT_EQ(err_code, FUNC_OK);

	ar = du_existsdir (one_buff);
	EXPECT_EQ(ar & DU_READABLE, 0);
	EXPECT_EQ(ar & DU_WRITABLE, 0);

	free (one_buff);
}
/* ========================================================================= */



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
