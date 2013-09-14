/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_manager.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_manager_h_INCLUDE
#define AITOWN_plugin_manager_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "plugin_data.h"
#include "plugin_sign.h"
#include <aitown/aitown_global.h>
#include <aitown/accumulator.h>
#include <aitown/error_codes.h>
#include <aitown/linked_list_str.h>
#include <aitown/utils_offset.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! manager structure for plug-ins
/// The structure hosts an accumulator_t, with all the data regarding
/// the signatures of the plug-ins that were found on the last 
/// plugin_manager_rescan().
typedef struct _plugin_manager_t {
	accumulator_t acm_sign;
	linked_list_str_t *user_paths_first;
	
	plugin_data_t *first_plugin;	/**< first plug-in in the linked list */
	offset_t first_sign;		/**< first plug-in signature in the linked list */
} plugin_manager_t;

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

//! initialize the manager
AITOWN_EXPORT func_error_t
plugin_manager_init (plugin_manager_t** plugin_manager_);

//! terminate the manager
AITOWN_EXPORT func_error_t
plugin_manager_end (plugin_manager_t** plugin_manager_);


//! add a path to the list of user paths
/// the new path is scanned, so user should not call plugin_manager_rescan()
AITOWN_EXPORT void
plugin_manager_add_user_path (
    plugin_manager_t *plugin_manager_, const char * path_);

//! rescan the directories looking for plug-ins
AITOWN_EXPORT void
plugin_manager_rescan (plugin_manager_t *plugin_manager_);


//! load a plug-in
AITOWN_EXPORT plugin_data_t *
plugin_manager_load (plugin_manager_t *plugin_manager_, const char * path_);

//! unload a plug-in
AITOWN_EXPORT void
plugin_manager_unload (plugin_manager_t *plugin_manager_, 
    plugin_data_t * plugin_);


//! the callback informed about each cached plugin signature (description)
/// @return 0 to continue, anything else to break
typedef int (*plugin_manager_foreach_signature_t) (
    plugin_manager_t *, plugin_sign_t*, void*);

//! inform the callback about each cached plugin signature (description)
AITOWN_EXPORT void
plugin_manager_foreach_signature (
    plugin_manager_t *plugin_manager_, 
    plugin_manager_foreach_signature_t kb, 
    void *user_data);


//! the callback informed about each cached plugin signature (description)
/// @return 0 to continue, anything else to break
typedef int (*plugin_manager_foreach_plugin_t) (
    plugin_manager_t *, plugin_data_t*, void*);

//! inform the callback about each loaded plugin
AITOWN_EXPORT void
plugin_manager_foreach_plugin (
	plugin_manager_t *plugin_manager_, 
    plugin_manager_foreach_plugin_t kb, 
    void *user_data);

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
#endif /* AITOWN_plugin_manager_h_INCLUDE */
