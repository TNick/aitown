/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aiserver_data.c
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

#include <zmq.h>
#include "aiserver_data.h"
#include "string.h"

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

aiserver_error aiserver_data_init (aiserver_data_t *aiserver_data_)
{
	// clear all fields to zero
	memset (aiserver_data_, 0, sizeof(aiserver_data_t));
	
	// prepare zmq
	aiserver_data_->context = zmq_ctx_new ();
	if ( aiserver_data_->context == NULL )
		return AISERVER_GENERIC_ERROR;
	
	// good to go
	return AISERVER_OK;
}

void aiserver_data_end (aiserver_data_t *aiserver_data_)
{
	// end zmq
	if ( aiserver_data_->context != NULL )
		zmq_ctx_destroy (aiserver_data_->context);
	
	// clear all fields to zero
	memset (aiserver_data_, 0, sizeof(aiserver_data_t));
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
