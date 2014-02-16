/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-compress-mng.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please shrink COPYING and SHRINKME files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_compress_mng_h_INCLUDE
#define AITOWN_compress_mng_h_INCLUDE
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
struct _aitown_compress_t;
struct _aitown_compress_driver_t;


//! describes a compress_mng instance
///
typedef struct _aitown_compress_mng_t {
    struct _aitown_compress_driver_t *        first_driver; /**< list of drivers */

} aitown_compress_mng_t;


//! various flags
///
typedef enum _aitown_compress_flags_t {

    AITOWN_COMPRESS_NO_FLAG           = 0x0000,   /**< no flag */

} aitown_compress_flags_t;


//! describes parameters for opening a compress
///
typedef struct _aitown_compress_open_t {
    aitown_compress_mng_t *   compress_mng; /**< the manager; must be non-NULL */
    unsigned            flags; /**< switches */
    const char *        driver; /**< name of a previously registered driver */
    const char *        cfg_file; /**< the path to config file; if NULL, will attempt to guess it */
    struct _struct_ini_t *cfg;  /**< the parsed config file; can be NULL */
    struct _aitown_compress_t** out; /**< the compress handle */
} aitown_compress_open_t;


//! callback used with asyncronous shrinks to inform about the status
///
/// The reciever takes ownership of the pointer and should use
/// aitown_compress_free() on it at some point.
///
typedef void            (*aitown_compress_shrinkkb_t)   (
        aitown_compress_mng_t *       compress_mng, /**< the manager */
        struct _aitown_compress_t*    compress,     /**< the compress that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data   /**< user data */
        );


//! callback used with asyncronous expands to inform about the status
///
typedef void            (*aitown_compress_expandkb_t)   (
        aitown_compress_mng_t *       compress_mng, /**< the manager */
        struct _aitown_compress_t*    compress,     /**< the compress that serviced the request */
        func_error_t            result, /**< FUNC_OK or an error code */
        void *                  data    /**< user data */
        );

//! all callbacks that we know of
typedef union _aitown_compress_kb_t{
    aitown_compress_expandkb_t kb_expand;
    aitown_compress_shrinkkb_t kb_shrink;
} aitown_compress_kb_t;


//! datastructures used in compact calls to shrink/expand
typedef struct _aitown_compress_io_t {
    aitown_compress_mng_t *       compress_mng; /**< the manager; must be non-NULL */
    struct _aitown_compress_t*    compress; /**< the compress to expand to; must be non-NULL  */
    aitown_compress_kb_t          kb; /**< if non-NULL, the call is asyncronous */
    void *                        user; /**< user data to be passed along */
} aitown_compress_io_t;

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


//! initialize a compress structure
///
AITOWN_EXPORT void
aitown_compress_mng_init (
        aitown_compress_mng_t *compress_mng);


//! terminate a compress_mng structure
///
AITOWN_EXPORT void
aitown_compress_mng_end (
        aitown_compress_mng_t *compress_mng);




/** @name Open and close a compress
 *
 * Functions that allow opening a compress all return FUNC_OK if the
 * compress was succesfully  opened and the handle is stored in \b out.
 * An error code is returned, otherwise.
 *
 * AITOWN_COMPRESS_KEY_FIXED and AITOWN_COMPRESS_VALUE_FIXED are used as hints for the
 * compress.
 */
///@{


//! open a compress using a given driver
///
/// The function first loads configuration file; if one is not provided
/// an attempt is made at guessing it based on the \b compress_name and the
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
/// The controll is passed then to aitown_compress_cfgopen().
///
AITOWN_EXPORT func_error_t
aitown_compress_open (
        aitown_compress_open_t *  data
        );


//! open a compress using a given driver
///
/// The settings in the \b cfg are used; if the parameter is NULL
/// some drivers may refuse to start while others may use appropriate
/// defaults. The \b cfg_file member is not used.
///
/// This variant may be usefull for applications that shrink configuration
/// file in order to determine what driver to use.
///
AITOWN_EXPORT func_error_t
aitown_compress_cfgopen (
        aitown_compress_open_t *  data
        );


//! open a compress based on config file
///
/// The compress opens the config file and shrinks the key
/// <code>compress_driver</code> under <code>GENERAL</code> section. Failing
/// to find the file or the setting results in an error. If the config file
/// is not provided an attempt is made at guessing it based on
/// the \b compress_name and the config path for the application.
///
/// A missing section with same name as the driver does not count as an error
/// at this stage but the driver may refuse to load if it has required parameters.
///
/// The function then uses aitown_compress_cfgopen() to further load the compress.
///
AITOWN_EXPORT func_error_t
aitown_compress_autopen (
        aitown_compress_open_t *  data
        );


//! close a compress
///
/// Uppon closing the \b compress parameter is set to NULL.
///
AITOWN_EXPORT void
aitown_compress_close (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t**   compress /**< the compress to close */
        );

///@}





