/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_sign.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_sign_h_INCLUDE
#define AITOWN_plugin_sign_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/error_codes.h>
#include <stddef.h>
#include <aitown/utils_version.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! plug-in signature flags
typedef enum {
	PLUGIN_SIGN_FAIL_TO_LOAD = 0x0001,
	PLUGIN_SIGN_FOR_SERVER = 0x0002,
	PLUGIN_SIGN_FOR_CLIENT= 0x0004
	
} plugin_sign_flags_t;

//! describes a plugin signature
typedef struct _plugin_sign_t {
	const char * path_lib;
	const char * plugin_name;
	
	const char * pretty_name;
	const char * description;
	version_t my_ver;
	version_t mng_ver;
	
	plugin_sign_flags_t flags;
	// forward/backward
	struct _plugin_sign_t *previous, *next;
} plugin_sign_t;

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

//! initialize the structure
func_error_t
plugin_sign_init (plugin_sign_t *plugin_sign_);


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
#endif /* AITOWN_plugin_sign_h_INCLUDE */
