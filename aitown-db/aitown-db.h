/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_db_h_INCLUDE
#define AITOWN_db_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_db_driver_t;


//! describes a db instance
///
typedef struct _aitown_db_t {
    struct _aitown_db_driver_t *    driver; /**< the driver that manages this database */

} aitown_db_t;

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
aitown_db_init (
        aitown_db_t *db);


//! terminate a db structure
///
/// @warning This method is intended for driver writers and should not be
/// called by the database users.
///
AITOWN_EXPORT void
aitown_db_end (
        aitown_db_t *db);





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
#endif /* AITOWN_db_h_INCLUDE */
