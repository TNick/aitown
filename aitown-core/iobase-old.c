/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			iobase.c
  \date			September 2013
  \author		TNick

  \brief		implementation of an iobase


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

#include "aitown-core.h"
#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>

#include <string.h>
#include <stdlib.h>

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

func_error_t core_iobase_init (
        unsigned sz, const char *name, const char *description,
        core_iobase_t **iobase)
{
    DBG_ASSERT (iobase != NULL);

    // compute the additional size for texts
    unsigned sz_name = (name == NULL ? 0 : strlen (name));
    unsigned sz_descr = (name == NULL ? 0 : strlen (description));

    // compute full size and allocate
    unsigned full_size = sz + sz_name + 1 + sz_descr + 1;
    core_iobase_t * ret = (core_iobase_t*)malloc (full_size);
    if (ret == NULL) {
        return FUNC_MEMORY_ERROR;
    }

    // clear the structure
    memset (ret, 0, sz);
    ret->enabled = 1;

    // compute name/description and copy
    ret->name = PTR_ADD(ret, sz);
    ret->description = PTR_ADD(ret->name, sz_name+1);
    if (name != NULL) memcpy((char*)ret->name, name, sz_name);
    *(char*)PTR_ADD(ret->name, sz_name) = 0;
    if (description != NULL) memcpy((char*)ret->description, description, sz_descr);
    *(char*)PTR_ADD(ret->description, sz_descr) = 0;

    // and that's that
    *iobase = ret;
    return FUNC_OK;
}

void core_iobase_end (core_iobase_t **iobase)
{
    memset (*iobase, 0, sizeof(core_iobase_t));
    free ((char*)*iobase);
    *iobase = NULL;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
