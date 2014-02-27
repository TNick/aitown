/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-open.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_open_h_INCLUDE
#define AITOWN_db_open_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Opening aitown-db databases
 *
 * Here is an example config file:
 *
 * @code
 *     [databases]
 *     ; generic configuration for all databases
 *
 *     [databases/db1]
 *
 *     ; name is used in a driver-specific way
 *     ; (kyoto will use it as  abase name for file, mysql will use
 *     ; is as database name)
 *
 *     ; configuration for a certain database managed by "mydriver"
 *     driver = mydriver
 *
 *     ; some drivers may need paths for the file (eq kyoto), others
 *     ; will ignore this parameter
 *     path_hint = "/tmp"
 *
 *     ; geometry of the database (some drivers may optimize particular cases)
 *     ; an infinite lenght is denoted by an empty value or the key missing
 *     key_len = 64
 *     value_len = 128
 *
 *
 *     [databases/db2]
 *
 *     ; configuration for another database managed by "mydriver"
 *     driver = mydriver
 *     path_hint =
 *     key_len =
 *     value_len =
 * @endcode
 */
///@{

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_db_mng_t;


//! various flags for databases
///
typedef enum _aitown_db_flags_t {
    AITOWN_DB_FLAGS_NONE = 0 /**< no flag is set */
} aitown_db_flags_t;


//! describes parameters for opening a database
///
typedef struct _aitown_db_open_t {
    struct _aitown_db_mng_t *   db_mng; /**< the manager; must be non-NULL */
    unsigned            flags; /**< switches */

    unsigned            key_len; /**< 0 means variable lenght */
    unsigned            value_len; /**< 0 means variable lenght */

    const char *        driver; /**< name of a previously registered driver */
    const char *        db_name; /**< name of the database */
    const char *        path_hint; /**< may or may not be used, depending on driver */

    struct _aitown_db_t** out; /**< the database handle */
} aitown_db_open_t;


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


//! open a database
///
/// Options in config file decide what driver to use and what characteristics
/// should have.
///
AITOWN_EXPORT func_error_t
aitown_db_open (
        struct _aitown_db_mng_t * db_mng,
        const char * db_id,
        struct _aitown_db_t ** out);


//! open a database while enforcing some characteristics
///
/// This functions allows overriding settings in config file.
///
AITOWN_EXPORT func_error_t
aitown_db_open_ovr (aitown_db_open_t * params);


//! prepare to open a database
///
/// prepares the structure with defaults (except arguments)
///
static inline void
aitown_db_preopen (
        struct _aitown_db_open_t * data,
        struct _aitown_db_mng_t * db_mng,
        const char * db_id,
        struct _aitown_db_t ** out) {

    memset (data, 0, sizeof(struct _aitown_db_open_t));
    data->db_mng = db_mng;
    data->db_name = db_id;
    data->out = out;
}


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
#endif /* AITOWN_db_open_h_INCLUDE */
