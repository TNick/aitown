/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_controllers.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_controllers_h_INCLUDE
#define AITOWN_dstorage_controllers_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/utils.h>
#include <aitown/error_codes.h>
#include <aitown/dstorage_func.h>
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

struct _dstorage_ctrl_t;
struct _dstorage_t;
struct _struct_ini_sect_t;

//! the callback that is capable of creating a controller
typedef struct _dstorage_ctrl_t * (*dstorage_ctrl_creator) (
        struct _dstorage_t *, struct _struct_ini_sect_t*);

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

//! initialize a controller list structure; needs to be called at app start
/// It is safe to call this function more than once
DSTORAGE_FUNC void
dstorage_controllers_init ();

//! terminate the controller list structure;
/// It is NOT safe to call this function more than once
DSTORAGE_FUNC void
dstorage_controllers_end ();

//! find the callback associated with the string
DSTORAGE_FUNC dstorage_ctrl_creator
dstorage_controllers_find_callback (const char *p_name);

//! add a callback that is capable of creating a controller
DSTORAGE_FUNC void
dstorage_controllers_add_callback (dstorage_ctrl_creator kb, const char *p_name);

//! remove a callback that is capable of creating a controller
DSTORAGE_FUNC void
dstorage_controllers_rem_callback (const char *p_name);

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
#endif // AITOWN_dstorage_controllers_h_INCLUDE
