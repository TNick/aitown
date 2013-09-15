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
	linked_list_str_t *user_paths_first;
	
	plugin_data_t *first_plugin;	/**< first plug-in in the linked list */
	size_t plugin_count;			/**< number of loaded plug-ins */
	
	// signatures
	accumulator_t acm_sign;
	offset_t first_sign;		/**< first plug-in signature in the linked list */
	size_t sign_count;			/**< number of signatures */
	
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
/// @param plugin_manager_ the manager
/// @return FUNC_OK for success or an error code
AITOWN_EXPORT func_error_t
plugin_manager_init (plugin_manager_t** plugin_manager_);

//! terminate the manager
/// @param plugin_manager_ the manager
/// @return FUNC_OK or error code
AITOWN_EXPORT func_error_t
plugin_manager_end (plugin_manager_t** plugin_manager_);


//! add a path to the list of user paths
/// The new path is NOT scanned, so user should call plugin_manager_rescan()
/// at some point. The method does not check if the path is already in its list.
/// @param plugin_manager_ the manager
/// @param path_ the directory to add
AITOWN_EXPORT void
plugin_manager_add_user_path (
    plugin_manager_t *plugin_manager_, 
    const char * path_);

//! rescan the directories looking for plug-ins
/// @param plugin_manager_ the manager
AITOWN_EXPORT func_error_t plugin_manager_rescan(plugin_manager_t *plugin_manager_);


//! load a plug-in
/// @param plugin_manager_ the manager
/// @param plugin_ returned plug-in
/// @param signature_ the signature describing the plug-in
/// @return FUNC_OK if loaded or found, error code otherwise
AITOWN_EXPORT func_error_t
plugin_manager_load (plugin_manager_t *plugin_manager_, 
    plugin_data_t **plugin_, offset_t signature_);

//! unload a plug-in
/// @param plugin_manager_ the manager
/// @param plugin_ the plugin
AITOWN_EXPORT void
plugin_manager_unload (plugin_manager_t *plugin_manager_, 
    plugin_data_t * plugin_);


//! the callback informed about each cached plugin signature (description)
/// @param plugin_manager_ the manager
/// @param sign_ the signature
/// @param user_data_ payload passed to plugin_manager_foreach_signature()
/// @return FUNC_OK to continue, anything else to break
typedef func_error_t (*plugin_manager_foreach_signature_t) (
    plugin_manager_t *plugin_manager_, 
    plugin_sign_t *sign_,
    void *user_data_);

//! inform the callback about each cached plugin signature (description)
/// @param plugin_manager_ the manager
/// @param kb the function to call
/// @param user_data payload passed to plugin_manager_foreach_plugin()
/// @return FUNC_OK if all calls returned it, the returned error otherwise
AITOWN_EXPORT func_error_t 
plugin_manager_foreach_signature (
    plugin_manager_t *plugin_manager_, 
    plugin_manager_foreach_signature_t kb, 
    void *user_data);


//! find a signature given its name
/// @warning sign_out pointer is only valid until next accumulator allocation
/// @param plugin_manager_ the manager
/// @param name the name to search
/// @param sign_out if not NULL, receives pointer to signature
/// @return ACCUMULATOR_BAD_OFFSET if not found, offset  otherwise
AITOWN_EXPORT offset_t 
plugin_manager_find_signature (
    plugin_manager_t *plugin_manager_,
    const char *name,
    plugin_sign_t **sign_out);


//! the callback informed about each cached plugin signature (description)
/// @param plugin_manager_ the manager
/// @param plugin_ the plug-in
/// @param user_data_ payload passed to plugin_manager_foreach_plugin()
/// @return FUNC_OK to continue, anything else to break
typedef func_error_t (*plugin_manager_foreach_plugin_t) (
    plugin_manager_t *plugin_manager_, 
    plugin_data_t *plugin_, 
    void *user_data_);

//! inform the callback about each loaded plugin
/// @param plugin_manager_ the manager
/// @param kb the function to call
/// @param user_data payload passed to plugin_manager_foreach_plugin()
/// @return FUNC_OK if all calls returned it, the returned error otherwise
AITOWN_EXPORT func_error_t
plugin_manager_foreach_plugin (
	plugin_manager_t *plugin_manager_, 
    plugin_manager_foreach_plugin_t kb, 
    void *user_data);


//! the callback informed about each dependency od a plug-in signature
/// @param plugin_manager_ the manager
/// @param dep_ the dependency structure
/// @param dep_name_ the dependency name
/// @param user_data_ payload passed to plugin_manager_foreach_plugin()
/// @return FUNC_OK to continue, anything else to break
/// @warning Do NOT use accumulator allocations
typedef func_error_t (*plugin_manager_foreach_dep_t) (
    plugin_manager_t *plugin_manager_, 
    plugin_dep_t *dep_, 
    const char *dep_name_, 
    void *user_data_);

//! inform the callback about each loaded plugin
/// @param plugin_manager_ the manager where the signature belongs
/// @param sign_ the signature whose definitions are being enumerated
/// @param kb_ the function to call
/// @param user_data_ payload passed to plugin_manager_foreach_plugin()
/// @return FUNC_OK if all calls returned it, the returned error otherwise
AITOWN_EXPORT func_error_t
plugin_manager_foreach_dependency (plugin_manager_t *plugin_manager_, 
    plugin_sign_t *sign_, 
    plugin_manager_foreach_dep_t kb_, 
    void *user_data_);

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
