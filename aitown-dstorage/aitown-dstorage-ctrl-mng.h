/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-ctrl-mng.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_ctrl_mng_h_INCLUDE
#define AITOWN_dstorage_ctrl_mng_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/aitown-db-mng.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_cfg_sect_t;
struct _aitown_dstorage_controller_t;

//! manages the list of controllers
///
typedef struct _aitown_dstorage_ctrl_mng_t {

    struct _aitown_dstorage_controller_t *  ctrl_list; /**< the list of controllers */
    unsigned                                count; /**< number of controllers in the list */

    struct _aitown_dstorage_controller_t *  priority; /**< pointer to the chain of controllers in the order of their speed */
    unsigned                                priority_dirty; /**< the list should be rescaned when used */

} aitown_dstorage_ctrl_mng_t;

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


//! initialize a list of controllers
///
/// @param mng              the structure to initialise
/// @param dstorage_sect    settings structure that hosts [dstorage]
/// @param index            the index of this controller in list
///
AITOWN_EXPORT func_error_t
aitown_dstorage_ctrl_mng_init (
        struct _aitown_db_mng_t * dbmng,
        struct _aitown_dstorage_ctrl_mng_t * mng,
        struct _aitown_cfg_sect_t * dstorage_sect);


//! terminate a list of controllers
///
/// @param mng              the structure to terminate
/// @param dstorage_sect    settings structure that hosts [dstorage]
///
AITOWN_EXPORT void
aitown_dstorage_ctrl_mng_end (
        struct _aitown_dstorage_ctrl_mng_t *mng,
        struct _aitown_cfg_sect_t * dstorage_sect);


//! sort controllers in list by their performance
AITOWN_EXPORT void
aitown_dstorage_ctrl_mng_sort (
        struct _aitown_dstorage_ctrl_mng_t *mng);


//! get best controller to use or NULL if none
AITOWN_EXPORT struct _aitown_dstorage_controller_t *
aitown_dstorage_ctrl_mng_best (
        struct _aitown_dstorage_ctrl_mng_t *mng);


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
#endif /* AITOWN_dstorage_ctrl_mng_h_INCLUDE */
