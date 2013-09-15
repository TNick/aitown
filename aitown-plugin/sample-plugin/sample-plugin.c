/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			sample-plugin.c
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

#include "sample-plugin.h"
#include <aitown/utils.h>
#include <aitown/utils_unused.h>
#include <aitown/dbg_assert.h>

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

static const char * sample_data = "Sample data";

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

func_error_t * plugin__initialize ( 
plugin_manager_t *manager_, plugin_data_t *plugin_ )
{
	VAR_UNUSED (manager_);
	plugin_->user_data = (void*)sample_data;
	dbg_message ("Hello from inside sample-plugin; init called");
	return FUNC_OK;
}

void plugin__terminate ( 
plugin_manager_t *manager_, plugin_data_t *plugin_ )
{
	VAR_UNUSED (manager_);
	VAR_UNUSED (plugin_);
	DBG_ASSERT (plugin_->user_data == (void*)sample_data);
	dbg_message ("sample-plugin is being unloaded");	
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
