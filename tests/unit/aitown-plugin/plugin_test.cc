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
#include <aitown/plugin_manager.h>
#include <aitown/dir_utils.h>

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

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
static void createTempCfgFile ( const char * path_, size_t sz_path, size_t seed ) {
	
	// create the file name
	size_t sz_alloc = sz_path + 64;
	char * p_file_path = (char*)malloc(sz_alloc);
	sprintf(p_file_path, "%s" DU_PATH_SEP "aitown_plugin_%d.inf", path_, (int)seed);
	
	// open the file for writing
	FILE *file = fopen (p_file_path, "w");
	fprintf(file,
			"[general]\n"
			"name =     Gorgeous plug-in %d\n"
			"description=Example plug-in that does nothing, really\n"
			"version= 1.0.0\n"
			"manager =1.0.0\n"
			"dependencies = badabum_a:1.0.0, big-badabum_b:2.4, big-badabum_c:21, big-badabum_d\n"
			"dependencies = badabum_e :1.57. 0112, big-badabum_f : 212.0 , big-badabum_g : 4564 , big----badabum_h\n"
			"\n"
			"[aitown]\n"
			"server = yes    \n"
			"\n"
			"\n"
			"client =no\n", (int)seed
	);
	
	// clean-up
	fclose (file);
	free (p_file_path);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(plugin_testing,t1) {

	plugin_manager_t * mng;
	func_error_t err_code;
	
	// start the manager
	err_code = plugin_manager_init (&mng);
	EXPECT_EQ (err_code, FUNC_OK);
	EXPECT_TRUE (mng != NULL);
	
	// create the custom path
	size_t sz;
	char * temp_config_dir = (char*)malloc(sz+64);
	const char * crtd = du_pwd (&sz);
	sprintf (temp_config_dir, "%s" DU_PATH_SEP "%s", crtd, "test_dir_utils_002" );
	du_mkdir_p (temp_config_dir);
	
	// add our custom path
	EXPECT_TRUE (mng->user_paths_first == NULL);
	plugin_manager_add_user_path (mng, temp_config_dir);
	EXPECT_TRUE (mng->user_paths_first != NULL);
	
	// create some files
	size_t sz_path = strlen (temp_config_dir);
	size_t i; for ( i = 0; i < 100; i++ ) {
		createTempCfgFile (temp_config_dir, sz_path, i);
	}
	
	// tigger reloads
	err_code = plugin_manager_rescan (mng);
	EXPECT_EQ (err_code, FUNC_OK);	
	err_code = plugin_manager_rescan (mng);
	EXPECT_EQ (err_code, FUNC_OK);	
	err_code = plugin_manager_rescan (mng);
	EXPECT_EQ (err_code, FUNC_OK);		
	
	// stop the manager
	err_code = plugin_manager_end (&mng);
	EXPECT_EQ (err_code, FUNC_OK);
	EXPECT_TRUE (mng == NULL);
	
	// delete our temporary directory
	du_rm (temp_config_dir);
	
	// free buffer for custom path
	free (temp_config_dir);
}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
