/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-lku.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_lku_h_INCLUDE
#define AITOWN_dstorage_lku_h_INCLUDE
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

struct _aitown_db_t;
struct _aitown_dstorage_t;
struct _aitown_cfg_sect_t;
struct _aitown_dstorage_handle_t;


//! describes a look-up structure
///
typedef struct _aitown_dstorage_lku_t {

    struct _aitown_db_t *               db; /**< the id database */
    struct _aitown_dstorage_handle_t *  tree; /**< or map of id-handle pairs */

} aitown_dstorage_lku_t;

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


//! initialize a look-up structure
///
AITOWN_EXPORT func_error_t
aitown_dstorage_lku_init (
        struct _aitown_dstorage_t * dstorage,
        struct _aitown_dstorage_lku_t *lku,
        struct _aitown_cfg_sect_t * sect_dstorage);


//! terminate a look-up structure
///
AITOWN_EXPORT void
aitown_dstorage_lku_end (
        struct _aitown_dstorage_lku_t *lku);


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
#endif /* AITOWN_dstorage_lku_h_INCLUDE */
