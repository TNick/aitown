/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file            aiclient_data.h
  \date            September 2013
  \author        TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_aiclient_data_h_INCLUDE
#define AITOWN_aiclient_data_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! singleton management structure
/// one such structure exists in each program
typedef struct _aiclient_data_t {
	void *context; // zmq contenxt


} aiclient_data_t;

//! the version of the protocol to use when sending messages
#define AISERVER_CURRENT_PROTOCOL_VERSION 1

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

//! initialise a pre-allocated aiclient structure
extern func_error_t
aiclient_data_init (aiclient_data_t *aiclient_data_);

//! terminate a pre-allocated aiclient structure
extern void
aiclient_data_end (aiclient_data_t *aiclient_data_);

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
#endif // AITOWN_aiclient_data_h_INCLUDE
