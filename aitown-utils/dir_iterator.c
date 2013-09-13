/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dir_iterator.c
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

#include "dir_iterator.h"

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>

#ifdef AITOWN_WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	pragma warning (disable : 4996)
#else
#	include <dirent.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#	include <regex.h>
#	include <errno.h>
#	include <err.h>
#endif

#ifdef AITOWN_WIN32
#	define strncasecmp _strnicmp
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

typedef struct {
#ifdef AITOWN_WIN32
#else
	regex_t rex;
#endif
	const char *init_path;
	size_t init_path_sz;
	const char *name_filter;
	dir_iterator_flags_t flags;
	dir_iterator_foreach_t kb;
	void *user_data;
	
	char *path_buffer;
	size_t allocated_buffer;
	size_t used_buffer;
	
	char path_separator;
} find_struct_t;

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

static int append_to_buffer ( find_struct_t* fs_, const char * p) {
	size_t len = strlen (p);
	char * tmp_buf;
	size_t new_used = fs_->used_buffer + len;
	
	// enlarge the buffer if required
	if ( new_used+8 >= fs_->allocated_buffer ) {
		size_t new_alloc = new_used + 128 + (sizeof(void*) - (new_used % sizeof(void*)));
		tmp_buf = (char*)realloc(fs_->path_buffer, new_alloc);
		if ( tmp_buf == NULL )
			return FUNC_MEMORY_ERROR;
		fs_->path_buffer = tmp_buf;
		fs_->allocated_buffer = new_alloc;
	}
	
	// append the string
	char * dest_buf = fs_->path_buffer + fs_->used_buffer;
	memcpy (dest_buf, p, len);
	dest_buf[len] = 0;
	fs_->used_buffer = new_used;
	
	return FUNC_OK;
}

#ifdef AITOWN_WIN32

