/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-level-II-mng.c
  \date			March 2014
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

#include "dejavu-level-II.h"
#include "dejavu-level-II-mng.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>
#include <aitown/aitown-core.h>

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-read.h>
#include <aitown/aitown-db-write.h>
#include <aitown/aitown-db-free.h>


#include <stdlib.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define AITOWN_DEJAVU_LEVELII_CMPARATOR(x,y) ((x->id)-(y->id))

SGLIB_DEFINE_RBTREE_PROTOTYPES(dejavu_level_II_t, left, right, red, AITOWN_DEJAVU_LEVELII_CMPARATOR)
SGLIB_DEFINE_RBTREE_FUNCTIONS(dejavu_level_II_t, left, right, red, AITOWN_DEJAVU_LEVELII_CMPARATOR)

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

func_error_t dejavu_level_II_mng_init (
        dejavu_level_II_mng_t *mng, aitown_db_mng_t * db_mng,
        aitown_cfg_sect_t * cfg_sect)
{

    func_error_t ret = FUNC_OK;

    for (;;) {

        // initialise
        // memset (mng, 0, sizeof(dejavu_level_II_mng_t));

        // open the database that has color-ID associations
        // we're receiving the section hosting [dejavu] section but
        // the databases are hosted one level higher


        // we're inconsistent, here; we should either have al database
        // names hardcoded or all extracted from relevant section
        // i.e. dejavu/database
        ret = aitown_db_open (db_mng, "dejavu_level1", &mng->db);
        if (ret != FUNC_OK) {
            err_message ("DejaVu sensor: can't open database");
            return ret;
        }


        break;
    }

    return ret;
}

void dejavu_level_II_mng_end (dejavu_level_II_mng_t *mng)
{

    // close the database
    if (mng->db != NULL) {
        aitown_db_close (&mng->db);
    }

    // clear all
    memset (mng, 0, sizeof(dejavu_level_II_mng_t));
}

func_error_t dejavu_level_II_mng_alloc (
        dejavu_level_II_mng_t* mng, aitown_dstorage_id_t id, dejavu_level_II_t ** out)
{

    func_error_t ret = FUNC_OK;
    dejavu_level_II_t * it = NULL;

    for (;;) {

        if (mng->free_level_II == NULL) {
            DBG_ASSERT(mng->free_level_II_count == 0);
            it = (dejavu_level_II_t*) malloc (sizeof(dejavu_level_II_t));
        } else {
            DBG_ASSERT(mng->free_level_II_count > 0);
            it = mng->free_level_II;
            mng->free_level_II = it->right;
            --mng->free_level_II_count;
        }
        if (it == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // reset fields
        memset (it, 0, sizeof(dejavu_level_II_t));
        it->id = id;

        break;
    }

    *out = it;
    return ret;
}

void dejavu_level_II_mng_free (
        dejavu_level_II_mng_t *mng, dejavu_level_II_t ** it_ptr)
{
    dejavu_level_II_t * it = *it_ptr;

    sglib_dejavu_level_II_t_delete (&mng->tree_level_II, it);
    it->right = mng->free_level_II;
    ++mng->free_level_II_count;
    mng->free_level_II = it;
}

void dejavu_level_II_mng_free_all (
        dejavu_level_II_mng_t *mng)
{
    struct sglib_dejavu_level_II_t_iterator itr;
    dejavu_level_II_t * iter_next;
    dejavu_level_II_t * iter =
            sglib_dejavu_level_II_t_it_init(&itr, mng->tree_level_II);
    while (iter != NULL) {
        iter_next = sglib_dejavu_level_II_t_it_next(&itr);

        iter->right = mng->free_level_II;
        ++mng->free_level_II_count;
        mng->free_level_II = iter;

        iter = iter_next;
    }
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
