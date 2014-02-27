/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-mng.h
  \date			February 2014
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
#include <aitown/aitown-cfg.h>

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

struct _aitown_db_t;
struct _aitown_db_driver_t;


//! describes a db_mng instance
///
typedef struct _aitown_db_mng_t {

    struct _aitown_db_driver_t *        first_driver; /**< list of drivers */
    struct _aitown_cfg_sect_t *         cfg_sect; /**< configuration section */
} aitown_db_mng_t;


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


//! initialize a database manager structure with settings from a section
///
AITOWN_EXPORT func_error_t
aitown_db_mng_init (
        aitown_db_mng_t *db_mng,
        struct _aitown_cfg_sect_t * cfg_sect
        );


//! initialize a database manager structure with settings from a config file
///
AITOWN_EXPORT func_error_t
aitown_db_mng_finit (
        aitown_db_mng_t *db_mng,
        const char * file);


//! terminate a database manager structure
///
AITOWN_EXPORT void
aitown_db_mng_end (
        aitown_db_mng_t *db_mng);

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
