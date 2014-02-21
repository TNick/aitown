/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-driver.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_driver_h_INCLUDE
#define AITOWN_db_driver_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

/** @todo reference-count the driver; each database increases the count */


#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_db_mng_t;
struct _aitown_db_open_t;
struct _aitown_db_io_t;
struct _aitown_db_t;

//! callback used by the manager to open a database
///
typedef  func_error_t   (*aitown_db_driver_kb_open_t) (
        struct _aitown_db_open_t*data);

//! callback used by the manager to close a database
///
typedef  func_error_t   (*aitown_db_driver_kb_close_t) (
        struct _aitown_db_mng_t * db_mng,
        struct _aitown_db_t*db);

//! callback used by the manager to write to a database
///
typedef  func_error_t   (*aitown_db_driver_kb_write_t) (
        struct _aitown_db_io_t * request);

//! callback used by the manager to read from a database
///
typedef  func_error_t   (*aitown_db_driver_kb_read_t) (
        struct _aitown_db_io_t * request);

//! callback used by the manager to free a chunk of memory from a database
///
typedef  func_error_t   (*aitown_db_driver_kb_free_t) (
        struct _aitown_db_mng_t * db_mng,
        struct _aitown_db_t*db,
        void * chunk
        );



//! describes a database driver instance
///
/// Actual drivers will inherit from this structure
typedef struct _aitown_db_driver_t {
    const char *                        name;

    /* all drivers must implement following callbacks */
    aitown_db_driver_kb_open_t          open;
    aitown_db_driver_kb_close_t         close;
    aitown_db_driver_kb_read_t          read;
    aitown_db_driver_kb_write_t         write;
    aitown_db_driver_kb_free_t          free_chunk;

    /* next callbacks are potional */
    aitown_db_driver_kb_read_t          read_k64;
    aitown_db_driver_kb_write_t         write_k64;

    struct _aitown_db_driver_t *        next;

} aitown_db_driver_t;

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


//! initialize a database structure
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_db_driver_init (
        aitown_db_driver_t *db_driver);


//! terminate a db_driver structure
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_db_driver_end (
        aitown_db_driver_t *db_driver);





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
#endif /* AITOWN_db_driver_h_INCLUDE */
