/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_h_INCLUDE
#define AITOWN_dstorage_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "dstorage_handle_mng.h"
#include "dstorage_handle.h"
#include "dstorage_lookup.h"
#include "dstorage_ctrl.h"
#include "dstorage_clist.h"
#include "dstorage_controllers.h"
#include "dstorage_chunk.h"
#include "dstorage_chunk_mng.h"
#include "dstorage_id.h"

#include <aitown/struct_ini.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a dstorage
///
/// One such structure holds data about an instance of a dstorage. There may
/// be multiple, independent instances in the same program.
typedef struct _dstorage_t {
    char*                   p_name;     /**< the name of the base; used as file name */
    char*                   p_db_file;  /**< file for the database */
    struct_ini_t            sini;       /**< the setting from config file */
    dstorage_lookup_t       lku;        /**< the database for the ids */
    dstorage_clist_t        clist;      /**< the list of controllers */
    dstorage_chunk_mng_t    ckmng;      /**< memory manager for chunks */

    /* DEPRECATED */
    dstorage_handle_mng_t   hmng;       /**< handle manager */
} dstorage_t;

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

//! initialize the structure
AITOWN_EXPORT void
dstorage_init (dstorage_t *dstorage, const char *p_name, const char *p_ini_file);

//! terminate the structure
AITOWN_EXPORT void
dstorage_end (dstorage_t *dstorage);

//! get the handle for an id
AITOWN_EXPORT dstorage_handle_t *
dstorage_handle (dstorage_t *dstorage, dstorage_id_t id, void *owner);

//! release the handle
AITOWN_EXPORT void
dstorage_handle_done (dstorage_t *dstorage, dstorage_handle_t **handle, void *owner);

//! resolve the handle
AITOWN_EXPORT void
dstorage_handle_resolve (dstorage_t *dstorage, dstorage_handle_t *handle,
                         dstorage_ctrl_response kb);

//! create a new id, handle and chunk of given number of bytes
AITOWN_EXPORT dstorage_handle_t *
dstorage_new (dstorage_t *dstorage, size_t sz);


//! allocate a chunk of memory
///
/// @warning This is for controller use; do not call this function
/// for other purposes!
///
/// The implementation attempt a simple allocation but tries harder if it
/// fails, by freing chunks frm old handlers.
AITOWN_EXPORT dstorage_chunk_t*
dstorage_alloc_chunk (dstorage_t *dstorage, size_t sz);


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
#endif /* AITOWN_dstorage_h_INCLUDE */
