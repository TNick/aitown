/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg-leaf.c
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "aitown-cfg.h"

#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

func_error_t aitown_cfg_leaf_init (
        aitown_cfg_t *cfg, aitown_cfg_sect_t *parent,
        const char *name, unsigned name_len,
        aitown_cfg_leaf_t **new_leaf)
{
    DBG_ASSERT (parent != NULL);

    func_error_t ret = FUNC_OK;
    aitown_cfg_node_t * n;
    aitown_cfg_leaf_t * s;
    for (;;) {
        // create this node
        ret = aitown_cfg_node_init (
                    cfg, parent, name, name_len,
                    sizeof(aitown_cfg_leaf_t), &n);
        if (ret != FUNC_OK)
            break;
        s = (aitown_cfg_leaf_t *)n;

        // insert it in the parent section
        n->next = &parent->first_leaf->node;
        parent->first_leaf = s;

        // return the node
        *new_leaf = s;
        break;
    }

    return ret;
}

func_error_t aitown_cfg_leaf_end ( aitown_cfg_leaf_t *leaf )
{
    DBG_ASSERT (leaf != NULL);
    aitown_cfg_node_t * n = (aitown_cfg_node_t*)&leaf->node;
    return aitown_cfg_node_end (&n, sizeof(aitown_cfg_leaf_t));
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


