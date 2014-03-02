/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-controller.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_controller_h_INCLUDE
#define AITOWN_dstorage_controller_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
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

struct _aitown_cfg_sect_t;
struct _aitown_db_t;
struct _aitown_dstorage_ctrl_mng_t;
struct _aitown_dstorage_handle_t;

//! describes the status a controller can have
///
typedef enum _aitown_dstorage_controller_status_t {
    AITOWN_DSTORAGE_CONTROLLER_STATUS_OK, /**< good to go */
    AITOWN_DSTORAGE_CONTROLLER_STATUS_TMP_UNAVAIL,/**< temporary unavailable */
    AITOWN_DSTORAGE_CONTROLLER_STATUS_FULL,/**< completly filled */
    AITOWN_DSTORAGE_CONTROLLER_STATUS_PERM_REMOVED /**< removed for all eternity */

} aitown_dstorage_controller_status_t;


//! describes a dstorage instance
///
typedef struct _aitown_dstorage_controller_t {

    struct _aitown_db_t *                   db; /**< the database */
    unsigned                                performance; /**< how good is this controller */
    aitown_dstorage_controller_status_t     sts; /**< the status; when changing the status to an something other than OK performance should also be changed */

    struct _aitown_dstorage_controller_t *  next; /**< link for priority list */

} aitown_dstorage_controller_t;

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


//! initialize a dstorage structure with settings from a section, ie [dstorage/controllers/11]
///
/// The section should have following keys:
/// - database (required) - the name of the database to use; config file should
/// contain a key named [databases/dbname]
/// - performance (required) - an integer indicating how good is this controller
/// compared with others
/// - status (optional, defaults to OK) - one of OK, TEMP_ERR or PERM_ERR
/// to indicate that the controller should be loaded, treated as temporary
/// unavailable or lost forever
///
AITOWN_EXPORT func_error_t
aitown_dstorage_controller_init (
        struct _aitown_db_mng_t *mng,
        struct _aitown_dstorage_controller_t * ctrl,
        struct _aitown_cfg_sect_t * cfg_sect,
        unsigned index);


//! terminate a dstorage structure
///
AITOWN_EXPORT void
aitown_dstorage_controller_end (
        struct _aitown_dstorage_controller_t *dstorage,
        struct _aitown_cfg_sect_t * cfg_sect);


//! retreive the content associated with an ID within a handle
///
AITOWN_EXPORT func_error_t
aitown_dstorage_controller_get (
        struct _aitown_dstorage_handle_t *handle);

//! save the content associated with an ID within a handle
///
/// This functionis usually called when a handle is being freed
///
AITOWN_EXPORT func_error_t
aitown_dstorage_controller_set (
        struct _aitown_dstorage_handle_t *handle);


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
#endif /* AITOWN_dstorage_controller_h_INCLUDE */
