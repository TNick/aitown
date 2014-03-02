/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-h-mng.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_h_mng_h_INCLUDE
#define AITOWN_dstorage_h_mng_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/aitown-dstorage-defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_cfg_sect_t;
struct _aitown_dstorage_handle_t;
struct _aitown_db_t;
struct _aitown_dstorage_controller_t;

//! describes a handle manager
///
typedef struct _aitown_dstorage_h_mng_t {

    unsigned                            handle_max; /**< maximum number of handlers */
    unsigned                            handle_count; /**< current number of handlers */

    unsigned                            free_max; /**< maximum number of free handlers */
    unsigned                            free_count; /**< current number of free handlers */

    unsigned                            cache_max; /**< maximum number of cache handlers */
    unsigned                            cache_count; /**< current number of cache handlers */

    aitown_dstorage_id_t                next_id; /**< next id to be allocated */

    struct _aitown_db_t *               db; /**< the database for id-controller pairs */

    struct _aitown_dstorage_handle_t *  tree; /**< our map of id-handle pairs */
    struct _aitown_dstorage_handle_t *  free; /**< the list of freed handlers */

} aitown_dstorage_h_mng_t;

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


//! initialize a handle manager
///
func_error_t
aitown_dstorage_h_mng_init (
        struct _aitown_db_mng_t * dbmng,
        struct _aitown_dstorage_h_mng_t * mng,
        struct _aitown_cfg_sect_t * dstorage_sect);


//! terminate a handle manager
///
void
aitown_dstorage_h_mng_end (
        struct _aitown_dstorage_h_mng_t *h_mng,
        struct _aitown_cfg_sect_t * dstorage_sect);


//! get a reference to a handle
///
AITOWN_EXPORT void
aitown_dstorage_h_mng_handle_incref (
        struct _aitown_dstorage_handle_t *handle,
        void * owner);

//! release a reference to a handle
///
AITOWN_EXPORT void
aitown_dstorage_h_mng_handle_decref (
        struct _aitown_dstorage_h_mng_t * mng,
        struct _aitown_dstorage_handle_t *handle,
        void * owner);


//! get the handle for a particular id
///
/// The function first looks the id in the list of those that have
/// already been loaded and returns coresponding handle
///
/// In any case the caller gets a reference owned by the handle itself.
///
AITOWN_EXPORT struct _aitown_dstorage_handle_t *
aitown_dstorage_h_mng_get_handle (
        struct _aitown_dstorage_h_mng_t *h_mng,
        aitown_dstorage_id_t id);


//! create a new handle-id pair
///
AITOWN_EXPORT struct _aitown_dstorage_handle_t *
aitown_dstorage_h_mng_new_handle (
        struct _aitown_dstorage_h_mng_t *h_mng,
        struct _aitown_dstorage_controller_t * ctrl,
        unsigned size);


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
#endif /* AITOWN_dstorage_h_mng_h_INCLUDE */
