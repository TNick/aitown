/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_ctrl.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_ctrl_h_INCLUDE
#define AITOWN_dstorage_ctrl_h_INCLUDE
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
#include <aitown/dstorage_cdata.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _dstorage_t;
struct _dstorage_ctrl_t;
struct _dstorage_handle_t;
struct _dstorage_ctrl_param_t;

//! response status
typedef enum {
    DSTORAGE_CTRL_OK,           /**< The request completed succesfully */
    DSTORAGE_CTRL_GENERIC_ERR,  /**< Some kind of error occured */
    DSTORAGE_CTRL_FULL,         /**< No more space is available */
    DSTORAGE_CTRL_DATA_LOST,    /**< The data is forever lost */
    DSTORAGE_CTRL_UNREACHABLE,  /**< The data is momentarly unavailable */
    DSTORAGE_CTRL_CANT_WRITE    /**< Failed to write an ID */
} dstorage_ctrl_sts_t;


//! function type for terminating controllers;
typedef void (*dstorage_ctrl_ending) (struct _dstorage_ctrl_t* ctrl);
//! function type for asking a controller to read an id;
typedef void (*dstorage_ctrl_read) (struct _dstorage_ctrl_param_t* req);
//! function type for asking a controller to write an id;
typedef void (*dstorage_ctrl_write) (struct _dstorage_ctrl_param_t* req);
//! function type for responding to a request
typedef void (*dstorage_ctrl_response) (
        dstorage_ctrl_sts_t sts, struct _dstorage_ctrl_param_t* req);


//! storage space for request parameter
///
/// The requests (read, write, ...) take a pointer to this kind of
/// structure and will pass it
typedef struct _dstorage_ctrl_param_t {
    struct _dstorage_ctrl_t*    ctrl;   /**< pointer to yourself */
    dstorage_cdata_t*           ctrl_data;/**< data associated with this id in the database */
    struct _dstorage_handle_t * handle; /**< the handle to recieve/give info */
    dstorage_ctrl_response      kb;     /**< callback informed of the status */
    // this structure may be extended for other user-defined data
} dstorage_ctrl_param_t;


//! describes a controller
///
/// This is the header structure that must be present for each controller.
/// The implementation may choose to extend this structure with fields
/// appropriate for itself.
///
/// After the storage space was allocated one needs to call
/// dstorage_ctrl_init(). To register/unregister a controller use
/// functions in the dstorage_clist group.
typedef struct _dstorage_ctrl_t {
    struct _dstorage_t*     dstorage;
    dstorage_ctrl_ending    ending;
    dstorage_ctrl_read      read;
    dstorage_ctrl_write     write;
    // this structure may be extended for other user-defined data
} dstorage_ctrl_t;


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


//! initialize a controller structure; this does not register the controller
DSTORAGE_FUNC void
dstorage_ctrl_init (dstorage_ctrl_t *ctrl, struct _dstorage_t *dstorage);

//! terminate the controller; this does not unregister the controller
DSTORAGE_FUNC void
dstorage_ctrl_end (dstorage_ctrl_t *ctrl);


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
#endif // AITOWN_dstorage_ctrl_h_INCLUDE
