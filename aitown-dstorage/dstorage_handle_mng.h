/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_handle_mng.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_handle_mng_h_INCLUDE
#define AITOWN_dstorage_handle_mng_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/utils.h>
#include <aitown/error_codes.h>
#include <aitown/pivotal_gmtime.h>
#include <aitown/dstorage_id.h>
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

//! describes a dstorage handle manager
///
/// This is a stub now but may be used to optimise allocation of handlers
/// in the future.
typedef struct _dstorage_handle_mng_t {
    char tmp[8];
} dstorage_handle_mng_t;

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

/* DEPRECATED */
//! initialize a handle manager
DSTORAGE_FUNC void
dstorage_handle_mng_init (dstorage_handle_mng_t *mng);

//! terminate the handle manager
DSTORAGE_FUNC void
dstorage_handle_mng_end (dstorage_handle_mng_t *mng);

//! get a handle for a given id
DSTORAGE_FUNC void
dstorage_handle_mng_get (dstorage_handle_mng_t *mng, dstorage_id_t id);
/* DEPRECATED */

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
#endif // AITOWN_dstorage_handle_mng_h_INCLUDE
