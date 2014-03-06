/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-level-I-mng.h
  \date			March 2014
  \author		TNick



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dejavu_level_I_mng_h_INCLUDE
#define AITOWN_dejavu_level_I_mng_h_INCLUDE
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
struct _aitown_cfg_sect_t;

//! describes a level I ID instance
///
typedef struct _dejavu_level_I_mng_t {

} dejavu_level_I_mng_t;

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


//! initialize
AITOWN_EXPORT func_error_t
dejavu_level_I_mng_init (
        struct _dejavu_level_I_mng_t *id_level_I,
        struct _aitown_db_mng_t * db_mng,
        struct _aitown_cfg_sect_t * cfg_sect);


//! terminate
///
/// @param dejavu   address of the structure to terminate
///
AITOWN_EXPORT void
dejavu_level_I_mng_end (
        dejavu_level_I_mng_t *id_level_I);


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
#endif /* AITOWN_dejavu_level_I_mng_h_INCLUDE */
