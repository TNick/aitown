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
#include <aitown/aitown-db-mng.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a dstorage instance
///
/// One such structure holds data about an instance of a dstorage. There may
/// be multiple, independent instances in the same program. It is the main
/// interface for the program and the methods associated with it are the
/// public API interface.
typedef struct _dstorage_t {
    char*                   p_name;     /**< the name of the base; used as file name */
    char*                   p_db_file;  /**< file for the database */
    struct_ini_t            sini;       /**< the setting from config file */
    dstorage_lookup_t       lku;        /**< the database for the ids */
    dstorage_clist_t        clist;      /**< the list of controllers */
    dstorage_chunk_mng_t    ckmng;      /**< memory manager for chunks */
    aitown_db_mng_t         db;         /**< manager for databases */



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


//! initialize a dstorage structure
///
/// Before using a dstorage_t instance one needs to initialise it using a
/// call to this method. It initialises sub-components based on the settings
/// provided by config file. This process includes loading controllers
/// (and the plug-ins that provide them, if any). Call dstorage_end() when
/// dstorage_t instance is no longer needed.
AITOWN_EXPORT void
dstorage_init (
        dstorage_t *dstorage,
        const char *p_name,
        const char *p_ini_file);


//! terminate a dstorage structure
///
/// Various subcomponents are terminated and all memory is freed.
AITOWN_EXPORT void
dstorage_end (
        dstorage_t *dstorage);


//! get the handle for an id
///
/// An unresolved handle (no memory associated) is returned. If a handle for
/// this id was requested before then same handle is returned, otherwise
/// a new one is created. The id is NOT checked in the database to see if
/// it is valid.
///
/// @warning Avoid using this method for new IDs. Instead, use dstorage_new().
AITOWN_EXPORT dstorage_handle_t *
dstorage_handle (
        dstorage_t *dstorage,
        dstorage_id_t id,
        void *owner);


//! release the handle reference
///
/// If the reference counter reaches 0,
/// depending on the memory availability the handle may be left in
/// cache or it may be freed right away.
///
/// The dirty flag is checked and, if set, the buffer is
/// flushed back to associated controller.
AITOWN_EXPORT void
dstorage_handle_done (
        dstorage_t *dstorage,
        dstorage_handle_t **handle,
        void *owner);


//! resolve the handle
///
/// This method querries the ID database for controller specific
/// data, then asks the controller to read the content in memory.
/// The status of the querry should be checked in the callback, as
/// the controller may perform asyncronous opperations.
///
/// There is a strong chance for this function to fail. Thake that
/// into consideration when designing the code.
AITOWN_EXPORT void
dstorage_handle_resolve (
        dstorage_t *dstorage,
        dstorage_handle_t *handle,
        void * user,
        dstorage_ctrl_response kb);


//! create a new id, handle and chunk of given number of bytes
///
/// This is how a new ID comes into existance. The memory is allocated
/// but empty. The reference count is set to 1.
///
/// There is a strong chance for this function to fail. Thake that
/// into consideration when designing the code.
AITOWN_EXPORT dstorage_handle_t *
dstorage_new (
        dstorage_t *dstorage,
        size_t sz);


//! allocate a chunk of memory
///
/// @warning This is for controller use; do not call this function
/// for other purposes!
///
/// The implementation attempt a simple allocation but tries harder if it
/// fails, by freing chunks frm old handlers.
///
/// There is a real chance that this may be NULL.
AITOWN_EXPORT dstorage_chunk_t*
dstorage_alloc_chunk (
        dstorage_t *dstorage,
        size_t sz);


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
