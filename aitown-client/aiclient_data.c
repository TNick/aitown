/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aiclient_data.c
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

#include "aiclient_data.h"
#include "string.h"
#include <aitown/zmq_wrapper.h>

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

func_error_t aiclient_data_init (aiclient_data_t *aiclient_data_)
{
	// clear all fields to zero
	memset (aiclient_data_, 0, sizeof(aiclient_data_t));
	
	// prepare zmq
	aiclient_data_->context = zmq_ctx_new ();
	if ( aiclient_data_->context == NULL )
		return FUNC_GENERIC_ERROR;

	// good to go
	return FUNC_OK;
}

void aiclient_data_end (aiclient_data_t *aiclient_data_)
{
	// end zmq
	if ( aiclient_data_->context != NULL )
		zmq_ctx_destroy (aiclient_data_->context);

	// clear all fields to zero
	memset (aiclient_data_, 0, sizeof(aiclient_data_t));
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
