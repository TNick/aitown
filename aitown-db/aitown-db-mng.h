/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-mng.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_mng_h_INCLUDE
#define AITOWN_db_mng_h_INCLUDE
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

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _struct_ini_t;
struct _aitown_db_t;
struct _aitown_db_driver_t;


//! describes a db_mng instance
///
typedef struct _aitown_db_mng_t {
    struct _aitown_db_driver_t *        first_driver; /**< list of drivers */

} aitown_db_mng_t;


//! various flags
///
typedef enum _aitown_db_flags_t {

    AITOWN_DB_KEY_FIXED         = 0x0001,   /**< the key is fixed in length */
    AITOWN_DB_KEY_VARIABLE      = 0x0000,   /**< the key is variable in length */

    AITOWN_DB_VALUE_FIXED       = 0x0002,   /**< the value is fixed in length */
    AITOWN_DB_VALUE_VARIABLE    = 0x0000,   /**< the value is variable in length */

} aitown_db_flags_t;


//! describes parameters for opening a database
///
typedef struct _aitown_db_open_t {
    aitown_db_mng_t *   db_mng; /**< the manager; must be non-NULL */
    unsigned            flags; /**< switches */
    unsigned            key_len; /**< only if AITOWN_DB_KEY_FIXED */
    unsigned            value_len; /**< only if AITOWN_DB_VALUE_FIXED */
    const char *        driver; /**< name of a previously registered driver */
    const char *        db_name; /**< name of the database */
    const char *        db_path_hint; /**< may or may not be used, depending on driver */
    const char *        cfg_file; /**< the path to config file; if NULL, will attempt to guess it */
    struct _struct_ini_t *cfg;  /**< the parsed config file; can be NULL */
    struct _aitown_db_t** out; /**< the database handle */
} aitown_db_open_t;


//! callback used with asyncronous reads to inform about the status
///
/// The reciever takes ownership of the pointer and should use
/// aitown_db_free() on it at some point.
///
typedef void            (*aitown_db_readkb_t)   (
        aitown_db_mng_t *       db_mng, /**< the manager */
        struct _aitown_db_t*    db,     /**< the database that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data,   /**< user data */
        const void *            value,  /**< the data to use as value */
        unsigned                value_sz /**< size of the data to use as value */
        );


//! callback used with asyncronous writes to inform about the status
///
typedef void            (*aitown_db_writekb_t)   (
        aitown_db_mng_t *       db_mng, /**< the manager */
        struct _aitown_db_t*    db,     /**< the database that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data    /**< user data */
        );

//! all callbacks that we know of
typedef union _aitown_db_kb_t{
    aitown_db_writekb_t kb_write;
    aitown_db_readkb_t kb_read;
} aitown_db_kb_t;

//! various ways to represent a key
typedef union _aitown_db_key_t{
    const void *            ptr;
    uint64_t                u64;
    uint32_t                u32;
    int64_t                 i64;
    int32_t                 i32;
} aitown_db_key_t;

//! values represented as pointer to data or as output pointers
typedef union _aitown_db_value_t{
    const void **           pp;
    const void *            p;
} aitown_db_value_t;

//! value size represented as pointer to data or as data
typedef union _aitown_db_value_size_t{
    unsigned *              pd;
    unsigned                d;
} aitown_db_value_size_t;

//! datastructures used in compact calls to read/write
typedef struct _aitown_db_io_t {
    aitown_db_mng_t *       db_mng; /**< the manager; must be non-NULL */
    struct _aitown_db_t*    db; /**< the database to write to; must be non-NULL  */
    aitown_db_key_t         key; /**< the data to use as key */
    unsigned                key_sz; /**< size of the data to use as key */
    aitown_db_value_t       val; /**< the data to use as value */
    aitown_db_value_size_t  val_sz; /**< size of the data to use as value */
    aitown_db_kb_t          kb; /**< if non-NULL, the call is asyncronous */
    void *                  user; /**< user data to be passed along */
} aitown_db_io_t;

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
AITOWN_EXPORT void
aitown_db_mng_init (
        aitown_db_mng_t *db_mng);


