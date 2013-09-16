/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			utils_win.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_utils_win_h_INCLUDE
#define AITOWN_utils_win_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#define WIN32_LEAN_AND_MEAN

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#include <windows.h>
#include <stdlib.h>
#include <direct.h>

#include "dir_iterator.h"
#include "dir_utils.h"
#include "char_buff.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#	define strncasecmp _strnicmp

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

#ifdef DIR_UTILS_C
//! get working directory
static inline char * 
du_pwd_sysdep (size_t *sz)
{
	char * ret_buf = NULL;
	size_t ret_sz;
	size_t ret_sz_after = 0;
	ret_sz = GetCurrentDirectory(0, NULL);
	if ( ret_sz > 0) {
		ret_sz++;
		char * tmp_buf = (char *)malloc(ret_sz);
		if ( tmp_buf != NULL ) {
			ret_sz_after = GetCurrentDirectory(ret_sz, tmp_buf);
			if ( (ret_sz_after > 0 ) && (ret_sz_after < ret_sz ) )
			{
				ret_buf = tmp_buf;
			} else {
				free (tmp_buf);
			}
		}
	}
	
	*sz = ret_sz_after;
	return ret_buf;
}

//! create directory
static inline func_error_t 
du_mkdir_sysdep (const char *path)
{
	return CreateDirectory(path, NULL) == 0 ? FUNC_GENERIC_ERROR : FUNC_OK;
}

static func_error_t kb_iter_del ( const char * path_, 
	const char * name_, void * user_data_, int is_file_)
{
	func_error_t exit_code;
	if ( is_file_ ) {
		exit_code = DeleteFile(path_) == 0 ? 
		    FUNC_GENERIC_ERROR : FUNC_OK;
	} else {
		exit_code = RemoveDirectory (path_) == 0 ? 
		    FUNC_GENERIC_ERROR : FUNC_OK;
	}
	return exit_code;
}

//! remove directory
static inline func_error_t 
du_rm_sysdep (const char * path_, int including_myself) {
	
	func_error_t exit_code = dir_iterator (
	    path_,
	    NULL,
	    DIR_ITERATOR_FILES_AND_DIRECTORIES |
	    DIR_ITERATOR_ALL_DIRECTORIES |
	    DIR_ITERATOR_ALL_FILES,
	    kb_iter_del,
	    NULL);
	   if ( exit_code == FUNC_OK ) {
		   if (including_myself) {
			   exit_code = RemoveDirectory (path_) == 0 ? 
			       FUNC_GENERIC_ERROR : FUNC_OK;
		   }
	   }
	return exit_code;
}

//! tell if directory exists
access_rights_t 
du_existsdir_sysdep (const char * path_)
{
	DWORD dwAttrib = GetFileAttributes(path_);
	if ( (dwAttrib == INVALID_FILE_ATTRIBUTES) || 
	      ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0) ) {
		return DU_NONE;
	} else {
		access_rights_t ret = DU_READABLE | DU_EXECUTABLE;
		if ((dwAttrib & FILE_ATTRIBUTE_READONLY) == 0) {
			ret = (access_rights_t)(ret | DU_WRITABLE);
		}
		return ret;
	}
}

//! tell if file exists
access_rights_t 
du_existsfile_sysdep (const char * path_)
{
	DWORD dwAttrib = GetFileAttributes(path_);
	if ( (dwAttrib == INVALID_FILE_ATTRIBUTES) || 
	      ((dwAttrib & FILE_ATTRIBUTE_NORMAL) == 0) ) {
		return DU_NONE;
	} else {
		access_rights_t ret = DU_READABLE | DU_EXECUTABLE;
		if ((dwAttrib & FILE_ATTRIBUTE_READONLY) == 0) {
			ret = (access_rights_t)(ret | DU_WRITABLE);
		}
		return ret;
	}
}
#endif // DIR_UTILS_C

#ifdef DIR_ITERATOR_C
typedef struct {
	const char *init_path;
	size_t init_path_sz;
	const char *name_filter;
	dir_iterator_flags_t flags;
	dir_iterator_foreach_t kb;
	void *user_data;
	
	char_buff_t chb;
	
	char path_separator;
} find_struct_t;


