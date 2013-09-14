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
#include <aitown/utils_offset.h>
#include <aitown/pointer_aritmetic.h>
#include "plugin_data.h"

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
	PLUGIN_SIGN_FOR_CLIENT= 0x0004,
	
	PLUGIN_IN_LOADING = 0x000100
	
} plugin_sign_flags_t;

//! describes a plugin signature
typedef struct _plugin_sign_t {
	offset_t next;
	
	offset_t path_lib;
	offset_t plugin_name;
	
	offset_t pretty_name;
	offset_t description;
	version_t my_ver;
	version_t mng_ver;
	offset_t first_dep;
	
	int flags;
	
	plugin_data_t *loaded_plugin;
} plugin_sign_t;

//! describes a dependency; is followed by the name of the dependency
typedef struct _plugin_dep_t {
	version_t my_ver;	/**< the version*/
	offset_t next;		/**< next in chain*/
	size_t str_sz;		/**< lenght of the name that follows*/
} plugin_dep_t;

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

//! get the name of a dependency
static inline char *
plugin_dep_name (plugin_dep_t *dep) {
	return (char*)PTR_ADD(dep, sizeof(plugin_dep_t));
}

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
