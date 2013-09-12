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

func_error plugin_manager_init (plugin_manager_t** plugin_manager_)
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
	return INDEX_OK;
}

func_error plugin_manager_delete (plugin_manager_t** plugin_manager_)
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
	return INDEX_OK;
}

void plugin_manager_add_user_path (plugin_manager_t *plugin_manager_,
	const char * path_)
{
	linked_list_str_append (&manager.user_paths_first,path_, 0);
}

static int plugin_manager_scan_a_path(const char * path, void * manager)
{
	/** @todo scan this path */
	
}

void plugin_manager_rescan (plugin_manager_t *plugin_manager_)
{
	/** @todo free previous */
	linked_list_str_foreach (
	    &manager.user_paths_first,
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
