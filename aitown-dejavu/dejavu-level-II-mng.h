/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-level-II-mng.h
  \date			March 2014
  \author		TNick



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dejavu_level_II_mng_h_INCLUDE
#define AITOWN_dejavu_level_II_mng_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/aitown-dstorage-defs.h>

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
struct _dejavu_level_II_t;

//! describes a level I ID instance
///
typedef struct _dejavu_level_II_mng_t {

    struct _aitown_db_t *       db;         /**< the database that holds our associations */

    struct _dejavu_level_II_t * tree_level_II;
    struct _dejavu_level_II_t * free_level_II;
    unsigned                    free_level_II_count;

} dejavu_level_II_mng_t;

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
func_error_t
dejavu_level_II_mng_init (
        struct _dejavu_level_II_mng_t *mng,
        struct _aitown_db_mng_t * db_mng,
        struct _aitown_cfg_sect_t * cfg_sect);


//! terminate
///
/// @param dejavu   address of the structure to terminate
///
void
dejavu_level_II_mng_end (
        struct _dejavu_level_II_mng_t *mng);


//! allocate a new Level II id structure
func_error_t
dejavu_level_II_mng_alloc (
        struct _dejavu_level_II_mng_t *mng,
        aitown_dstorage_id_t id,
        struct _dejavu_level_II_t ** out);


//! release an existing Level II id structure
void
dejavu_level_II_mng_free (
        struct _dejavu_level_II_mng_t *mng,
        struct _dejavu_level_II_t ** it);


//! release all existing Level II id structures
void
dejavu_level_II_mng_free_all (
        struct _dejavu_level_II_mng_t *mng);



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
#endif /* AITOWN_dejavu_level_II_mng_h_INCLUDE */
