/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			server_data.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_server_data_h_INCLUDE
#define AITOWN_server_data_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/sglib.h>
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

//! describes a server that we know about
typedef struct _server_data_t {
     const char * name;
     const char * address;
     int port;
     struct _server_data_t *previous, *next;
} server_data_t;

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

//! create a new server structure
extern index_error
server_data_new (server_data_t** server_data_,const char * name_,
    const char * address, int port);

//! delete server structure
extern index_error
server_data_delete (server_data_t** server_data_);

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
#endif /* AITOWN_server_data_h_INCLUDE */
