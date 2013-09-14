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
#define SIGN_COMPARATOR(e1, e2) (e1 == e2)
SGLIB_DEFINE_DL_LIST_PROTOTYPES(plugin_sign_t,SIGN_COMPARATOR,previous,next)
SGLIB_DEFINE_DL_LIST_FUNCTIONS(plugin_sign_t,SIGN_COMPARATOR,previous,next)

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
		manager.first_sign = NULL;
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
		manager.first_sign = NULL;
		linked_list_str_delete_all (&manager.user_paths_first);
	}
	
	// get rid of the old data
	*plugin_manager_ = NULL;
	return FUNC_OK;
}

void plugin_manager_add_user_path (plugin_manager_t *plugin_manager_,
	const char * path_)
{
	linked_list_str_append (&manager.user_paths_first,path_, 0);
}

typedef struct {
	plugin_manager_t *plugin_manager;
	plugin_sign_t *sign_data;
	const char * file_path;
} plugin_manager_inf_t;

static inline func_error_t is_yes_no ( const char * value, int * out ) {
	if ( strcmp (value, "yes") ) {
		*out = 1;
	} else if ( strcmp (value, "yes") ) {
		*out = 0;
	} else {
		return FUNC_GENERIC_ERROR;
	}
	return FUNC_OK;
}

static int plugin_manager_inf(void* user_, const char* section_, const char* name_,
				   const char* value_)
{
	int ret_val = 1;
	// remember that memory is inside an accumulator, so do NOT free previous values
	plugin_manager_inf_t* data = (plugin_manager_inf_t*)user_;
	if ( strcmp (section_, "general") ) {
		if ( strcmp (name_, "name") ) {
			data->sign_data->pretty_name = accumulator_add_string( 
			    &data->plugin_manager->acm_sign, value_, 0 );
			if ( data->sign_data->pretty_name == NULL )
				ret_val = 0;
		} else if ( strcmp (name_, "description") ) {
			data->sign_data->description = accumulator_add_string( 
			    &data->plugin_manager->acm_sign, value_, 0 );
			if ( data->sign_data->description == NULL )
				ret_val = 0;
		} else if ( strcmp (name_, "version") ) {
			int ver[3];
			if ( sscanf (value_,"%d.%d.%d", &ver[0], &ver[1], &ver[2]) != 3 ) {
				ret_val = 0;
			}
			data->sign_data->my_ver = version_from_int (ver[0], ver[1], ver[2]);
		} else if ( strcmp (name_, "manager") ) {
			int ver[3];
			if ( sscanf (value_,"%d.%d.%d", &ver[0], &ver[1], &ver[2]) != 3 ) {
				ret_val = 0;
			}
			data->sign_data->mng_ver = version_from_int (ver[0], ver[1], ver[2]);
		} else if ( strcmp (name_, "dependencies") ) {
			
		} else {
			ret_val = 0;
		}
	} else if ( strcmp (section_, "aitown") ) {
		if ( strcmp (name_, "server") ) {
			int i;
			if (is_yes_no(value_, &i ) == FUNC_OK) {
				if ( i ) {
					data->sign_data->flags = (plugin_sign_flags_t) 
							data->sign_data->flags | PLUGIN_SIGN_FOR_SERVER;
				} else {
					data->sign_data->flags = (plugin_sign_flags_t) 
							data->sign_data->flags & (~PLUGIN_SIGN_FOR_SERVER);
				}
			} else {
				ret_val = 0;
			}
		} else if ( strcmp (name_, "client") ) {
			int i;
			if (is_yes_no(value_, &i ) == FUNC_OK) {
				if ( i ) {
					data->sign_data->flags = (plugin_sign_flags_t) 
							data->sign_data->flags | PLUGIN_SIGN_FOR_CLIENT;
				} else {
					data->sign_data->flags = (plugin_sign_flags_t) 
							data->sign_data->flags & (~PLUGIN_SIGN_FOR_CLIENT);
				}
			} else {
				ret_val = 0;
			}
		} else {
			ret_val = 0;
		}
	} else {
		ret_val = 0;
	}
	return ret_val;
}

static int plugin_manager_scan_an_inf ( const char * path_, const char * name_,
    void * user_data_, int is_file_)
{
	func_error_t err_code;
	DBG_ASSERT (is_file_);
	plugin_manager_t *plugin_manager = (plugin_manager_t *)user_data_;
	
	// allocate a new structure for this file and initialise it
	plugin_sign_t * new_sign = accumulator_alloc(
	    &plugin_manager->acm_sign, sizeof(plugin_sign_t) );
	if ( new_sign == NULL )
		return FUNC_MEMORY_ERROR;
	err_code = plugin_sign_init (new_sign);
	DBG_ASSERT (err_code==FUNC_OK);
	
	// allocate space for path and name; copy them
	size_t name_sz = strlen(name_);
	size_t path_sz = PTR_OFF(name_, path_) + name_sz;
	new_sign->path_lib = accumulator_add_string (
	    &plugin_manager->acm_sign, path_, path_sz );
	if ( new_sign->path_lib == NULL )
		return FUNC_MEMORY_ERROR;
	
	// allocate space for name; copy it
	size_t name_base_sz = name_sz - 4; // we know is ".inf"
	new_sign->plugin_name = accumulator_add_string (
	    &plugin_manager->acm_sign, name_, name_base_sz );
	if ( new_sign->plugin_name == NULL )
		return FUNC_MEMORY_ERROR;
	
	// read the file and interpret its content
	plugin_manager_inf_t inf_data = {plugin_manager, new_sign, path_};
	if ( ini_parse (path_, plugin_manager_inf, &inf_data) < 0 ) {
		new_sign->flags = PLUGIN_SIGN_FAIL_TO_LOAD;
	}
	
	// insert the new structure into the chain
	sglib_plugin_sign_t_add_before(
	    &plugin_manager->first_sign, new_sign);
	

	return FUNC_OK;
}
	
static int plugin_manager_scan_a_path(const char * path_, void * manager_)
{
	return dir_iterator (
	    path_,
	    "*.inf",
	    DIR_ITERATOR_EXCLUDE_DIRECTORIES, 
	    plugin_manager_scan_an_inf,
	    manager_);
}

void plugin_manager_rescan (plugin_manager_t *plugin_manager_)
{
	/** @todo free previous ? */
	linked_list_str_foreach (
	    manager.user_paths_first,
	    plugin_manager_scan_a_path,
	    plugin_manager_);
	
}

plugin_data_t * plugin_manager_load (plugin_manager_t *plugin_manager_,
	const char * path_)
{
	
}

void plugin_manager_unload (plugin_manager_t *plugin_manager_,
	plugin_data_t * plugin_)
{
	
}

void plugin_manager_foreach_signature (plugin_manager_t *plugin_manager_,
	 plugin_manager_foreach_signature_t kb, 
	void *user_data )
{
	
}

void plugin_manager_foreach_plugin (plugin_manager_t *plugin_manager_,
	plugin_manager_foreach_plugin_t kb, void *user_data)
{
	
}

/*  FUNCTIONS    =========================================================== */




/* ------------------------------------------------------------------------- */
/* ========================================================================= */
