/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_lookup.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_lookup_h_INCLUDE
#define AITOWN_dstorage_lookup_h_INCLUDE
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
#include <aitown/dstorage_handle.h>
#include <aitown/dstorage_ctrl.h>
#include <aitown/dstorage_cdata.h>

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

//! describes a dstorage lookup table
typedef struct _dstorage_lookup_t {
    void *              database;   /**< handle for our back-end database */
    dstorage_handle_t * tree;       /**< or map of id-handle pairs */
    dstorage_cdata_t    header;     /**< the copy of the header (record 0) */
} dstorage_lookup_t;

//! get current id to be used
#define dstorage_lookup_crt_id(lku)             dstorage_cdata_64(&lku->header)

//! set current id to be used
#define dstorage_lookup_set_crt_id(lku, id)     dstorage_cdata_64_set(&lku->header, id)

//! get the value and advance current id
#define dstorage_lookup_get_a_new_id(lku)       dstorage_cdata_64(&lku->header)++


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

//! initialize the look-up structure
DSTORAGE_FUNC void
dstorage_lookup_init (dstorage_lookup_t *lku);

//! initialise the look-up structure with a database of ids
DSTORAGE_FUNC void
dstorage_lookup_init_open (dstorage_lookup_t *lku, const char * database);

//! terminate the look-up structure
DSTORAGE_FUNC void
dstorage_lookup_end (dstorage_lookup_t *lku);

//! find a handle given the id (fast) or NULL if not inside
DSTORAGE_FUNC dstorage_handle_t *
dstorage_lookup_handle (dstorage_lookup_t *lku, dstorage_id_t id);

//! find an id given the handle (slow) or 0 if not inside
DSTORAGE_FUNC dstorage_id_t
dstorage_lookup_id (dstorage_lookup_t *lku, dstorage_handle_t *handle);

//! add a handle and the id inside
DSTORAGE_FUNC void
dstorage_lookup_add (dstorage_lookup_t *lku, dstorage_handle_t *handle);

//! remove a handle
DSTORAGE_FUNC void
dstorage_lookup_rem (dstorage_lookup_t *lku, dstorage_handle_t *handle);

//! get data associated with an id or NULL
///
/// The handle status is set ONLY if an error occurs
DSTORAGE_FUNC dstorage_cdata_t *
dstorage_lookup_cdata (dstorage_lookup_t *lku, dstorage_handle_t *handle);

//! release data associated with an id
///
/// The pointer MUST have come from a previous dstorage_lookup_cdata() call
DSTORAGE_FUNC void
dstorage_lookup_free_cdata (dstorage_lookup_t *lku, dstorage_cdata_t *cdata);

/// allocate a new id in the database
DSTORAGE_FUNC dstorage_id_t
dstorage_lookup_new_id (dstorage_lookup_t *lku, dstorage_ctrl_t * ctrl);


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
#endif // AITOWN_dstorage_lookup_h_INCLUDE
