/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg-node.h
  \date			September 2013
  \author		TNick




*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_cfg_node_h_INCLUDE
#define AITOWN_cfg_node_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/aitown-cfg-config.h>
#include <aitown/error_codes.h>

#include <inttypes.h>
#include <stddef.h>

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
struct _aitown_cfg_leaf_t;

//! describes a node instance
///
typedef struct _aitown_cfg_node_t {
    struct _aitown_cfg_t * root; /**< parent config */
    struct _aitown_cfg_sect_t * parent; /**< parent section (NULL for root) */
    const char *name; /**< name of this section */
    unsigned name_hash; /**< name hash for faster look-up */
    struct _aitown_cfg_node_t *next; /**< next in list */

} aitown_cfg_node_t;


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


//! initialize a node
///
/// @param cfg          config file where this node belongs
/// @param parent       parent node
/// @param name         name for this node
/// @param struct_sz    entire size of the inheritant
/// @param out          resulted pointer
///
func_error_t
aitown_cfg_node_init (
        struct _aitown_cfg_t *cfg,
        struct _aitown_cfg_sect_t *parent,
        const char *name,
        unsigned name_len,
        size_t struct_sz,
        aitown_cfg_node_t **out
        );

//! terminate (free) a node
///
/// @param node         the structure to free
/// @param struct_sz    entire size of the inheritant
///
func_error_t
aitown_cfg_node_end (
        aitown_cfg_node_t **node,
        size_t struct_sz
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
#endif /* AITOWN_cfg_node_h_INCLUDE */
