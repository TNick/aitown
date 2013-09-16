/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dir_utils.c
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#define DIR_UTILS_C 1
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "dir_utils.h"
#include "utils_null.h"
#include "dbg_assert.h"
#include "pointer_aritmetic.h"
#include "error_codes.h"
#include "stack_buff.h"
#include "utils_offset.h"
#include "utils_unused.h"

#ifdef AITOWN_WIN32
#	include "utils_win.h"
#else
#	include "utils_posix.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

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

static const char * crt_dir_buff = NULL;
static size_t crt_dir_sz = 0;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

const char * du_pwd (size_t *sz)
{
	if ( crt_dir_buff == NULL ) {
		crt_dir_buff = du_pwd_sysdep (&crt_dir_sz);
	}
	if ( sz != NULL )
		*sz = crt_dir_sz;
	return crt_dir_buff;
}

func_error_t du_cd (const char * new_path_)
{
	size_t sz = strlen (new_path_);
	if ( sz == 0 )
		return FUNC_GENERIC_ERROR;

	// attempt to do this for undelying architecture
	if ( chdir (new_path_) != 0 )
		return FUNC_GENERIC_ERROR;
		
	// get a copy of the string
	char * copy = (char*)malloc (sz+1);
	if (copy == NULL)
		return FUNC_MEMORY_ERROR;
	memcpy (copy,new_path_,sz+1);
	
	// free previous entry
	if ( crt_dir_buff != NULL ) {
		free ((void*)crt_dir_buff);
	}
	crt_dir_buff = copy;
	crt_dir_sz = sz;
	return FUNC_OK;
}

func_error_t du_cd_ptr (char * new_path_)
{
	size_t sz = strlen (new_path_);
	if ( sz == 0 )
		return FUNC_GENERIC_ERROR;

	// attempt to do this for undelying architecture
	if ( chdir (new_path_) != 0 )
		return FUNC_GENERIC_ERROR;
		
	// free previous entry
	if ( crt_dir_buff != NULL ) {
		free ((void*)crt_dir_buff);
	}
	crt_dir_buff = new_path_;
	crt_dir_sz = sz;
	
	return FUNC_OK;
}

func_error_t du_mkdir (const char * path_, const char * name_)
{
	func_error_t err_code;
	
	// default to pwd
	if ( path_ == NULL )
		path_ = du_pwd (NULL);
	
	// make sure base exists
	if ( ( du_existsdir(path_) & DU_WRITABLE ) == 0 ) 
		return FUNC_GENERIC_ERROR;
	
	// compute the sizes
	size_t sz_p = strlen (path_);
	size_t sz_n = strlen (name_);
	if ( sz_n == 0 )
		return FUNC_GENERIC_ERROR;
	
	// concatenate
	STACKBUFF_INIT(char, buff, 256, sz_p + 1 + sz_n + 1);
	if ( buff_ptr == NULL )
		return FUNC_MEMORY_ERROR;
	memcpy (buff_ptr, path_, sz_p);
	buff_ptr[sz_p] = '/';
	memcpy (buff_ptr+sz_p+1, name_, sz_n);
	buff_ptr[buff_actual_sz-1] = 0;
	
	// create
	err_code = du_mkdir_sysdep(buff_ptr);
	
	// free & return
	STACKBUFF_END(buff);
	return err_code;
}

func_error_t du_mkdir_p (const char * path_)
{
	return du_mkdir_sysdep(path_);
}

func_error_t du_mkpath (const char * path_)
{
	func_error_t err_code;
	STACKBUFF_INIT(char, buff, 256, strlen (path_) + 1);
	
	offset_t i;
	offset_t start_off = 0;
	size_t len;
	size_t components_count = 0;
	for ( i = 0; i < buff_actual_sz; i++ ) {
		char c = path_[i];
		if ( ( c == '/' ) || ( c == '\\' ) ) {
			len = i - start_off;
			buff_ptr[i] = 0;
			if ( len > 0 ) {
				if ( !du_existsdir(buff_ptr) ) {
					err_code = du_mkdir_p(buff_ptr);
					if ( err_code != FUNC_OK ) return err_code;
				}
			}
			components_count++;
			start_off = i + 1;
			buff_ptr[i] = DU_PATH_SEP_C;
		} else {
			buff_ptr[i] = c;
		}
	}
	
	len = i - start_off;
	buff_ptr[i] = 0;
	if ( len > 0 ) {
		if ( !du_existsdir(buff_ptr) ) {
			err_code = du_mkdir_p(buff_ptr);
			if ( err_code != FUNC_OK ) return err_code;
		}
	}
	components_count++;
	
	STACKBUFF_END(buff);
	return err_code;
}

func_error_t du_rm (const char * path_)
{
	return du_rm_sysdep(path_, 1);
}

func_error_t du_cleardir (const char * path_)
{
	return du_rm_sysdep(path_, 0);
}

access_rights_t du_existsdir (const char * path_)
{
	return du_existsdir_sysdep (path_);
}

access_rights_t du_existsfile (const char * path_)
{
	return du_existsfile_sysdep (path_);
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
