/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_posix.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_posix_h_INCLUDE
#define AITOWN_plugin_posix_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "plugin_definition.h"
#include <aitown/utils_unused.h>

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
	ret_ptr_->handle = dlopen (path_, RTLD_LAZY | RTLD_GLOBAL);
	if ( ret_ptr_->handle == NULL ) {
		err_message ("Failed to load %s plugin: %s", path_, dlerror());
		return FUNC_GENERIC_ERROR;
	}
	return FUNC_OK;
}
#endif

#ifdef PLUGIN_DATA_C
//! unload the binary
static void
plugin_manager_unload_binary (plugin_data_t **plugin_)
{
	if ( (*plugin_)->handle != NULL ) {
		dlclose ((*plugin_)->handle);
	}
}
#endif

#ifdef PLUGIN_MANAGER_C
//! call initialisation function
static inline func_error_t
plugin_manager_call_init (plugin_manager_t *plugin_manager_, plugin_data_t *plugin_)
{
	func_error_t err_code = FUNC_OK;
	plugin__initialize_t fptr;
	*(void **)(&fptr) = dlsym(plugin_->handle, "plugin__initialize");
	if ( fptr == NULL ) {
		err_message (
		    "Failed to load %s plugin: "
		    "plugin__initialize() method was not found.", 
		    plugin_->name);
		return FUNC_GENERIC_ERROR;
	}
	err_code = (*fptr)(plugin_manager_, plugin_);
	return err_code;
}

//! call termination function
static inline void
plugin_manager_call_end (plugin_manager_t *plugin_manager_, plugin_data_t *plugin_)
{
	plugin__terminate_t fptr;
	*(void **)(&fptr) = dlsym(plugin_->handle, "plugin__terminate");
	(*fptr)(plugin_manager_, plugin_);
}
#endif // PLUGIN_MANAGER_C

#ifdef PLUGIN_MANAGER_INF_C
//! extension for plug-in binary (max 3 characters)
static inline void
plugin_manager_shared_binary_extension (char * out_buff)
{
	out_buff[0] = 's'; out_buff[1] = 'o'; out_buff[2] = 0;
}

//! get the path of the executable
static inline char *
plugin_manager_program_path ()
{
	char *linkname;
	struct stat sb;
	ssize_t r;
	size_t alloc_mem;
	
	// get the symbolic link to the executable
	char sym_path[64];
	sprintf (sym_path, "/proc/%d/exe", getpid());
	
	// get its status
	int res = lstat(sym_path, &sb);
	DBG_ASSERT (res != -1);
    VAR_UNUSED (res);
	alloc_mem = sb.st_size;
	
	if ( alloc_mem == 0 ) {
		
		alloc_mem = 256;
		int i; for ( i = 0; i < 10; i++ ) {
			
			// get a buffer of an arbitrary size
			linkname = (char*)malloc (alloc_mem);
			if (linkname == NULL) {
				return NULL;
			}
			
			// attempt a read
			r = readlink(sym_path, linkname, alloc_mem);
			if ( r <  0 ) {
				free (linkname);
				return NULL;
			}
			
			// if ok, return it; don't shrink it, since it is short lived
			if (r < alloc_mem) {
				linkname[r] = 0;
				break;
			}
			
			// attempt a larger buffer
			free (linkname);
			alloc_mem += 256;
			linkname = NULL;
		}
	
	} else {
	
		// allocate buffer for real path
		linkname = malloc(alloc_mem + 1);
		if (linkname == NULL) {
			return NULL;
		}
		
		// attempt to read it
		r = readlink(sym_path, linkname, alloc_mem + 1);
		if ( ( r <  0 ) || (r > alloc_mem) ) {
			free (linkname);
			return NULL;
		}
		linkname[alloc_mem] = 0;
	}
	
	return linkname;
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
#endif /* AITOWN_plugin_posix_h_INCLUDE */
