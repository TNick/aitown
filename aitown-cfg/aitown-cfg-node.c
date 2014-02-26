/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg-node.c
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
#include <aitown/pointer_aritmetic.h>

#include <stdlib.h>
#include <string.h>

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


func_error_t aitown_cfg_node_init (
        aitown_cfg_t *cfg, aitown_cfg_sect_t *parent,
        const char *name, unsigned name_len,
        size_t struct_sz, aitown_cfg_node_t **new_node)
{
    DBG_ASSERT (cfg != NULL);
    DBG_ASSERT (parent != NULL);
    DBG_ASSERT (name != NULL); // root structure does not gets allocated here
    DBG_ASSERT (new_node != NULL);
    DBG_ASSERT (struct_sz > sizeof(aitown_cfg_node_t));

    aitown_cfg_node_t * out = NULL;
    func_error_t ret = FUNC_OK;
    for (;;) {

        // comute sizes
        size_t sztot = struct_sz + name_len + 1;

        // allocate a chunk
        out = (aitown_cfg_node_t*)malloc (sztot);
        if (out == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // clear it and set known values
        memset (out, 0, struct_sz);
        out->root = cfg;
        out->parent = parent;
        out->name = (char*)PTR_ADD(out, struct_sz);

        // copy the name and compute hash
        unsigned hash_comp[8] = {1,1,1,1,1,1,1,1};
        int i, j;
        char c;
        char * dest = (char*)out->name;
        for (i=0; i<name_len; ++i) {
            j = i % 8;
            c = name[i];
            hash_comp[j] += c;
            *dest = c;
            dest++;
        }
        *dest = 0;
        out->name_hash =
                hash_comp[0] * hash_comp[1] * hash_comp[2] * hash_comp[3] *
                hash_comp[4] * hash_comp[5] * hash_comp[6] * hash_comp[7];

        break;
    }

    *new_node = out;
    return ret;

}


func_error_t aitown_cfg_node_end ( aitown_cfg_node_t **node, size_t struct_sz )
{
    DBG_ASSERT (node != NULL);
    DBG_ASSERT (struct_sz > sizeof(aitown_cfg_node_t));

    aitown_cfg_node_t *n = *node;
    func_error_t ret = FUNC_OK;
    for (;;) {
        if (n == NULL)
            break;

        char * name_loc = (char*)PTR_ADD(n, struct_sz);
        if (name_loc != n->name) {
            free ((void*)n->name);
        }

        free (n);
        *node = NULL;
        break;
    }
    return ret;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


