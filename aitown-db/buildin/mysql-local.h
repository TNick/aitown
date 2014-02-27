/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			mysql-local.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_mysql_h_INCLUDE
#define AITOWN_db_mysql_h_INCLUDE
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

struct _aitown_db_mng_t;

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
AITOWN_EXPORT func_error_t
aitown_db_mysql_init (
        struct _aitown_db_mng_t *db_mng);


//! terminate a db_mysql structure
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_db_mysql_end (
        struct _aitown_db_mng_t *db_mng);


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
#endif /* AITOWN_db_mysql_h_INCLUDE */