static inline int 
is_entry_directory (WIN32_FIND_DATA * fd) {
	for (;;) {
		if ( (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			break;
		if ( fd->cFileName[0] == '.' ) {
			if ( fd->cFileName[1] == 0 ) {
				break;
			} else if ( fd->cFileName[1] == '.' ) {
				if ( fd->cFileName[2] == 0 )
					break;
			}
		}
		return 1;
	}
	return 0;
}

static inline int 
is_entry_file (WIN32_FIND_DATA * fd) {
	for (;;) {
		if ( (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			break;
		if ( (fd->dwFileAttributes & FILE_ATTRIBUTE_DEVICE) )
			break;
#ifdef _DEBUG
		if ( fd->cFileName[0] == '.' ) {
			if ( fd->cFileName[1] == 0 ) {
				DBG_ASSERT(0);
				break;
			} else if ( fd->cFileName[1] == '.' ) {
				if ( fd->cFileName[2] == 0 ) {
					DBG_ASSERT(0);
					break;
				}
			}
		}
#endif
		return 1;
	}
	return 0;
}

static inline func_error_t 
dir_iterator_win (find_struct_t* fs_)
{
	int err_code = FUNC_OK;
	HANDLE h_find;
	WIN32_FIND_DATA find_data;

	// we want to return the buffer in same state as we've got it
	size_t used_data = fs_->chb.used;
	{ // don't use end_of_dir below, the buffer may be rellocated
		char *end_of_dir = fs_->chb.data + used_data;
		*end_of_dir = fs_->path_separator; end_of_dir++; fs_->chb.used++;
		*end_of_dir = 0;
	}
	
	// first, dig the childs
	if ( (fs_->flags & DIR_ITERATOR_RECURSIVE) ) {
		
		// copy the pattern at the end by trick it so that it gets overwritten
		if ( (fs_->flags & DIR_ITERATOR_ALL_DIRECTORIES) == 0 ) {
			size_t used_data_pattern = fs_->chb.used;
			err_code = char_buff_add(&fs_->chb, fs_->name_filter);
			if ( err_code != FUNC_OK ) {
				return err_code;
			}
			fs_->chb.used = used_data_pattern;
		} else {
			fs_->chb.data[fs_->chb.used] = '*';
			fs_->chb.data[fs_->chb.used+1] = 0;
		}
		
		// start the search
		h_find = FindFirstFileEx(fs_->chb.data, FindExInfoStandard, &find_data,
             FindExSearchLimitToDirectories, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
			return 0;
        }
        
        // for each directory
        do { if ( is_entry_directory (&find_data) ) {
			// append the name of the directory
			err_code = char_buff_add(&fs_->chb, find_data.cFileName);
			if ( err_code != FUNC_OK ) {
				break;
			}
			
			// inform the callback
			if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_DIRECTORIES) == 0 ) {
				err_code = fs_->kb (
				    fs_->chb.data, 
				    fs_->chb.data+used_data+1, 
				    fs_->user_data,
				    0);
				if ( err_code != FUNC_OK ) {
					break;
				}
			}

			// and call this function again
			err_code = dir_iterator_win (fs_);
			if ( err_code != FUNC_OK ) {
				break;
			}
        } } while ( FindNextFile (h_find, &find_data) );
        
        // close the search and exit if error
        FindClose (h_find);
		if ( err_code != FUNC_OK ) {
			return err_code;
		}
	}
	
	// now loop in kids
	if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_FILES) == 0 ) {
		
		// copy the pattern at the end by trick it so that it gets overwritten
		if ( (fs_->flags & DIR_ITERATOR_ALL_FILES) == 0 ) {
			size_t used_data_pattern = fs_->chb.used;
			err_code = char_buff_add(&fs_->chb, fs_->name_filter);
			if ( err_code != FUNC_OK ) {
				return err_code;
			}
			fs_->chb.used = used_data_pattern;
		} else {
			fs_->chb.data[fs_->chb.used] = '*';
			fs_->chb.data[fs_->chb.used+1] = 0;
		}
		
		// start the search
		h_find = FindFirstFileEx(fs_->chb.data, FindExInfoStandard, &find_data,
             FindExSearchNameMatch, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
			return 0;
        }
        
        // for each file
        do { if ( is_entry_file (&find_data) ) {
			// append the name of the file
			err_code = char_buff_add(&fs_->chb, find_data.cFileName);
			if ( err_code != FUNC_OK ) {
				break;
			}
			
			// inform the callback
			err_code = fs_->kb (
			    fs_->chb.data, 
				fs_->chb.data+used_data+1, 
				fs_->user_data,
				1);
			if ( err_code != FUNC_OK ) {
				break;
			}
			
        } } while ( FindNextFile (h_find, &find_data) );
        
        // close the search and exit if error
        FindClose (h_find);
		if ( err_code != FUNC_OK ) {
			return err_code;
		}
	}
	
	// restore the buffer to its former glorry
	fs_->chb.used = used_data;
	fs_->chb.data[used_data] = 0;
	
	return FUNC_OK;
}

static inline func_error_t 
dir_iterator_sysdep (const char *path_, 
    const char *name_filter_, dir_iterator_flags_t flags_,
    dir_iterator_foreach_t kb_, void *user_data_)
{
	find_struct_t	fs;
	fs.init_path = path_;
	fs.init_path_sz = strlen (path_);
	fs.name_filter = name_filter_;
	fs.kb = kb_;
	fs.flags = flags_;
	fs.user_data = user_data_;
	
	// create a buffer containing a copy of the path
	char_buff_init (&fs.chb, 1024);
	if ( fs.chb.data == NULL )
		return FUNC_MEMORY_ERROR;
	char_buff_add_string (&fs.chb, path_,fs.init_path_sz);
	fs.path_separator = DU_PATH_SEP_C;
	return dir_iterator_win(&fs);
}
#endif // DIR_ITERATOR_C

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
#endif /* AITOWN_utils_win_h_INCLUDE */
