/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file            index_data.h
  \date            September 2013
  \author        TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_index_data_h_INCLUDE
#define AITOWN_index_data_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "server_data.h"
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
typedef struct _index_data_t {
    int server_count;
    server_data_t *first;
    int port;
    int max_serv;
} index_data_t;

//! the version of the protocol to use when sending messages
#define INDEX_CURRENT_PROTOCOL_VERSION 1

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

//! initialise a pre-allocated index structure
extern index_error
index_data_init (index_data_t *index_data_);
//! terminate a pre-allocated index structure
extern void
index_data_end (index_data_t *index_data_);

//! add a server data structure to an index
/// asserts that the server structure is not part of the list
extern void
index_data_add_server (
    index_data_t *index_data_, server_data_t *server_data_);
//! remove a server data structure from an index
/// asserts that the server structure is part of the list
extern void
index_data_rem_server (
    index_data_t *index_data_, server_data_t *server_data_);

//! find a server by its name
/// @return NULL if not found
extern server_data_t *
index_data_get_server (index_data_t *index_data_, const char * name_);

//! foreach callback type
/// @return 0 to continue, anything else to break
typedef int (*index_data_foreach_t) (index_data_t *, server_data_t*, void*);

//! foreach server call a function
/// @return code returned by callback if non-zero, 0 if all returned 0
int index_data_foreach_server (
    index_data_t *index_data_, index_data_foreach_t kb, void * user_data_);

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
#endif // AITOWN_index_data_h_INCLUDE