//! terminate a db_mng structure
///
AITOWN_EXPORT void
aitown_db_mng_end (
        aitown_db_mng_t *db_mng);




/** @name Open and close a database
 *
 * Functions that allow opening a database all return FUNC_OK if the
 * database was succesfully  opened and the handle is stored in \b out.
 * An error code is returned, otherwise.
 *
 * AITOWN_DB_KEY_FIXED and AITOWN_DB_VALUE_FIXED are used as hints for the
 * database.
 */
///@{


//! open a database using a given driver
///
/// The function first loads configuration file; if one is not provided
/// an attempt is made at guessing it based on the \b db_name and the
/// config path for the application.
///
/// If a config file is provided but can't be opened the function fails.
/// If a config file was not provided but one was guessed and
/// it was found on disk but could not be opened or parsed the execution
/// continues. The \b cfg member is not used.
///
/// In most cases the section that will be interpreted has same
/// name as the driver.
///
/// The controll is passed then to aitown_db_cfgopen().
///
AITOWN_EXPORT func_error_t
aitown_db_open (
        aitown_db_open_t *  data
        );


//! open a database using a given driver
///
/// The settings in the \b cfg are used; if the parameter is NULL
/// some drivers may refuse to start while others may use appropriate
/// defaults. The \b cfg_file member is not used.
///
/// This variant may be usefull for applications that read configuration
/// file in order to determine what driver to use.
///
AITOWN_EXPORT func_error_t
aitown_db_cfgopen (
        aitown_db_open_t *  data
        );


//! open a database based on config file
///
/// The database opens the config file and reads the key
/// <code>database_driver</code> under <code>GENERAL</code> section. Failing
/// to find the file or the setting results in an error. If the config file
/// is not provided an attempt is made at guessing it based on
/// the \b db_name and the config path for the application.
///
/// A missing section with same name as the driver does not count as an error
/// at this stage but the driver may refuse to load if it has required parameters.
///
/// The function then uses aitown_db_cfgopen() to further load the database.
///
AITOWN_EXPORT func_error_t
aitown_db_autopen (
        aitown_db_open_t *  data
        );


//! close a database
///
/// Uppon closing the \b db parameter is set to NULL.
///
AITOWN_EXPORT void
aitown_db_close (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t**   db /**< the database to close */
        );

///@}





/** @name Writing to a database
 *
 * All functions require the manager and the database to be non-NULL.
 * Each function return FUNC_OK if the writing was succesfull or an error code,
 * otherwise.
 *
 * In each case, if the key already exists previous value is erased.
 */
///@{

//! most general form; writes a key-value pair
///
AITOWN_EXPORT func_error_t
aitown_db_cwrite (
        aitown_db_io_t *   request
    );

//! most general form; writes a key-value pair
///
static inline func_error_t
aitown_db_write (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t*    db, /**< the database to write to; must be non-NULL  */
        const void *            key, /**< the data to use as key */
        unsigned                key_sz, /**< size of the data to use as key */
        const void *            value, /**< the data to use as value */
        unsigned                value_sz, /**< size of the data to use as value */
        aitown_db_writekb_t     kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_db_io_t   request;
    request.db_mng = db_mng;
    request.db = db;
    request.key.ptr = key;
    request.key_sz = key_sz;
    request.val.p = value;
    request.val_sz.d = value_sz;
    request.kb.kb_write = kb;
    return aitown_db_cwrite (&request);
}

//! writes a key-value pair, with the key being a 64-bit number
///
AITOWN_EXPORT func_error_t
aitown_db_cwrite_k64 (
        aitown_db_io_t *   request
    );

//! writes a key-value pair, with the key being a 64-bit number
///
static inline func_error_t
aitown_db_write_k64 (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t*    db, /**< the database to write to; must be non-NULL  */
        uint64_t                key, /**< the data to use as key */
        const void *            value, /**< the data to use as value */
        unsigned                value_sz, /**< size of the data to use as value */
        aitown_db_writekb_t     kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_db_io_t   request;
    request.db_mng = db_mng;
    request.db = db;
    request.key.u32 = key;
    request.val.p = value;
    request.val_sz.d = value_sz;
    request.kb.kb_write = kb;
    return aitown_db_cwrite_k64 (&request);
}

