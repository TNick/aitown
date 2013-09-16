/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_win.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_win_h_INCLUDE
#define AITOWN_plugin_win_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <windows.h>
#include "plugin_definition.h"

#ifdef __cplusplus
extern "C" {
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

#ifdef PLUGIN_MANAGER_C
//! load a shared library
static inline func_error_t
plugin_manager_load_binary (plugin_data_t *ret_ptr_, const char * path_)
{
	HMODULE hm = LoadLibrary (path_);
	if ( hm == NULL ) {
		err_message ("Failed to load %s plugin, error: %s", path_, GetLastError() );
		return FUNC_GENERIC_ERROR;
	}
	ret_ptr_->handle = hm;
	return FUNC_OK;
}
#endif

#ifdef PLUGIN_DATA_C
//! unload the binary
static void
plugin_manager_unload_binary (plugin_data_t **plugin_)
{
	if ( (*plugin_)->handle != NULL ) {
		FreeLibrary ((HMODULE)(*plugin_)->handle);
	}
}
#endif

#ifdef PLUGIN_MANAGER_C
//! call initialisation function
static inline func_error_t
plugin_manager_call_init (
    plugin_manager_t *plugin_manager_, plugin_data_t *plugin_)
{
	plugin__initialize_t fp = (plugin__initialize_t)GetProcAddress (
	    (HMODULE)plugin_->handle, "plugin__initialize");
	if ( fp == NULL ) {
		err_message (
		    "Failed to load %s plugin: "
		    "plugin__initialize() method was not found.", 
		    plugin_->name);
		return FUNC_GENERIC_ERROR;
	}
	return fp (plugin_manager_, plugin_);
}
//! call termination function
static inline void
plugin_manager_call_end (
    plugin_manager_t *plugin_manager_, plugin_data_t *plugin_)
{
	plugin__terminate_t fp = (plugin__terminate_t)GetProcAddress (
	    (HMODULE)plugin_->handle, "plugin__terminate");
	if ( fp == NULL ) {
		err_message (
		    "Failed to properly unload %s plugin: "
		    "plugin__terminate() method was not found.", 
		    plugin_->name);
		return;
	}
	fp (plugin_manager_, plugin_);
}
#endif // PLUGIN_MANAGER_C

#ifdef PLUGIN_MANAGER_INF_C
//! extension for plug-in binary (max 3 characters)
static inline void
plugin_manager_shared_binary_extension (char * out_buff)
{
	out_buff[0] = 'd'; out_buff[1] = 'l'; out_buff[2] = 'l';
}

//! get the path of the executable
static inline char *
plugin_manager_program_path ()
{
	int i;
	int buff_sz = 256;
	int result;
	char * buff;
	for ( i = 0; i < 10; i++ ) {
		buff = (char*)malloc(buff_sz);
		result = GetModuleFileName (NULL, buff, buff_sz);
		if ( result == 0 ) {
			dbg_message ("Failed to retrieve process directory; error code: %d", GetLastError());
			free (buff);
			return NULL;
		}
		if ( result != buff_sz ) {
			break;
		}
		free (buff);
		buff_sz += 256;
		buff = NULL;
	}
	// no need to shrink it, the buffer is short lived anyway
	return buff;
}
#endif // PLUGIN_MANAGER_INF_C

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
#endif /* AITOWN_plugin_win_h_INCLUDE */
