/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_h_INCLUDE
#define AITOWN_dstorage_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/struct_ini.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-dstorage-lku.h>

struct _aitown_db_mng_t;
struct _aitown_dstorage_ctrl_t;

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a dstorage instance
///
/// One such structure holds data about an instance of a dstorage. There may
/// be multiple, independent instances in the same program. It is the main
/// interface for the module and the methods associated with it are the
/// public API interface.
///
typedef struct _aitown_dstorage_t {

    struct _aitown_db_mng_t             db_mng; /**< database manager */
    struct _aitown_dstorage_lku_t       lku; /**< the database for the ids */

    struct _aitown_dstorage_ctrl_t *    first_ctrl; /**< first controller in the chain */
    struct _aitown_dstorage_ctrl_t *    last_ctrl; /**< last controller in the chain */

    struct _aitown_cfg_sect_t *         cfg_sect; /**< configuration section */
    int                                 manage_cfg; /**< do we save the config at the end or not */

} aitown_dstorage_t;

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


//! initialize a dstorage structure
///
/// Before using a dstorage_t instance one needs to initialise it using a
/// call to an init method. It initialises sub-components based on the settings
/// provided by config section. This process includes loading controllers
/// (and the plug-ins that provide them, if any). User should call
/// dstorage_end() when dstorage_t instance is no longer needed.
///
AITOWN_EXPORT func_error_t
aitown_dstorage_init (
        struct _aitown_dstorage_t * dstorage,
        struct _aitown_cfg_sect_t * cfg_sect);


//! initialize a dstorage structure
///
/// Before using a dstorage_t instance one needs to initialise it using a
/// call to an init method. It initialises sub-components based on the settings
/// provided by config file. This process includes loading controllers
/// (and the plug-ins that provide them, if any). User should call
/// dstorage_end() when dstorage_t instance is no longer needed.
///
AITOWN_EXPORT func_error_t
aitown_dstorage_finit (
        struct _aitown_dstorage_t * dstorage,
        const char * file);


//! terminate a dstorage structure
///
/// Various subcomponents are terminated and all memory is freed.
AITOWN_EXPORT void
aitown_dstorage_end (
        aitown_dstorage_t * dstorage);



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
#endif /* AITOWN_dstorage_h_INCLUDE */