///@}




/** @name Reading from a database
 *
 * All functions require the manager and the database to be non-NULL.
 * Each function return FUNC_OK if the reading was succesfull or an error code,
 * otherwise.
 *
 * In each case, if the key does not exists FUNC_BAD_INPUT is returned. The
 * pointer that is returned should be freed with a call to aitown_db_free().
 */
///@{

//! most general form; writes a key-value pair
///
AITOWN_EXPORT func_error_t
aitown_db_cread (
        aitown_db_io_t *   request
    );

//! most general form; writes a key-value pair
///
static inline func_error_t
aitown_db_read (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t*    db, /**< the database to write to; must be non-NULL  */
        const void *            key, /**< the data to use as key */
        unsigned                key_sz, /**< size of the data to use as key */
        const void **           value, /**< the data to use as value */
        unsigned *              value_sz, /**< size of the data to use as value */
        aitown_db_readkb_t      kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_db_io_t   request;
    request.db_mng = db_mng;
    request.db = db;
    request.key.ptr = key;
    request.key_sz = key_sz;
    request.val.pp = value;
    request.val_sz.pd = value_sz;
    request.kb.kb_read = kb;
    return aitown_db_cread (&request);
}

//! writes a key-value pair, with the key being a 64-bit number
///
AITOWN_EXPORT func_error_t
aitown_db_cread_k64 (
        aitown_db_io_t *   request
    );

//! writes a key-value pair, with the key being a 64-bit number
///
static inline func_error_t
aitown_db_read_k64 (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t*    db, /**< the database to write to; must be non-NULL  */
        uint64_t                key, /**< the data to use as key */
        const void **           value, /**< the data to use as value */
        unsigned *              value_sz, /**< size of the data to use as value */
        aitown_db_readkb_t      kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_db_io_t   request;
    request.db_mng = db_mng;
    request.db = db;
    request.key.u64 = key;
    request.val.pp = value;
    request.val_sz.pd = value_sz;
    request.kb.kb_read = kb;
    return aitown_db_cread_k64 (&request);
}

///@}



//! free a chunk of memory
///
/// The chunk of memory must have originated from this library
///
AITOWN_EXPORT void
aitown_db_free (
        aitown_db_mng_t *       db_mng, /**< the manager; must be non-NULL */
        struct _aitown_db_t*    db,     /**< the database; must be non-NULL */
        const void **           chunk   /**< the chunk to free */
        );



/** @name Drivers
 *
 * @todo
 */
///@{



//! add a driver to this manager
///
/// The pointer becomes part of the internal list of drivers and should not
/// be freed before calling aitown_db_mng_driver_rem()
///
/// Also, note that build in types are registered as part of the initialization
/// process and should not be registered again.
///
/// If a driver with same name is already registered, the function fails. Call
/// aitown_db_mng_driver_rem() first, then add the driver. The function
/// returns FUNC_BAD_INPUT in this case.
///
AITOWN_EXPORT func_error_t
aitown_db_mng_driver_add (
        aitown_db_mng_t *           db_mng,     /**< the manager; must be non-NULL */
        struct _aitown_db_driver_t * db_driver   /**< the driver to add */
        );

//! remove a driver from this manager
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_db_mng_driver_rem (
        aitown_db_mng_t *           db_mng,     /**< the manager; must be non-NULL */
        struct _aitown_db_driver_t * db_driver   /**< the driver to remove */
        );

//! remove a driver from this manager by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_db_mng_driver_rem_n (
        aitown_db_mng_t *           db_mng,     /**< the manager; must be non-NULL */
        const char *                driver_name,/**< the driver to remove */
        struct _aitown_db_driver_t ** instance /**< the instance that was removed */
        );



//! find a driver by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_db_mng_driver_find (
        aitown_db_mng_t *               db_mng,     /**< the manager; must be non-NULL */
        const char *                    driver_name,/**< the driver to remove */
        struct _aitown_db_driver_t **   db_driver   /**< the driver that was found */
        );

///@}


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
#endif /* AITOWN_db_mng_h_INCLUDE */
