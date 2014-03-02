/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.h
  \date			February 2014
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

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/dbg_assert.h>

#include <aitown/aitown-dstorage-h-mng.h>
#include <aitown/aitown-dstorage-ctrl-mng.h>
#include <aitown/aitown-dstorage-data.h>
#include <aitown/aitown-dstorage-handle.h>

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

//! describes a dstorage instance
///
typedef struct _aitown_dstorage_t {

    struct _aitown_dstorage_ctrl_mng_t  ctrl_mng; /**< controller manager */
    struct _aitown_dstorage_h_mng_t     h_mng; /**< handle manager */
    struct _aitown_db_mng_t             db_mng; /**< database manager */

    struct _aitown_cfg_sect_t *         cfg_sect; /**< configuration section */
    int                                 manage_cfg; /**< do we save the config at the end or not */

} aitown_dstorage_t;

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


//! initialize a dstorage structure with settings from a section
///
AITOWN_EXPORT func_error_t
aitown_dstorage_init (
        struct _aitown_dstorage_t * dstorage,
        struct _aitown_cfg_sect_t * cfg_sect);


//! initialize a dstorage structure with settings from a config file
///
AITOWN_EXPORT func_error_t
aitown_dstorage_finit (
        struct _aitown_dstorage_t * dstorage,
        const char * file);


//! terminate a dstorage structure
///
AITOWN_EXPORT void
aitown_dstorage_end (
        struct _aitown_dstorage_t *dstorage);


//! get a reference to a handle
///
static inline void
aitown_dstorage_handle_incref (
        struct _aitown_dstorage_handle_t *handle,
        void * owner)
{
    aitown_dstorage_h_mng_handle_incref (handle, owner);
}

//! release a reference to a handle
///
static inline void
aitown_dstorage_handle_decref (
        struct _aitown_dstorage_t * mng,
        struct _aitown_dstorage_handle_t *handle,
        void * owner)
{
    aitown_dstorage_h_mng_handle_decref (&mng->h_mng, handle, owner);
}


//! create a new handle-id pair
///
static inline struct _aitown_dstorage_handle_t *
aitown_dstorage_new_handle (
        struct _aitown_dstorage_t *mng,
        unsigned size)
{
    return aitown_dstorage_h_mng_new_handle (
                &mng->h_mng,
                aitown_dstorage_ctrl_mng_best (&mng->ctrl_mng),
                size);
}


//! get the handle for a particular id
///
/// The function first looks the id in the list of those that have
/// already been loaded and returns coresponding handle
///
/// In any case the caller gets a reference owned by the handle itself.
///
static inline struct _aitown_dstorage_handle_t *
aitown_dstorage_handle (
        struct _aitown_dstorage_t *mng,
        aitown_dstorage_id_t id)
{
    return aitown_dstorage_h_mng_get_handle (&mng->h_mng, id);
}


//! resolve the handle for a particular id
///
/// The function first looks the id in the list of those that have
/// already been loaded and returns coresponding handle
///
/// In any case the caller gets a reference owned by the handle itself.
///
AITOWN_EXPORT func_error_t
aitown_dstorage_resolve_handle (
        struct _aitown_dstorage_t * mng,
        struct _aitown_dstorage_handle_t * handle);



//! the size of the data associated with an ID
///
static inline unsigned
aitown_dstorage_raw_size (
        struct _aitown_dstorage_handle_t * handle)
{
    DBG_ASSERT (handle != NULL);
    DBG_ASSERT (handle->data != NULL);
    return handle->data->size;
}


//! pointer to raw data associated with an ID
///
static inline void *
aitown_dstorage_raw (
        struct _aitown_dstorage_handle_t * handle)
{
    DBG_ASSERT (handle != NULL);
    DBG_ASSERT (handle->data != NULL);
    return aitown_dstorage_data_buffer (handle->data);
}

//! set or replace raw data
///
AITOWN_EXPORT func_error_t
aitown_dstorage_set_raw (
        struct _aitown_dstorage_handle_t * handle,
        const void * data,
        unsigned data_size);


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
