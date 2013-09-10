/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file            aiserver_data.h
  \date            September 2013
  \author        TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_aiserver_data_h_INCLUDE
#define AITOWN_aiserver_data_h_INCLUDE
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
typedef struct _aiserver_data_t {
	void *context; // zmq contenxt
    
    // where to register
    const char *index_server_address;
    int index_server_port;
	int was_registered;
    
    // our info
	const char *name;
	const char *address;
	int port;

} aiserver_data_t;

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

//! initialise a pre-allocated aiserver structure
extern aiserver_error
aiserver_data_init (aiserver_data_t *aiserver_data_);

//! terminate a pre-allocated aiserver structure
extern void
aiserver_data_end (aiserver_data_t *aiserver_data_);

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
#endif // AITOWN_aiserver_data_h_INCLUDE
