/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_manager.c
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */




/*  INCLUDES    ------------------------------------------------------------ */

#include "plugin_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aitown/sglib.h>
#include <aitown/dir_iterator.h>
#include <aitown/dir_utils.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_version.h>
#include <aitown/ini.h>
#include <aitown/utils.h>
#include <aitown/utils_unused.h>

#ifdef AITOWN_WIN32
	/** @todo windows */
#else
#	include <dlfcn.h>
#endif

/*  INCLUDES    ============================================================ */




/*  DEFINITIONS    --------------------------------------------------------- */

// will define functions for working with double-linked list:
//	sglib_type_add
//	sglib_type_add_before
//	sglib_type_add_after
//	sglib_type_add_if_not_member
//	sglib_type_add_before_if_not_member
//	sglib_type_add_after_if_not_member
//	sglib_type_concat
//	sglib_type_delete
//	sglib_type_delete_if_member
//	sglib_type_find_member
//	sglib_type_is_member
//	sglib_type_get_first
//	sglib_type_get_last
//	sglib_type_len
//	sglib_type_sort
//	sglib_type_reverse
//	sglib_type_it_init
//	sglib_type_it_init_on_equal
//	sglib_type_it_next 
#define PLUGIN_COMPARATOR(e1, e2) (e1 == e2)
SGLIB_DEFINE_DL_LIST_PROTOTYPES(plugin_data_t,PLUGIN_COMPARATOR,previous,next)
SGLIB_DEFINE_DL_LIST_FUNCTIONS(plugin_data_t,PLUGIN_COMPARATOR,previous,next)

/*  DEFINITIONS    ========================================================= */




/*  DATA    ---------------------------------------------------------------- */

static plugin_manager_t		manager;
static int initialised = 0;

/*  DATA    ================================================================ */




/*  FUNCTIONS    ----------------------------------------------------------- */

func_error_t plugin_manager_init (plugin_manager_t** plugin_manager_)
{
	
	if ( initialised == 0 ) {
		// do initialisation
		accumulator_init (&manager.acm_sign, 256);
		manager.first_plugin = NULL;
		manager.first_sign = 0;
		manager.user_paths_first = NULL;
	}
	
	// return the singleton
	*plugin_manager_ = &manager;
	return FUNC_OK;
}

func_error_t plugin_manager_end (plugin_manager_t** plugin_manager_)
{
	if ( initialised != 0 ) {
		// do un-initialisation
		accumulator_end (&manager.acm_sign);
		manager.first_plugin = NULL;
		manager.first_sign = 0;
		linked_list_str_delete_all (&manager.user_paths_first);
	}
	
	// get rid of the old data
	*plugin_manager_ = NULL;
	return FUNC_OK;
}

void plugin_manager_add_user_path (plugin_manager_t *plugin_manager_,
	const char * path_)
{
	linked_list_str_append (&plugin_manager_->user_paths_first,path_, 0);
}

static func_error_t plugin_manager_load_dependency ( 
	plugin_manager_t *plugin_manager_, plugin_dep_t *dep_, 
	const char *dep_name_, void *user_data_)
{
	VAR_UNUSED(user_data_);
	
	// locate this dependency
	plugin_sign_t *sign_ptr;
	offset_t sign_off = plugin_manager_find_signature (
	    plugin_manager_, dep_name_, &sign_ptr);
	if (sign_off == ACCUMULATOR_BAD_OFFSET) {
		err_message ("Dependency %s could not be found", dep_name_);
		return FUNC_GENERIC_ERROR;
	}
	
	// see if is not too old
	if (dep_->my_ver > sign_ptr->my_ver) {
		int ver_parts[6];
		version_to_int_array (sign_ptr->my_ver, &ver_parts[0]);
		version_to_int_array (dep_->my_ver, &ver_parts[3]);
		err_message (
		    "Dependency %s version %d.%d.%d present "
		    "but %d.%d.%d is required", 
		    dep_name_,
		    ver_parts[0], ver_parts[1], ver_parts[2], 
		    ver_parts[3], ver_parts[4], ver_parts[5]);
		return FUNC_GENERIC_ERROR;
	}
	
	// maybe already in process of loading?
	if ( sign_ptr->flags & PLUGIN_IN_LOADING ) {
		return FUNC_OK;
	}
	
	// attempt to load it; if already loaded will return FUNC_OK
	plugin_data_t * plg_in;
	return plugin_manager_load (plugin_manager_, &plg_in, sign_off);
}

