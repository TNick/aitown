/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-ctrl.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_ctrl_h_INCLUDE
#define AITOWN_dstorage_ctrl_h_INCLUDE
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

//! describes a controller
///
typedef struct _aitown_dstorage_ctrl_t {

    struct _aitown_db_t * db; /**< the database we're wrapping */
    struct _aitown_dstorage_ctrl_t * next; /**< the link in the chain */

} aitown_dstorage_ctrl_t;

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


//! initialize a controller
///
AITOWN_EXPORT func_error_t
aitown_dstorage_ctrl_init (
        struct _aitown_dstorage_t * dstorage,
        const char * database,
        aitown_dstorage_ctrl_t **ctrl);


//! terminate a controller
///
AITOWN_EXPORT void
aitown_dstorage_ctrl_end (
        struct _aitown_dstorage_ctrl_t **ctrl);


//! load a list of controllers from [dstorage/controllers] config section
///
AITOWN_EXPORT func_error_t
aitown_dstorage_ctrl_load_list (
        struct _aitown_dstorage_t * dstorage,
        struct _aitown_cfg_sect_t * sect
        );


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
#endif /* AITOWN_dstorage_ctrl_h_INCLUDE */
