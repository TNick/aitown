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

#include <string.h>
#include <stdlib.h>

#ifdef AITOWN_WIN32
#	include <>
#else
#	include <unistd.h>
#	include <errno.h>
#	include <sys/stat.h>
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
#ifdef AITOWN_WIN32
		/** @todo */
#else
		// allocate an initial buffer
		int storage_sz = 256;
		char * tmp_buf = (char *)malloc(storage_sz);
		if ( tmp_buf == NULL )
			return NULL;
		
		// but be ready to enlarge it
		int i = 0;
		for ( ;; ) {
			if ( getcwd (tmp_buf, storage_sz) == tmp_buf ) {
				// probably too large, so shrink it down and set it
				crt_dir_sz = strlen(tmp_buf);
				char * tmp_buf2 = (char*)realloc(tmp_buf, crt_dir_sz+1);
				if ( tmp_buf2 == NULL )
					break;
				crt_dir_buff = tmp_buf2;
				tmp_buf = NULL;
				break;
			}
			// attempt to enlarge the buffer if that's the issue
			i++; if ( i > 5 ) break;
			if ( ( errno == ENOMEM ) || ( errno == ERANGE ) ) {
				char * tmp_buf2;
				storage_sz += 256;
				tmp_buf2 = (char*)realloc(tmp_buf, storage_sz);
				if ( tmp_buf2 == NULL )
					break;
				tmp_buf = tmp_buf2;
			} else {
				break;
			}
		} // for ( ;; )
		if ( tmp_buf != NULL ) {
			free (tmp_buf);
		}
#endif
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
	
#ifdef AITOWN_WIN32
		/** @todo */
#else
	// read/write/search permissions for owner and group, 
	// and with read/search permissions for others.
	int sts = mkdir (buff_ptr, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	err_code = (sts == 0 ? FUNC_OK : FUNC_GENERIC_ERROR);
#endif
	STACKBUFF_END(buff);
	return err_code;
}

func_error_t du_mkdir_p (const char * path_)
{
	func_error_t err_code;

#ifdef AITOWN_WIN32
		/** @todo */
#else
	// read/write/search permissions for owner and group, 
	// and with read/search permissions for others.
	int sts = mkdir (path_, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	err_code = (sts == 0 ? FUNC_OK : FUNC_GENERIC_ERROR);
#endif

	return err_code;
}

func_error_t du_mkpath (const char * path_)
{
	func_error_t err_code;
	
	return err_code;
}

func_error_t du_rm (const char * path_)
{
	func_error_t err_code;
	
	return err_code;
}

func_error_t du_cleardir (const char * path_)
{
	func_error_t err_code;
	
	return err_code;
}


access_rights_t du_existsdir (const char * path_)
{
#ifdef AITOWN_WIN32
		/** @todo */
#else
	struct stat sb;
	int ret = DU_NONE;
	
	if ( stat(path_, &sb) == 0 && S_ISDIR(sb.st_mode) )
	{
		if ( sb.st_mode & S_IRUSR )
			ret = ret | DU_READABLE;
		if ( sb.st_mode & S_IWUSR )
			ret = ret | DU_WRITABLE;
		if ( sb.st_mode & S_IXUSR )
			ret = ret | DU_EXECUTABLE;
	}
	return (access_rights_t)ret;
#endif	
}

access_rights_t du_existsfile (const char * path_)
{
#ifdef AITOWN_WIN32
		/** @todo */
#else
	struct stat sb;
	int ret = DU_NONE;
	
	if ( stat(path_, &sb) == 0 && S_ISREG(sb.st_mode) )
	{
		if ( sb.st_mode & S_IRUSR )
			ret = ret | DU_READABLE;
		if ( sb.st_mode & S_IWUSR )
			ret = ret | DU_WRITABLE;
		if ( sb.st_mode & S_IXUSR )
			ret = ret | DU_EXECUTABLE;
	}
	return (access_rights_t)ret;
#endif	
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
