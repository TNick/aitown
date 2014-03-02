/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage-ctrl-mng.c
  \date			February 2014
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

#include "aitown-dstorage.h"
#include "aitown-dstorage-ctrl-mng.h"
#include "aitown-dstorage-controller.h"

#include <aitown/aitown-cfg.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils.h>
#include <aitown/sglib.h>


#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define AITOWN_DSTORAGE_CONTROLLER_COMPARATOR(a,b) (a->performance - b->performance)
SGLIB_DEFINE_SORTED_LIST_PROTOTYPES(aitown_dstorage_controller_t, AITOWN_DSTORAGE_CONTROLLER_COMPARATOR, next)
SGLIB_DEFINE_SORTED_LIST_FUNCTIONS(aitown_dstorage_controller_t, AITOWN_DSTORAGE_CONTROLLER_COMPARATOR, next)

static func_error_t
recursive_load_controllers(
        aitown_db_mng_t * dbmng,
        aitown_dstorage_ctrl_mng_t *mng,
        aitown_cfg_sect_t * sect,
        unsigned index);

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


func_error_t aitown_dstorage_ctrl_mng_init (
        aitown_db_mng_t * dbmng, aitown_dstorage_ctrl_mng_t *mng,
        aitown_cfg_sect_t * dstorage_sect)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // we should have been cleared by the caller
        DBG_ASSERT (mng->ctrl_list == NULL);
        DBG_ASSERT (mng->count == 0);
        DBG_ASSERT (mng->priority == NULL);
        DBG_ASSERT (mng->priority_dirty == 0);

        // get the array section
        aitown_cfg_sect_t * sect_controllers = aitown_cfg_get_sect (
                    dstorage_sect, "controllers");
        if (sect_controllers == NULL) {
            err_message ("DStorage [controllers] section missing from config file");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // loop-load them all
        ret = recursive_load_controllers (dbmng, mng, sect_controllers->first_sect, 0);
        if (ret != FUNC_OK) {
            break;
        }

        // sort the list based on provided priorities
        sglib_aitown_dstorage_controller_t_sort (&mng->priority);

        break;
    }


    return ret;
}


void aitown_dstorage_ctrl_mng_end (
        aitown_dstorage_ctrl_mng_t *mng, aitown_cfg_sect_t * dstorage_sect)
{
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (dstorage_sect != NULL);


    if (mng->ctrl_list != NULL) {

        unsigned i;
        unsigned i_max = mng->count;
        char textbuff[64];
        aitown_cfg_sect_t * ctrl_sect = NULL;
        aitown_dstorage_controller_t* ctrl = mng->ctrl_list;

        aitown_cfg_sect_t * sect_controllers = aitown_cfg_get_or_create_sect (
                    dstorage_sect, "controllers");
        for (i = 0; i < i_max; ++i) {
            // the name of the section
            sprintf (textbuff, "%d", i);
            if (sect_controllers != NULL) {
                ctrl_sect = aitown_cfg_get_or_create_sect (
                            sect_controllers, textbuff);
            }
            aitown_dstorage_controller_end (ctrl, ctrl_sect);
            ctrl++;
        }

        // free the list
        free (mng->ctrl_list);
    }

    // will be cleared by the parent
}

void aitown_dstorage_ctrl_mng_sort ( aitown_dstorage_ctrl_mng_t *mng)
{
    DBG_ASSERT (mng != NULL);
    DBG_ASSERT (mng->priority != NULL);
    sglib_aitown_dstorage_controller_t_sort (&mng->priority);
}


static func_error_t recursive_load_controllers(
        aitown_db_mng_t * dbmng, aitown_dstorage_ctrl_mng_t *mng,
        aitown_cfg_sect_t * sect, unsigned index)
{
    DBG_ASSERT (dbmng != NULL);
    DBG_ASSERT (mng != NULL);

    func_error_t ret = FUNC_OK;
    if (sect == NULL) {
        unsigned memsz = mng->count * sizeof(aitown_dstorage_controller_t);
        if (memsz == 0) {
            err_message ("DStorage [controllers] section has no controller");
            return FUNC_BAD_INPUT;
        }

        // allocate the list of controllers and clear it
        DBG_ASSERT (mng->ctrl_list == NULL);
        mng->ctrl_list = (aitown_dstorage_controller_t*)malloc (memsz);
        if (mng->ctrl_list == NULL) {
            return FUNC_MEMORY_ERROR;
        }
        memset (mng->ctrl_list, 0, memsz);

        return ret;
    }

    // first recurse in all controllers to allocate a proper buffer
    ++mng->count;
    ret = recursive_load_controllers (
                dbmng, mng, (aitown_cfg_sect_t *)sect->node.next, index+1);
    if (ret !=FUNC_OK) {
        return ret;
    }

    // the location for this controller
    DBG_ASSERT ((unsigned)atoi (sect->node.name) == index);
    aitown_dstorage_controller_t* ctrl = mng->ctrl_list + index;

    // initialize it
    ret = aitown_dstorage_controller_init (dbmng, ctrl, sect, index);

    // performance list must exist before sorting it
    // last one is null
    if (index+1 < mng->count) {
        ctrl->next = ctrl + 1;
    }

    return ret;
}

aitown_dstorage_controller_t * aitown_dstorage_ctrl_mng_best (
    aitown_dstorage_ctrl_mng_t *mng)
{
    DBG_ASSERT (mng != NULL);

    if (mng->priority_dirty) {
        aitown_dstorage_ctrl_mng_sort (mng);
    }
    aitown_dstorage_controller_t * ret = mng->priority;
    while (ret != NULL) {
        if (ret->sts == AITOWN_DSTORAGE_CONTROLLER_STATUS_OK) {
            return ret;
        }
        ret = ret->next;
    }
    return NULL;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
