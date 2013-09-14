/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dir_iterator.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dir_iterator_h_INCLUDE
#define AITOWN_dir_iterator_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <stddef.h>

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

//! type filter flags
typedef enum {
	DIR_ITERATOR_FILES_AND_DIRECTORIES = 0,
	DIR_ITERATOR_EXCLUDE_DIRECTORIES = 0x0001,   /**< no dirs reach the callback */
	DIR_ITERATOR_EXCLUDE_FILES = 0x0002,   /**< no files reach the callback */
	DIR_ITERATOR_ALL_DIRECTORIES = 0x0004, /**< don't use the pattern for directories, dump all to the callback */
	DIR_ITERATOR_ALL_FILES = 0x0008,       /**< don't use the pattern for files, dump all to the callback */
	
	DIR_ITERATOR_RECURSIVE = 0x0010
} dir_iterator_flags_t;

//! the callback type used with dir_iterator
/// @return 0 to continue or an error code to be returned by dir_iterator
typedef func_error_t (*dir_iterator_foreach_t) (
	const char * path_, 
	const char * name_,
	void * user_data_, 
	int is_file_);

//! iterate in files and folders
/// @return 0 for success or the error code returned by callback
AITOWN_EXPORT func_error_t
dir_iterator (
	const char * path_, 
	const char * name_filter_,
	dir_iterator_flags_t flags_,
	dir_iterator_foreach_t kb_, 
	void * user_data_);


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
#endif // AITOWN_dir_iterator_h_INCLUDE