func_error_t plugin_manager_load (plugin_manager_t *plugin_manager_,
	plugin_data_t ** plugin_, offset_t signature_)
{
	func_error_t err_code = FUNC_OK;
	plugin_data_t *ret_ptr = NULL;

	// get actual signature and other pointers
	plugin_sign_t * crt_sign = accumulator_to_ptr (
	    &plugin_manager_->acm_sign, signature_);
	const char * plugin_path = accumulator_to_char(
	    &plugin_manager_->acm_sign, crt_sign->path_lib);
	const char * plugin_name = accumulator_to_char(
	    &plugin_manager_->acm_sign, crt_sign->plugin_name);

	// see if already loaded; if so, return the instance
	if ( crt_sign->loaded_plugin != NULL ) {
		*plugin_ = crt_sign->loaded_plugin;
		return FUNC_OK;
	}
	
	// should not be in the process of loading
	DBG_ASSERT ( (crt_sign->flags & PLUGIN_IN_LOADING) == 0 );
	
	// see what other plug-ins should be loaded
	err_code = plugin_manager_foreach_dependency (
	    plugin_manager_, crt_sign, 
	    plugin_manager_load_dependency, 
	    NULL);
	if ( err_code != FUNC_OK ) {
		err_message("Loading of plugin %s failed due to missing dependencies", 
		    plugin_name);
		return FUNC_GENERIC_ERROR;
	}
	
	// allocate a new structure and initialise it
	if (plugin_data_new (&ret_ptr, plugin_name, signature_) != FUNC_OK) {
		return FUNC_MEMORY_ERROR;
	}
	
	// actual loading
#	ifdef AITOWN_WIN32
	/** @todo windows */
#	else
	ret_ptr->handle = dlopen (plugin_path, RTLD_LAZY | RTLD_GLOBAL);
	if ( ret_ptr->handle == NULL ) {
		err_message ("Failed to load %s plugin: %s", dlerror());
		plugin_data_delete (&ret_ptr);
		return FUNC_GENERIC_ERROR;
	}
#	endif
	
	// insert the plugin into the chain
	crt_sign->flags = crt_sign->flags | PLUGIN_IN_LOADING;
	sglib_plugin_data_t_add_before (&plugin_manager_->first_plugin, ret_ptr);
	crt_sign->loaded_plugin = ret_ptr;
	crt_sign->flags = crt_sign->flags & (~PLUGIN_IN_LOADING);
	
	// call initialisation function	
#	ifdef AITOWN_WIN32
	/** @todo windows */
	err_code = FUNC_OK;
#	else
	func_error_t (*fptr)(plugin_manager_t*, plugin_data_t*);
	*(void **)(&fptr) = dlsym(ret_ptr->handle, "plugin__initialize");
	err_code = (*fptr)(plugin_manager_, ret_ptr);
#	endif
	
	// unload if it refused to start
	if ( err_code != FUNC_OK ) {
		plugin_manager_unload (plugin_manager_, ret_ptr);
		err_code = FUNC_GENERIC_ERROR;
	}
	
	// and a happy exit to everyone
	*plugin_ = ret_ptr;
	return err_code;
}