static inline int is_entry_directory (WIN32_FIND_DATA * fd) {
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

static inline int is_entry_file (WIN32_FIND_DATA * fd) {
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

int dir_iterator_win (find_struct_t* fs_)
{
	int err_code = FUNC_OK;
	HANDLE h_find;
	WIN32_FIND_DATA find_data;

	// we want to return the buffer in same state as we've got it
	char *end_of_dir = fs_->path_buffer + fs_->used_buffer;
	size_t used_data = fs_->used_buffer;
	*end_of_dir = fs_->path_separator; end_of_dir++; fs_->used_buffer++;
	*end_of_dir = 0;
	
	// first, dig the childs
	if ( (fs_->flags & DIR_ITERATOR_RECURSIVE) ) {
		
		// copy the pattern at the end by trick it so that it gets overwritten
		if ( (fs_->flags & DIR_ITERATOR_ALL_DIRECTORIES) == 0 ) {
			size_t used_data_pattern = fs_->used_buffer;
			err_code = append_to_buffer (fs_, fs_->name_filter);
			if ( err_code != FUNC_OK ) {
				return err_code;
			}
			fs_->used_buffer = used_data_pattern;
		} else {
			fs_->path_buffer[fs_->used_buffer] = '*';
			fs_->path_buffer[fs_->used_buffer+1] = 0;
		}
		
		// start the search
		h_find = FindFirstFileEx(fs_->path_buffer, FindExInfoStandard, &find_data,
             FindExSearchLimitToDirectories, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
			return 0;
        }
        
        // for each directory
        do { if ( is_entry_directory (&find_data) ) {
			// append the name of the directory
			err_code = append_to_buffer (fs_, find_data.cFileName);
			if ( err_code != FUNC_OK ) {
				break;
			}
			
			// inform the callback
			if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_DIRECTORIES) == 0 ) {
				err_code = fs_->kb (fs_->path_buffer, end_of_dir, fs_->user_data);
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
			size_t used_data_pattern = fs_->used_buffer;
			err_code = append_to_buffer (fs_, fs_->name_filter);
			if ( err_code != FUNC_OK ) {
				return err_code;
			}
			fs_->used_buffer = used_data_pattern;
		} else {
			fs_->path_buffer[fs_->used_buffer] = '*';
			fs_->path_buffer[fs_->used_buffer+1] = 0;
		}
		
		// start the search
		h_find = FindFirstFileEx(fs_->path_buffer, FindExInfoStandard, &find_data,
             FindExSearchNameMatch, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
			return 0;
        }
        
        // for each file
        do { if ( is_entry_file (&find_data) ) {
			// append the name of the file
			err_code = append_to_buffer (fs_, find_data.cFileName);
			if ( err_code != FUNC_OK ) {
				break;
			}
			
			// inform the callback
			err_code = fs_->kb (fs_->path_buffer, end_of_dir, fs_->user_data);
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
	fs_->used_buffer = used_data;
	fs_->path_buffer[used_data] = 0;
	
	return FUNC_OK;
}
#else // AITOWN_WIN32

static inline int is_dot_dot (struct dirent *dent) {
	for (;;) {
		if ( dent->d_name[0] == '.' ) {
			if ( dent->d_name[0] == 0 ) {
				break;
			} else if ( dent->d_name[1] == '.' ) {
				if ( dent->d_name[2] == 0 )
					break;
			}
		}
		return 1;
	}
	return 0;
}

int dir_iterator_unix (find_struct_t* fs_)
{
	DIR *dir;
	struct dirent *e;
	
	// we want to return the buffer in same state as we've got it
	char *end_of_dir = fs_->path_buffer + fs_->used_buffer;
	size_t used_data = fs_->used_buffer;
	*end_of_dir = fs_->path_separator; end_of_dir++; fs_->used_buffer++;
	*end_of_dir = 0;
	
	// open the directory
	dir = opendir (fs_->path_buffer);
	if (dir == NULL) {
		return FUNC_GENERIC_ERROR;
	}
	
	// loop in all entries except . and .., symlinks
	while ( (dent = readdir (dir)) ) {
		if ( is_dot_dot (dent) )
			continue;
		if (S_ISLNK(st.st_mode))
			continue;
		
		append_to_buffer();
		
		if (S_ISDIR(st.st_mode)) {
			// pattern match
			if ( (fs_->flags & DIR_ITERATOR_ALL_DIRECTORIES) == 0 ) {
				if (regexec(reg, fn, 0, 0, 0)) {
					break;
				}
			}
			
			// inform the callback
			if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_DIRECTORIES) == 0 ) {
				err_code = fs_->kb (fs_->path_buffer, end_of_dir, fs_->user_data);
				if ( err_code != FUNC_OK ) {
					break;
				}
			}
			if ( (fs_->flags & DIR_ITERATOR_RECURSIVE) ) {
				err_code = dir_iterator_unix(fs_);
				if ( err_code != FUNC_OK ) {
					break;
				}
			}
		} else if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_FILES) == 0 ) {
		
			// pattern match
			if ( (fs_->flags & DIR_ITERATOR_ALL_FILES) == 0 ) {
				if (regexec(reg, fn, 0, 0, 0)) {
					break;
				}
			}
			
			// inform the callback
			err_code = fs_->kb (fs_->path_buffer, end_of_dir, fs_->user_data);
			if ( err_code != FUNC_OK ) {
				break;
			}
		}
	}
	
	if (dir)
		closedir(dir);
	// restore the buffer to its former glorry
	fs_->used_buffer = used_data;
	fs_->path_buffer[used_data] = 0;
	
	return FUNC_OK;
}
#endif // AITOWN_WIN32

int dir_iterator (const char *path_, const char *name_filter_, 
    dir_iterator_flags_t flags_, dir_iterator_foreach_t kb_, void *user_data_)
{
	find_struct_t	fs;
	fs.init_path = path_;
	fs.init_path_sz = strlen (path_);
	fs.name_filter = name_filter_;
	fs.kb = kb_;
	fs.flags = flags_;
	fs.user_data = user_data_;
	
	// create a buffer containing a copy of the path
	fs.path_buffer = (char*)malloc (1024);
	if ( fs.path_buffer == NULL )
		return FUNC_MEMORY_ERROR;
	fs.allocated_buffer = 1024;
	fs.used_buffer = fs.init_path_sz;
	memcpy (fs.path_buffer, path_, fs.init_path_sz+1);
	
#ifdef AITOWN_WIN32
	fs.path_separator = '\\';
	return dir_iterator_win(&fs);
#else
	
	fs.path_separator = '/';
	char pattern_bf[256];
	char *actual_pattern = pattern_bf;
	int b_dyn_pattern = 0;
	int orig_len = strlen (name_filter_);
	int i;
	int j;
	int err_code;
	
	if ( orig_len == 0 ) {
		// no pattern means all files
		orig_len = 1;
		pattern_bf[0] = '.';
		pattern_bf[1] = '*';
		pattern_bf[2] = 0;
	} else {
		// only use dynamic buffer if we have to
		if ( orig_len > 128 ){
			actual_pattern = (char*)malloc (orig_len*2);
			b_dyn_pattern = 1;
		}
		
		// put proper regex codes
		j = 0;
		for ( i = 0; i < orig_len; i++ ) {
			char c = name_filter_[i];
			if (c == '*') {
				actual_pattern[j] = '.'; j++;
				actual_pattern[j] = '*'; j++;
			} else if (c == '?') {
				actual_pattern[j] = '.'; j++;
			} else {
				actual_pattern[j] = c; j++;
			}
		}
		actual_pattern[i] = 0;
	}
	
	// run the search
	for (;;) {
		err_code = regcomp (&rex, actual_pattern, REG_ICASE | REG_NOSUB);
		if (err_code) {
			err_code = FUNC_GENERIC_ERROR;
			break;
		}
		err_code = dir_iterator_unix (&fs);
		regfree (&fs.rex);
	}
	
	// free the buffer
	if (b_dyn_pattern) {
		free (actual_pattern);
	}
	return err_code;
#endif

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


