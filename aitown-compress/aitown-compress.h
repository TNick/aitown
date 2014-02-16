/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-compress.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_compress_h_INCLUDE
#define AITOWN_compress_h_INCLUDE
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

struct _aitown_compress_driver_t;


//! describes a compress instance
///
typedef struct _aitown_compress_t {
    struct _aitown_compress_driver_t *    driver; /**< the driver that manages this compress */

} aitown_compress_t;

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
/// @warning This method is intended for driver writers and should not be
/// called by the compress users.
///
AITOWN_EXPORT void
aitown_compress_init (
        aitown_compress_t *compress);


//! terminate a compress structure
///
/// @warning This method is intended for driver writers and should not be
/// called by the compress users.
///
AITOWN_EXPORT void
aitown_compress_end (
        aitown_compress_t *compress);





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
#endif /* AITOWN_compress_h_INCLUDE */