void plugin_manager_unload (plugin_manager_t *plugin_manager_,
	plugin_data_t * plugin_)
{
	
	// call termination function	
#	ifdef AITOWN_WIN32
	/** @todo windows */
	err_code = FUNC_OK;
#	else
	void (*fptr)(plugin_manager_t*, plugin_data_t*);
	*(void **)(&fptr) = dlsym(plugin_->handle, "plugin__initialize");
	(*fptr)(plugin_manager_, plugin_);
#	endif
	
	// remove it from the chain and release the resources
	if ( plugin_->sign != ACCUMULATOR_BAD_OFFSET ) {
		plugin_sign_t * assoc_sign = (plugin_sign_t*)accumulator_to_ptr (
		    &plugin_manager_->acm_sign, plugin_->sign);
		assoc_sign->loaded_plugin = NULL;
	}
	sglib_plugin_data_t_delete (&plugin_manager_->first_plugin, plugin_);
	plugin_data_delete (&plugin_);
	
}

func_error_t plugin_manager_foreach_signature (plugin_manager_t *plugin_manager_,
	 plugin_manager_foreach_signature_t kb, void *user_data )
{
	// prepare to iterate
	func_error_t err_code = FUNC_OK;
	offset_t crt_off;
	offset_t next_off;
	crt_off = plugin_manager_->first_sign;
	while ( crt_off != ACCUMULATOR_BAD_OFFSET ) {
		
		// get the signature
		plugin_sign_t * crt_sign = accumulator_to_ptr (
		    &plugin_manager_->acm_sign, crt_off);
		next_off = crt_sign->next;
		
		// call callback
		err_code = kb (plugin_manager_, crt_sign, user_data);
		if ( err_code != FUNC_OK )
			break;
		
		// step to next one
		crt_off = next_off;
	}
	return err_code;
}

offset_t plugin_manager_find_signature ( plugin_manager_t *plugin_manager_,
    const char * name, plugin_sign_t ** sign_out )
{
	// prepare to iterate
	offset_t crt_off;
	crt_off = plugin_manager_->first_sign;
	while ( crt_off != ACCUMULATOR_BAD_OFFSET ) {
		
		// get the signature and its name
		plugin_sign_t *crt_sign = accumulator_to_ptr (
		    &plugin_manager_->acm_sign, crt_off);
		const char * my_name = accumulator_to_char( 
		    &plugin_manager_->acm_sign, crt_sign->plugin_name);
		
		// compare the names
		if ( strcasecmp(my_name, name) == 0 ) {
			if (sign_out != NULL) {
				*sign_out = crt_sign;
			}
			return crt_off;
		}
		
		// step to next one
		crt_off = crt_sign->next;
	}
	return ACCUMULATOR_BAD_OFFSET;
}

func_error_t plugin_manager_foreach_plugin (plugin_manager_t *plugin_manager_,
	plugin_manager_foreach_plugin_t kb_, void *user_data_)
{
	plugin_data_t *itr_crt;
	struct sglib_plugin_data_t_iterator itr;
	
	itr_crt = sglib_plugin_data_t_it_init (&itr, plugin_manager_->first_plugin);
	while (itr_crt!=NULL) {
		func_error_t i = kb_ (plugin_manager_, itr_crt, user_data_);
		if ( i != FUNC_OK )
			return i;
		itr_crt = sglib_plugin_data_t_it_next(&itr);
	}
	return FUNC_OK;
}

func_error_t plugin_manager_foreach_dependency ( 
	plugin_manager_t *plugin_manager_, plugin_sign_t *sign_, 
	plugin_manager_foreach_dep_t kb_, void *user_data_)
{
	func_error_t err_code;
	offset_t crt_off = sign_->first_dep;
	plugin_dep_t *crt_dep_ptr;
	const char * dep_name;
	while ( crt_off != ACCUMULATOR_BAD_OFFSET ) {
		crt_dep_ptr = accumulator_to_ptr (
		    &plugin_manager_->acm_sign, crt_off);
		dep_name = plugin_dep_name (crt_dep_ptr);
		
		err_code = kb_ (plugin_manager_, crt_dep_ptr, dep_name, user_data_);
		
		crt_off = crt_dep_ptr->next;
	}
	return err_code;
}

/*  FUNCTIONS    =========================================================== */




/* ------------------------------------------------------------------------- */
/* ========================================================================= */
