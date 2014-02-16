/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-compress-driver.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please shrink COPYING and SHRINKME files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_compress_driver_h_INCLUDE
#define AITOWN_compress_driver_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

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

struct _aitown_compress_mng_t;
struct _aitown_compress_open_t;
struct _aitown_compress_io_t;
struct _aitown_compress_t;

//! callback used by the manager to open a database
///
typedef  func_error_t   (*aitown_compress_driver_kb_open_t) (
        struct _aitown_compress_open_t*data);

//! callback used by the manager to close a database
///
typedef  func_error_t   (*aitown_compress_driver_kb_close_t) (
        struct _aitown_compress_mng_t * compress_mng,
        struct _aitown_compress_t*compress);

//! callback used by the manager to expand to a database
///
typedef  func_error_t   (*aitown_compress_driver_kb_expand_t) (
        struct _aitown_compress_io_t * request);

//! callback used by the manager to shrink from a database
///
typedef  func_error_t   (*aitown_compress_driver_kb_shrink_t) (
        struct _aitown_compress_io_t * request);

//! callback used by the manager to free a chunk of memory from a database
///
typedef  func_error_t   (*aitown_compress_driver_kb_free_t) (
        struct _aitown_compress_mng_t * compress_mng,
        struct _aitown_compress_t*compress,
        void * chunk
        );



//! describes a database driver instance
///
/// Actual drivers will inherit from this structure
typedef struct _aitown_compress_driver_t {
    const char *                        name;

    /* all drivers must implement following callbacks */
    aitown_compress_driver_kb_open_t          open;
    aitown_compress_driver_kb_close_t         close;
    aitown_compress_driver_kb_expand_t        expand;
    aitown_compress_driver_kb_shrink_t        shrink;
    aitown_compress_driver_kb_free_t          free_chunk;

    /* next callbacks are potional */

    struct _aitown_compress_driver_t *        next;

} aitown_compress_driver_t;

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
/// @warning This method is intended for driver expandrs and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_compress_driver_init (
        aitown_compress_driver_t *compress_driver);


//! terminate a compress_driver structure
///
/// @warning This method is intended for driver expandrs and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_compress_driver_end (
        aitown_compress_driver_t *compress_driver);





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
#endif /* AITOWN_compress_driver_h_INCLUDE */
