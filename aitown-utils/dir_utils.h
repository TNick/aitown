/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dir_utils.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dir_utils_h_INCLUDE
#define AITOWN_dir_utils_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include "error_codes.h"
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

typedef enum {
	DU_NONE = 0x0000,
	DU_READABLE = 0x0001,
	DU_WRITABLE = 0x0002,
	DU_EXECUTABLE = 0x0004
} access_rights_t;

#ifdef AITOWN_WIN32
#    define DU_PATH_SEP_C '\\'
#    define DU_PATH_SEP "\\"
#else
#    define DU_PATH_SEP_C '/'
#    define DU_PATH_SEP "/"
#endif

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

//! get current working directory
/// returns a buffer allocated for this purpose; this is not thread safe
/// as it uses a static data member
AITOWN_EXPORT const char *
du_pwd (size_t *sz);

//! change current working directory
AITOWN_EXPORT func_error_t
du_cd (const char * new_path_);

//! change current working directory
/// the ownership of the pointer is taken by the library
AITOWN_EXPORT func_error_t
du_cd_ptr (char * new_path_);

//! create a directory in specified path
//! if \a path_ is NULL, it defaults to current directory
AITOWN_EXPORT func_error_t
du_mkdir (const char *path_, const char *name_);

//! create a directory in specified path
AITOWN_EXPORT func_error_t
du_mkdir_p (const char *path_);

//! create all missing pices of a path
AITOWN_EXPORT func_error_t
du_mkpath (const char * path_);

//! remove a directory (may be non-empty) or a file
AITOWN_EXPORT func_error_t
du_rm (const char * path_);

//! remove the content of a directory, but keep the directory
AITOWN_EXPORT func_error_t
du_cleardir (const char * path_);

//! tell if a directory exists and what rights the user has
/// if it exists it returns something other than DU_NONE
AITOWN_EXPORT access_rights_t
du_existsdir (const char * path_);

//! tell if a file exists and what rights the user has
/// if it exists it returns something other than DU_NONE
AITOWN_EXPORT access_rights_t
du_existsfile (const char * path_);


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
#endif // AITOWN_dir_utils_h_INCLUDE
