/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			utils_posix.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_utils_posix_h_INCLUDE
#define AITOWN_utils_posix_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#   define _XOPEN_SOURCE 500
#   define __USE_XOPEN_EXTENDED
#	include <unistd.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#   include <ftw.h>
#	include <dirent.h>
#	include <regex.h>
#	include <errno.h>
#	include <err.h>

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
	size_t ret_sz = 0;
	
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
			ret_sz = strlen(tmp_buf);
			char * tmp_buf2 = (char*)realloc(tmp_buf, ret_sz+1);
			if ( tmp_buf2 == NULL )
				break;
			ret_buf = tmp_buf2;
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
	
	*sz = ret_sz;
	return ret_buf;
}

//! create directory
static inline func_error_t 
du_mkdir_sysdep (const char *path)
{
	// read/write/search permissions for owner and group, 
	// and with read/search permissions for others.
	int sts = mkdir (path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return (sts == 0 ? FUNC_OK : FUNC_GENERIC_ERROR);
}

static inline int 
unlink_cb(const char *fpath, const struct stat *sb, 
    int typeflag, struct FTW *ftwbuf)
{
	VAR_UNUSED (sb);
	VAR_UNUSED (typeflag);
	int rv = 0;
	if ( ftwbuf->level > 0 ) {
		rv = remove(fpath);
	}
	return rv;
}

//! remove directory
static inline func_error_t 
du_rm_sysdep (const char * path_, int including_myself) {
	func_error_t err_code;
	int ret = nftw (path_, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
	if ( ret == 0 ) {
		if (including_myself) {
			err_code = (rmdir(path_) == 0 ? FUNC_OK : FUNC_GENERIC_ERROR);
		} else {
			err_code = (ret == 0 ? FUNC_OK : FUNC_GENERIC_ERROR);
		}
	} else {
		err_code = FUNC_GENERIC_ERROR; 
	}
	return err_code;
}

//! tell if directory exists
access_rights_t 
du_existsdir_sysdep (const char * path_)
{
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
}

//! tell if file exists
access_rights_t 
du_existsfile_sysdep (const char * path_)
{
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
}
#endif // DIR_UTILS_C

#ifdef DIR_ITERATOR_C
typedef struct {
	regex_t rex;
	const char *init_path;
	size_t init_path_sz;
	const char *name_filter;
	dir_iterator_flags_t flags;
	dir_iterator_foreach_t kb;
	void *user_data;
	
	char_buff_t chb;
	
	char path_separator;
} find_struct_t;

static inline int is_dot_dot (struct dirent *dent) {
	for (;;) {
		if ( dent->d_name[0] == '.' ) {
			if ( dent->d_name[1] == 0 ) {
				break;
			} else if ( dent->d_name[1] == '.' ) {
				if ( dent->d_name[2] == 0 )
					break;
			}
		}
		return 0;
	}
	return 1;
}

func_error_t dir_iterator_unix (find_struct_t* fs_)
{
	DIR *dir;
	struct dirent *dent;
	struct stat st;
	size_t preserve_used;
	const char * file_name;
	int err_code = FUNC_OK;
	
	// we want to return the buffer in same state as we've got it
	size_t used_data = fs_->chb.used;
	{ // don't use end_of_dir below, the buffer may be rellocated
		char *end_of_dir = fs_->chb.data + fs_->chb.used;
		*end_of_dir = fs_->path_separator; end_of_dir++; fs_->chb.used++;
		*end_of_dir = 0;
	}
	
	// open the directory
	dir = opendir (fs_->chb.data);
	if (dir == NULL) {
		return FUNC_GENERIC_ERROR;
	}
	preserve_used = fs_->chb.used;
	
	// loop in all entries except . and .., symlinks
	while ( (dent = readdir (dir)) ) {
		fs_->chb.used = preserve_used;
		if ( is_dot_dot (dent) )
			continue;
		
		// copy this name after the path; trick it so that it does not increase
		err_code = char_buff_add(&fs_->chb, dent->d_name);
		if ( err_code != FUNC_OK ) {
			break;
		}
		file_name = fs_->chb.data + preserve_used;
		
		// get file info; if symbolic link, don't follow
		if (lstat (fs_->chb.data, &st) == -1)
			continue; // silently ignoring the error
		if (S_ISLNK(st.st_mode))
			continue;
		
		if (S_ISDIR(st.st_mode)) {
			// pattern match if they don't get all included
			if ( (fs_->flags & DIR_ITERATOR_ALL_DIRECTORIES) == 0 ) {
				if (regexec (&fs_->rex, file_name, 0, 0, 0))
					continue;
			}
			
			// inform the callback
			if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_DIRECTORIES) == 0 ) {
				err_code = fs_->kb (fs_->chb.data, file_name, fs_->user_data, 0);
				if ( err_code != FUNC_OK ) {
					break;
				}
			}
			
			// recursive? dive in...
			if ( (fs_->flags & DIR_ITERATOR_RECURSIVE) ) {
				err_code = dir_iterator_unix(fs_);
				if ( err_code != FUNC_OK ) {
					break;
				}
			}
		} else if ( (fs_->flags & DIR_ITERATOR_EXCLUDE_FILES) == 0 ) {
		
			// pattern match
			if ( (fs_->flags & DIR_ITERATOR_ALL_FILES) == 0 ) {
				if (regexec (&fs_->rex, file_name, 0, 0, 0)) {
					continue;
				}
			}
			
			// inform the callback
			err_code = fs_->kb (fs_->chb.data, file_name, fs_->user_data, 1);
			if ( err_code != FUNC_OK ) {
				break;
			}
		}
	}
	
	if (dir)
		closedir(dir);
	// restore the buffer to its former glorry
	fs_->chb.used = used_data;
	fs_->chb.data[used_data] = 0;
	
	return FUNC_OK;
}

func_error_t dir_iterator_sysdep (const char *path_, const char *name_filter_, 
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
	char_buff_init (&fs.chb, 1024);
	if ( fs.chb.data == NULL )
		return FUNC_MEMORY_ERROR;
	char_buff_add_string (&fs.chb, path_,fs.init_path_sz);
	fs.path_separator = DU_PATH_SEP_C;

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
			} else if (c == ')') {
				actual_pattern[j] = '\\'; j++;
				actual_pattern[j] = ')'; j++;
			} else if (c == '(') {
				actual_pattern[j] = '\\'; j++;
				actual_pattern[j] = '('; j++;
			} else if (c == '.') {
				actual_pattern[j] = '\\'; j++;
				actual_pattern[j] = '.'; j++;
			} else {
				actual_pattern[j] = c; j++;
			}
		}
		actual_pattern[j] = 0;
	}
	
	// run the search
	for (;;) {
		err_code = regcomp (&fs.rex, actual_pattern, REG_ICASE | REG_NOSUB);
		if (err_code) {
			err_code = FUNC_GENERIC_ERROR;
			break;
		}
		err_code = dir_iterator_unix (&fs);
		regfree (&fs.rex);
		break;
	}
	
	// free the buffer
	if (b_dyn_pattern) {
		free (actual_pattern);
	}
	return err_code;
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
#endif /* AITOWN_utils_posix_h_INCLUDE */