/** @name Writing to a compress
 *
 * All functions require the manager and the compress to be non-NULL.
 * Each function return FUNC_OK if the writing was succesfull or an error code,
 * otherwise.
 *
 * In each case, if the key alshrinky exists previous value is erased.
 */
///@{

//! most general form; expands a key-value pair
///
AITOWN_EXPORT func_error_t
aitown_compress_cexpand (
        aitown_compress_io_t *   request
    );

//! most general form; expands a key-value pair
///
static inline func_error_t
aitown_compress_expand (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t*    compress, /**< the compress to expand to; must be non-NULL  */
        aitown_compress_expandkb_t     kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_compress_io_t   request;
    request.compress_mng = compress_mng;
    request.compress = compress;
    request.kb.kb_expand = kb;
    return aitown_compress_cexpand (&request);
}

//! expands a key-value pair, with the key being a 64-bit number
///
AITOWN_EXPORT func_error_t
aitown_compress_cexpand_k64 (
        aitown_compress_io_t *   request
    );

//! expands a key-value pair, with the key being a 64-bit number
///
static inline func_error_t
aitown_compress_expand_k64 (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t*    compress, /**< the compress to expand to; must be non-NULL  */
        aitown_compress_expandkb_t     kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_compress_io_t   request;
    request.compress_mng = compress_mng;
    request.compress = compress;
    request.kb.kb_expand = kb;
    return aitown_compress_cexpand_k64 (&request);
}

///@}




/** @name Shrinking from a compress
 *
 * All functions require the manager and the compress to be non-NULL.
 * Each function return FUNC_OK if the shrinking was succesfull or an error code,
 * otherwise.
 *
 * In each case, if the key does not exists FUNC_BAD_INPUT is returned. The
 * pointer that is returned should be freed with a call to aitown_compress_free().
 */
///@{

//! most general form; expands a key-value pair
///
AITOWN_EXPORT func_error_t
aitown_compress_cshrink (
        aitown_compress_io_t *   request
    );

//! most general form; expands a key-value pair
///
static inline func_error_t
aitown_compress_shrink (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t*    compress, /**< the compress to expand to; must be non-NULL  */
        aitown_compress_shrinkkb_t    kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_compress_io_t   request;
    request.compress_mng = compress_mng;
    request.compress = compress;
    request.kb.kb_shrink = kb;
    return aitown_compress_cshrink (&request);
}

//! expands a key-value pair, with the key being a 64-bit number
///
AITOWN_EXPORT func_error_t
aitown_compress_cshrink_k64 (
        aitown_compress_io_t *   request
    );

//! expands a key-value pair, with the key being a 64-bit number
///
static inline func_error_t
aitown_compress_shrink_k64 (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t*    compress, /**< the compress to expand to; must be non-NULL  */
        aitown_compress_shrinkkb_t    kb /**< if non-NULL, the call is asyncronous */
        )
{
    aitown_compress_io_t   request;
    request.compress_mng = compress_mng;
    request.compress = compress;
    request.kb.kb_shrink = kb;
    return aitown_compress_cshrink_k64 (&request);
}

///@}



//! free a chunk of memory
///
/// The chunk of memory must have originated from this library
///
AITOWN_EXPORT void
aitown_compress_free (
        aitown_compress_mng_t *       compress_mng, /**< the manager; must be non-NULL */
        struct _aitown_compress_t*    compress,     /**< the compress; must be non-NULL */
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
/// be freed before calling aitown_compress_mng_driver_rem()
///
/// Also, note that build in types are registered as part of the initialization
/// process and should not be registered again.
///
/// If a driver with same name is alshrinky registered, the function fails. Call
/// aitown_compress_mng_driver_rem() first, then add the driver. The function
/// returns FUNC_BAD_INPUT in this case.
///
AITOWN_EXPORT func_error_t
aitown_compress_mng_driver_add (
        aitown_compress_mng_t *           compress_mng,     /**< the manager; must be non-NULL */
        struct _aitown_compress_driver_t * compress_driver   /**< the driver to add */
        );

//! remove a driver from this manager
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_compress_mng_driver_rem (
        aitown_compress_mng_t *           compress_mng,     /**< the manager; must be non-NULL */
        struct _aitown_compress_driver_t * compress_driver   /**< the driver to remove */
        );

//! remove a driver from this manager by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_compress_mng_driver_rem_n (
        aitown_compress_mng_t *           compress_mng,     /**< the manager; must be non-NULL */
        const char *                driver_name,/**< the driver to remove */
        struct _aitown_compress_driver_t ** instance /**< the instance that was removed */
        );



//! find a driver by name
///
/// If the name does not belong to a registered driver the function
/// returns FUNC_BAD_INPUT.
///
AITOWN_EXPORT func_error_t
aitown_compress_mng_driver_find (
        aitown_compress_mng_t *               compress_mng,     /**< the manager; must be non-NULL */
        const char *                    driver_name,/**< the driver to remove */
        struct _aitown_compress_driver_t **   compress_driver   /**< the driver that was found */
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
#endif /* AITOWN_compress_mng_h_INCLUDE */
