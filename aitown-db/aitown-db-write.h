/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-write.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_write_h_INCLUDE
#define AITOWN_db_write_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Writing to aitown-db databases
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
typedef void            (*aitown_db_writekb_t)   (
        struct _aitown_db_t*    db,     /**< the database that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data    /**< user data */
        );

//! data structures used in compact calls to read
typedef struct _aitown_db_write_t {

    struct _aitown_db_t*    db; /**< the database to write to; must be non-NULL  */

    const void *            key; /**< the data to use as key */
    size_t                  key_sz; /**< size of the data to use as key */

    const void *            val; /**< the data to use as value */
    size_t                  val_sz; /**< the size of the data to use as value */

    aitown_db_writekb_t     kb; /**< if non-NULL, the call is asyncronous */
    void *                  user; /**< user data to be passed along */

} aitown_db_write_t;

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


//! write a string key and a string value
///
AITOWN_EXPORT func_error_t
aitown_db_write (
        struct _aitown_db_t *db,
        const char * key,
        const char * value);


//! write a data key and a data value
///
AITOWN_EXPORT func_error_t
aitown_db_write_v (
        struct _aitown_db_t *db,
        const void * key,
        size_t key_len,
        const void * value,
        size_t value_len);


//! extended write function
///
AITOWN_EXPORT func_error_t
aitown_db_write_ex (
        struct _aitown_db_write_t *params);

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
#endif /* AITOWN_db_write_h_INCLUDE */
