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
#include <aitown/dir_iterator.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef AITOWN_WIN32
#define REAL_TREE_DIR "C:"
#else
#define REAL_TREE_DIR getenv("HOME")
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

typedef struct {
	size_t	files;
	size_t	dirs;
} kb_iter_1_t;

int kb_iter_1 ( const char * path_, const char * name_, void * user_data_, int is_file_)
{
	EXPECT_TRUE(path_ != NULL);
	EXPECT_TRUE(name_ != NULL);
	EXPECT_TRUE(user_data_ != NULL);
	EXPECT_TRUE(name_-path_ > 0);
	
	kb_iter_1_t *data = (kb_iter_1_t *)user_data_;
	if (is_file_) {
		data->files++;
	} else {
		data->dirs++;
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_simple) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "*",
	    DIR_ITERATOR_FILES_AND_DIRECTORIES,
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_recursive) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "*",
	    (dir_iterator_flags_t)(DIR_ITERATOR_FILES_AND_DIRECTORIES | DIR_ITERATOR_RECURSIVE),
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_recursive_no_dirs) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "*",
	    (dir_iterator_flags_t)(DIR_ITERATOR_EXCLUDE_DIRECTORIES | DIR_ITERATOR_RECURSIVE),
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_recursive_no_files) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "*",
	    (dir_iterator_flags_t)(DIR_ITERATOR_EXCLUDE_FILES | DIR_ITERATOR_RECURSIVE),
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_all_dirs) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "abcd123456789",
	    DIR_ITERATOR_ALL_DIRECTORIES,
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(dir_iterator,iter_all_files) {

	kb_iter_1_t data = {0,0};
	int exit_code = dir_iterator (
	    REAL_TREE_DIR,
	    "abcd123456789",
	    DIR_ITERATOR_ALL_FILES,
	    kb_iter_1,
	    &data);
	EXPECT_EQ(exit_code, FUNC_OK);
	
}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
