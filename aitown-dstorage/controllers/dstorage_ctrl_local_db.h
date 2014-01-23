/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl_local_db.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_ctrl_local_db_h_INCLUDE
#define AITOWN_dstorage_ctrl_local_db_h_INCLUDE
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
/// The controller uses a database to store
/// data. The key is the id and the value is the content.
typedef struct _dstorage_ctrl_local_db_t {
    dstorage_ctrl_t header;
    void *          database;   /**< database handle */
} dstorage_ctrl_local_db_t;


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
dstorage_ctrl_local_db_init ();

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
#endif // AITOWN_dstorage_ctrl_local_db_h_INCLUDE
