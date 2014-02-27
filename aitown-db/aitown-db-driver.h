/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-driver.h
  \date			February 2014
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

/** @name aitown-db drivers
 *
 * Drivers are back-ends that interpret high level instructions from
 * aitown-db into database-specific instructions.
 *
 */
///@{

/** @todo reference-count the driver; each database increases the count */


#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_db_t;
struct _aitown_db_open_t;
struct _aitown_db_mng_t;

//! callback used by the manager to open a database
///
typedef  func_error_t   (*aitown_db_driver_kb_open_t) (
        struct _aitown_db_open_t*data);

//! callback used by the manager to close a database
///
typedef  func_error_t   (*aitown_db_driver_kb_close_t) (
        struct _aitown_db_t*db);

//! callback used by the manager to write to a database
///
typedef  func_error_t   (*aitown_db_driver_kb_write_t) (
        struct _aitown_db_write_t * request);

//! callback used by the manager to read from a database
///
typedef  func_error_t   (*aitown_db_driver_kb_read_t) (
        struct _aitown_db_read_t * request);

//! callback used by the manager to free a chunk of memory from a database
///
typedef  func_error_t   (*aitown_db_driver_kb_free_t) (
        struct _aitown_db_t*db,
        void * chunk
        );



//! describes a database driver instance
///
/// Actual drivers will inherit from this structure
typedef struct _aitown_db_driver_t {

    const char *                        name;
    struct _aitown_db_driver_t *        next;
    int                                 ref_count;

    /* all drivers must implement following callbacks */
    aitown_db_driver_kb_open_t          open;
    aitown_db_driver_kb_close_t         close;
    aitown_db_driver_kb_read_t          read;
    aitown_db_driver_kb_write_t         write;
    aitown_db_driver_kb_free_t          free_chunk;

    /* next callbacks are optional */
    aitown_db_driver_kb_read_t          read_k64;
    aitown_db_driver_kb_write_t         write_k64;

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


//! add a driver to manager
///
/// The pointer becomes part of the internal list of drivers and should not
/// be freed before calling aitown_db_driver_rem()
///
/// Also, note that build in types are registered as part of the initialization
/// process and should not be registered again.
///
/// If a driver with same name is already registered, the function fails. Call
/// aitown_db_driver_rem() first, then add the driver. The function
/// returns FUNC_BAD_INPUT in this case.
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT func_error_t
aitown_db_driver_add (
        struct _aitown_db_mng_t *    db_mng,     /**< the manager; must be non-NULL */
        struct _aitown_db_driver_t * db_driver   /**< the driver to add */
        );

//! remove a driver from manager
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT func_error_t
aitown_db_driver_rem (
        struct _aitown_db_mng_t *    db_mng,     /**< the manager; must be non-NULL */
        struct _aitown_db_driver_t * db_driver   /**< the driver to remove */
        );

//! remove a driver from manager by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT func_error_t
aitown_db_driver_rem_n (
        struct _aitown_db_mng_t *    db_mng,     /**< the manager; must be non-NULL */
        const char *                 driver_name,/**< the driver to remove */
        struct _aitown_db_driver_t ** instance /**< the instance that was removed */
        );



//! find a driver by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_db_driver_find (
        struct _aitown_db_mng_t *       db_mng,     /**< the manager; must be non-NULL */
        const char *                    driver_name,/**< the driver to remove */
        struct _aitown_db_driver_t **   db_driver   /**< the driver that was found */
        );


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

///@}

#ifdef __cplusplus
}
#endif
#endif /* AITOWN_db_driver_h_INCLUDE */
