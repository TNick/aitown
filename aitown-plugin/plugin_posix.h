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
		err_message ("Failed to load %s plugin: %s", dlerror());
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

//! call initialisation function
static inline func_error_t
plugin_manager_call_init (plugin_manager_t *plugin_manager_, plugin_data_t *ret_ptr_)
{
	func_error_t err_code = FUNC_OK;
	func_error_t (*fptr)(plugin_manager_t*, plugin_data_t*);
	*(void **)(&fptr) = dlsym(ret_ptr_->handle, "plugin__initialize");
	err_code = (*fptr)(plugin_manager_, ret_ptr_);
	return err_code;
}

//! call termination function
static inline void
plugin_manager_call_end (plugin_manager_t *plugin_manager_, plugin_data_t *plugin_)
{
	void (*fptr)(plugin_manager_t*, plugin_data_t*);
	*(void **)(&fptr) = dlsym(plugin_->handle, "plugin__terminate");
	(*fptr)(plugin_manager_, plugin_);
}

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
