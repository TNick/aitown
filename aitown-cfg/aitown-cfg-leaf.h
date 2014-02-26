/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg-leaf.h
  \date			September 2013
  \author		TNick



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_cfg_leaf_h_INCLUDE
#define AITOWN_cfg_leaf_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown-cfg-config.h>
#include <aitown/aitown-cfg-node.h>

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

struct _aitown_cfg_t;
struct _aitown_cfg_sect_t;

//! describes a section instance
///
typedef struct _aitown_cfg_leaf_t {
    aitown_cfg_node_t   node;
    const char * value;
} aitown_cfg_leaf_t;

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



//! initialize a leaf
///
/// @param cfg          config file where this leaf belongs
/// @param parent       parent section
/// @param name         name to assign (will be duplicated, owner stays with the caller)
/// @param out          allocated structure
///
func_error_t
aitown_cfg_leaf_init (
        struct _aitown_cfg_t *cfg,
        aitown_cfg_sect_t *parent,
        const char *name,
        unsigned name_len,
        aitown_cfg_leaf_t **out
        );

//! terminate (free) a leaf
///
/// @param leaf  the structure to free
///
func_error_t
aitown_cfg_leaf_end (
        aitown_cfg_leaf_t *leaf
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
#endif /* AITOWN_cfg_leaf_h_INCLUDE */
