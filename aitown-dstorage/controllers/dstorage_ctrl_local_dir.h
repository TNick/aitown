/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl_local_dir.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_ctrl_local_dir_h_INCLUDE
#define AITOWN_dstorage_ctrl_local_dir_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/dstorage_ctrl.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! an instance of this controller
///
/// The controller uses a directory and files to store
/// data. The files have the same name as the id.
typedef struct _dstorage_ctrl_local_dir_t {
    dstorage_ctrl_t header;
    char*           p_path;
    char*           p_file;
} dstorage_ctrl_local_dir_t;

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

//! adds this type of controller to the list
DSTORAGE_FUNC void
dstorage_ctrl_local_dir_init ();

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
#endif // AITOWN_dstorage_ctrl_local_dir_h_INCLUDE
