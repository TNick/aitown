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
#define PLUGIN_DATA_C	1
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "plugin_data.h"
#include "plugin_manager.h"
#include <aitown/sglib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aitown/utils.h>
#include <aitown/dbg_assert.h>
#ifdef AITOWN_WIN32
#	include "plugin_win.h"
#else
#	include "plugin_posix.h"
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
	// unload the binary
	plugin_manager_unload_binary (plugin_);

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
