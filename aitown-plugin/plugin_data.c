/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_data.c
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "plugin_data.h"
#include <aitown/sglib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef AITOWN_WIN32
	/** @todo windows */
#else
#	include <dlfcn.h>
#endif

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

func_error_t plugin_data_new (
    plugin_data_t **plugin_, const char * name_, offset_t sign_ )
{
	plugin_data_t *ret_ptr = NULL;
	
	// compute size and alloc
	size_t name_sz = strlen (name_);
	size_t alloc_sz = sizeof(plugin_data_t) + name_sz + 1;
	ret_ptr = (plugin_data_t*)malloc (alloc_sz);
	if ( ret_ptr == NULL ) {
		return FUNC_MEMORY_ERROR;
	}
	
	// initialise header structure
	memset (ret_ptr, 0, sizeof(plugin_data_t));
	ret_ptr->sign = sign_;
	ret_ptr->name = PTR_ADD(ret_ptr, sizeof(plugin_data_t));
	
	// copy the name
	memcpy ((void*)ret_ptr->name, name_, name_sz+1);
	
	*plugin_ = ret_ptr;
	return FUNC_OK;
}

func_error_t plugin_data_delete (
    plugin_data_t **plugin_)
{
	
#	ifdef AITOWN_WIN32
	/** @todo windows */
#	else
	if ( (*plugin_)->handle != NULL ) {
		dlclose ((*plugin_)->handle);
	}
#	endif

	// release memory
	free (*plugin_);
	*plugin_ = NULL;
	return FUNC_OK;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
