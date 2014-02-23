/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_handle.h
  \date			September 2013
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
#include <aitown/utils.h>
#include <aitown/error_codes.h>
#include <aitown/pivotal_gmtime.h>
#include <stddef.h>
#include <aitown/dstorage_id.h>
#include <aitown/dstorage_func.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


//! describes the status of the memory
typedef enum {
    /* sts[0] */
    DSTORAGE_H_UNINITIALISED = 0,/**< The structure is not used */
    DSTORAGE_H_NO_DATA = 1,      /**< no attempt was made to retrieve the data */
    DSTORAGE_H_WITH_DATA = 2,    /**< The data was resolved; the pointer is valid */
    DSTORAGE_H_WAITING_DATA = 3, /**< Waiting for the data to come */
    DSTORAGE_H_TEMP_FAILURE = 4, /**< Can't get data but try again in some time (secoonds-years) */
    DSTORAGE_H_LOST = 5,         /**< Data is permanently lost */

    /* sts[1] */
    DSTORAGE_H_RED = 1,          /**< The color is red :) */

    /* sts[2] */
    DSTORAGE_H_CLEAN = 0,        /**< The buffer was not modified */
    DSTORAGE_H_DIRTY = 1         /**< The buffer was modified */


} dstorage_sts_t;

typedef union _dstorage_sts_bytes_t {
    uint64_t                    u;
    char                        c[8];
} dstorage_sts_bytes_t;

//! describes a dstorage handle
typedef struct _dstorage_handle_t {
    struct _dstorage_chunk_t*   p_data; /**< pointer to data chunk */
    dstorage_id_t               id;     /**< a 64-bit number that identifies the data */
    int64_t                     tstamp; /**< last access time */
    dstorage_sts_bytes_t        sts;/**< the data status */
    /*char                        sts[8]; */

    struct _dstorage_handle_t * left;   /**< left link */
    struct _dstorage_handle_t * right;  /**< right link */
} dstorage_handle_t;

struct _dstorage_handle_mng_t;


//! handle is red or black field
#define dstorage_handle_red_status(h)   ((h)->sts.u & 0xFF)/*sts[0]*/

//! handle is red or black field
#define dstorage_handle_red_status_unsafe   sts.c[0]

//! tell if a handle is red or black
#define dstorage_handle_is_red(h)   (dstorage_handle_red_status(h) != DSTORAGE_H_RED)



//! handle exclusive status field
#define dstorage_handle_gen_status(h)   (((h)->sts.u >> 8) & 0xFF)/*sts[1]*/

//! tell if a handle is initialised or not
#define dstorage_handle_is_init(h)   (dstorage_handle_gen_status(h) != DSTORAGE_H_UNINITIALISED)

//! tell if a handle is waiting for data
#define dstorage_handle_is_waiting(h)   (dstorage_handle_gen_status(h) == DSTORAGE_H_WAITING_DATA)

//! tell if a handle has data ready to be read
#define dstorage_handle_is_resolved(h)   (dstorage_handle_gen_status(h) == DSTORAGE_H_WITH_DATA)

//! tell if temporary errors prevented data retrieval
#define dstorage_handle_temp_errors(h)   (dstorage_handle_gen_status(h) == DSTORAGE_H_TEMP_FAILURE)

//! tell if definitive errors prevented data retrieval
#define dstorage_handle_data_lost(h)   (dstorage_handle_gen_status(h) == DSTORAGE_H_LOST)


//! change the status
#define dstorage_handle_set_status(h,s)     (h)->sts.u = (((h)->sts.u & (~0xFF00)) | (s << 8))

//! mark a handle as not initialised
#define dstorage_handle_mark_uninit(h)      dstorage_handle_set_status(h, DSTORAGE_H_UNINITIALISED)

//! mark a handle as initialised
#define dstorage_handle_mark_init(h)        dstorage_handle_set_status(h, DSTORAGE_H_NO_DATA)

//! mark a handle as waiting for data
#define dstorage_handle_mark_waiting(h)     dstorage_handle_set_status(h, DSTORAGE_H_WAITING_DATA)

//! mark a handle as having data ready to be read
#define dstorage_handle_mark_resolved(h)    dstorage_handle_set_status(h, DSTORAGE_H_WITH_DATA)

//! mark handle with temporary errors that prevented data retrieval
#define dstorage_handle_mark_temp_errors(h) dstorage_handle_set_status(h, DSTORAGE_H_TEMP_FAILURE)

//! mark the handle with definitive errors that prevented data retrieval
#define dstorage_handle_mark_data_lost(h)   dstorage_handle_set_status(h, DSTORAGE_H_LOST)



//! associated buffer dirty or not
#define dstorage_handle_dirty_status(h)  (((h)->sts.u >> 16) & 0xFF) /*sts[2]*/

//! tell if a handle is dirty
#define dstorage_handle_is_dirty(h)   (dstorage_handle_dirty_status(h) != DSTORAGE_H_CLEAN)

//! mark a handle dirty
#define dstorage_handle_mark_dirty(h)   (h)->sts.u = (((h)->sts.u & (~0xFF0000)) | (DSTORAGE_H_DIRTY << 16))

//! mark a handle clean
#define dstorage_handle_mark_clean(h)   (h)->sts.u = (((h)->sts.u & (~0xFF0000)) | (DSTORAGE_H_CLEAN << 16))



//! get the reference counter for a handle
#define dstorage_handle_ref_count(h)   (((h)->sts.u >> 32) & 0xFFFFFFFF)/*(*(u_int32_t*)(&(h)->sts[4]))*/

//! increase the reference counter for a handle
#define dstorage_handle_inc_ref(h,__u__)   (h)->sts.u += 0x100000000; /*((*((u_int32_t*)(&(h)->sts[4])))++);*/ VAR_UNUSED(__u__)

//! decrease the reference counter for a handle
#define dstorage_handle_dec_ref(h,__u__)   (h)->sts.u -= 0x100000000; /*((*((u_int32_t*)(&(h)->sts[4])))--); */ VAR_UNUSED(__u__)



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

//! initialize an empty handle
DSTORAGE_FUNC dstorage_handle_t*
dstorage_handle_init_empty (struct _dstorage_handle_mng_t *mng);

//! initialize a handle with an id
DSTORAGE_FUNC dstorage_handle_t*
dstorage_handle_init (struct _dstorage_handle_mng_t *mng, dstorage_id_t id);

//! terminate the handle
DSTORAGE_FUNC void
dstorage_handle_end (struct _dstorage_handle_mng_t *mng, dstorage_handle_t** h);


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
#endif // AITOWN_dstorage_handle_h_INCLUDE
