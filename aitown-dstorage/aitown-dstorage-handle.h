/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-handle.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_handle_h_INCLUDE
#define AITOWN_dstorage_handle_h_INCLUDE
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

struct _aitown_dstorage_controller_t;
struct _aitown_dstorage_data_t;

//! describes the status of the handle
typedef enum {
    /* sts[1] */
    DSTORAGE_H_UNINITIALISED = 0,/**< The structure is not used */
    DSTORAGE_H_NO_DATA = 1,      /**< no attempt was made to retrieve the data */
    DSTORAGE_H_WITH_DATA = 2,    /**< The data was resolved; the pointer is valid */
    DSTORAGE_H_WAITING_DATA = 3, /**< Waiting for the data to come */
    DSTORAGE_H_TEMP_FAILURE = 4, /**< Can't get data but try again in some time (secoonds-years) */
    DSTORAGE_H_LOST = 5,         /**< Data is permanently lost */

    /* sts[0] */
    DSTORAGE_H_RED = 1,          /**< The color is red :) */

    /* sts[2] */
    DSTORAGE_H_CLEAN = 0x0000,   /**< The buffer was not modified */
    DSTORAGE_H_DIRTY = 0x0001,   /**< The buffer was modified */

    DSTORAGE_H_DBMEM = 0x0000,   /**< memory was allocated by the database */
    DSTORAGE_H_LOCMEM = 0x0002   /**< memory was allocated by this module */


} aitown_dstorage_sts_t;


//! status bytes may be referenced either as a whole or as parts
typedef union _aitown_dstorage_sts_bytes_t {
    uint64_t                    u;
    char                        c[8];
} aitown_dstorage_sts_bytes_t;

//! describes a dstorage instance
///
typedef struct _aitown_dstorage_handle_t {

    struct _aitown_dstorage_data_t * data; /**< associated data */

    aitown_dstorage_id_t            id; /**< the ID that we're representing here */
    aitown_dstorage_sts_bytes_t     sts; /**< the data status */
    struct _aitown_dstorage_controller_t * ctrl; /**< the controller that manages this ID */

    int                             ref_count; /**< number of references */

    struct _aitown_dstorage_handle_t * left;   /**< left link in tree */
    struct _aitown_dstorage_handle_t * right;  /**< right link in tree */

} aitown_dstorage_handle_t;

//! handle is red or black field
#define dstorage_handle_red_status_unsafe   sts.c[0]


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


//! initialize a dstorage structure with settings from a section, ie [dstorage/handles/11]
///
AITOWN_EXPORT func_error_t
aitown_dstorage_handle_init (
        struct _aitown_dstorage_handle_t *handle,
        aitown_dstorage_id_t id);


//! terminate a dstorage structure
///
AITOWN_EXPORT void
aitown_dstorage_handle_end (
        struct _aitown_dstorage_handle_t *handle);


//! get the reference counter (number of references)
static inline int
aitown_dstorage_handle_refcount(
        struct _aitown_dstorage_handle_t *handle)
{
    return (int)((handle->sts.u >> 32) & 0xFFFFFFFF);
}


/* -------------- STATUS ------------------ */

//! get handle status
static inline aitown_dstorage_sts_t
aitown_dstorage_handle_status(
        struct _aitown_dstorage_handle_t *handle)
{
    return (aitown_dstorage_sts_t)((handle->sts.u & 0x0000FF00) >> 8);
}

//! set handle status
static inline void
aitown_dstorage_handle_set_status(
        struct _aitown_dstorage_handle_t *handle, aitown_dstorage_sts_t sts)
{
    handle->sts.u = (aitown_dstorage_sts_t)(
                handle->sts.u | ((sts & 0xFF) << 8));
}


/* -------------- FLAGS ------------------ */

//! get handle flags
static inline aitown_dstorage_sts_t
aitown_dstorage_handle_flags(
        struct _aitown_dstorage_handle_t *handle)
{
    return (aitown_dstorage_sts_t)((handle->sts.u & 0x00FF0000) >> 16);
}

//! set a flag
static inline void
aitown_dstorage_handle_set_flag(
        struct _aitown_dstorage_handle_t *handle, int flag )
{
    handle->sts.u = (aitown_dstorage_sts_t)(
                handle->sts.u | ((flag & 0xFF) << 16));
}

//! reset (clear) a flag
static inline void
aitown_dstorage_handle_reset_flag(
        struct _aitown_dstorage_handle_t *handle, int flag )
{
    handle->sts.u = (aitown_dstorage_sts_t)(
                handle->sts.u & (~((flag & 0xFF) << 16)));
}


//! tell if the buffer inside suffered changes
static inline int
aitown_dstorage_handle_dirty(
        struct _aitown_dstorage_handle_t *handle)
{
    return (aitown_dstorage_handle_flags(handle) & DSTORAGE_H_DIRTY);
}

//! mark the handle as dirty
static inline void
aitown_dstorage_handle_set_dirty(
        struct _aitown_dstorage_handle_t *handle)
{
    aitown_dstorage_handle_set_flag (handle, DSTORAGE_H_DIRTY);
}

//! mark the handle as clean
static inline void
aitown_dstorage_handle_set_clean(
        struct _aitown_dstorage_handle_t *handle)
{
    aitown_dstorage_handle_reset_flag (handle, DSTORAGE_H_DIRTY);
}


//! tell if the buffer inside was allocated by this library
static inline int
aitown_dstorage_handle_local_mem(
        struct _aitown_dstorage_handle_t *handle)
{
    return (aitown_dstorage_handle_flags(handle) & DSTORAGE_H_LOCMEM);
}

//! mark the buffer inside as being allocated by this library
static inline void
aitown_dstorage_handle_set_mem_local(
        struct _aitown_dstorage_handle_t *handle)
{
    aitown_dstorage_handle_set_flag (handle, DSTORAGE_H_LOCMEM);
}

//! mark the buffer inside as being allocated by the database
static inline void
aitown_dstorage_handle_set_mem_db(
        struct _aitown_dstorage_handle_t *handle)
{
    aitown_dstorage_handle_reset_flag (handle, DSTORAGE_H_LOCMEM);
}


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
#endif /* AITOWN_dstorage_handle_h_INCLUDE */
