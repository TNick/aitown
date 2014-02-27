/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-read.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_read_h_INCLUDE
#define AITOWN_db_read_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>

#include <stddef.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Reading from aitown-db databases
 *
 *
 *
 */
///@{

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_db_t;
struct _aitown_db_mng_t;

//! callback used with asyncronous reads to inform about the status
///
/// The reciever takes ownership of the pointer and should use
/// aitown_db_free() on it at some point.
///
typedef void            (*aitown_db_readkb_t)   (
        struct _aitown_db_t*    db,     /**< the database that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data,   /**< user data */
        const void *            value,  /**< the data to use as value */
        unsigned                value_sz /**< size of the data to use as value */
        );


//! data structures used in compact calls to read
typedef struct _aitown_db_read_t {

    struct _aitown_db_t*    db; /**< the database to write to; must be non-NULL  */

    const void *            key; /**< the data to use as key */
    size_t                  key_sz; /**< size of the data to use as key */

    const void **           val; /**< receives the data to use as value */
    size_t *                val_sz; /**< receives the size of the data to use as value */

    aitown_db_readkb_t      kb; /**< if non-NULL, the call is asyncronous */
    void *                  user; /**< user data to be passed along */

} aitown_db_read_t;


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


//! locate a key, get its string value and return it
///
AITOWN_EXPORT const char *
aitown_db_read (
        struct _aitown_db_t *db,
        const char * key);


//! locate a key, get its string value and return it
///
AITOWN_EXPORT const void *
aitown_db_read_v (
        struct _aitown_db_t *db,
        const void * key, size_t key_len);


//! extended read function
///
AITOWN_EXPORT func_error_t
aitown_db_read_ex (
        struct _aitown_db_read_t *params);

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
#endif /* AITOWN_db_read_h_INCLUDE */
