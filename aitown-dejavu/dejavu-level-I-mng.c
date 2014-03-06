/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dejavu-level-I-mng.c
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

#include "dejavu-level-I.h"
#include "dejavu-level-I-mng.h"

#include <aitown/dbg_assert.h>
#include <aitown/aitown-image.h>
#include <aitown/utils.h>
#include <aitown/aitown-core.h>

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

func_error_t dejavu_level_I_mng_init (
        dejavu_level_I_mng_t *id_level_I_mng, aitown_db_mng_t * db_mng,
        aitown_cfg_sect_t * cfg_sect)
{

    func_error_t ret = FUNC_OK;

    for (;;) {

        // initialise
        memset (id_level_I_mng, 0, sizeof(dejavu_level_I_mng_t));

        break;
    }

    return ret;
}

void dejavu_level_I_mng_end (dejavu_level_I_mng_t *id_level_I)
{


    // clear all
    memset (id_level_I, 0, sizeof(dejavu_level_I_mng_t));
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
