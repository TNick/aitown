/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_data.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_data_h_INCLUDE
#define AITOWN_plugin_data_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/accumulator.h>

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
typedef struct _plugin_data_t {
     offset_t sign;
     const char *name;
     void *handle;
     void *user_data;
     
     struct _plugin_data_t *previous, *next;
} plugin_data_t;

/*
func_error_t *plugin__initialize (
    plugin_manager_t *manager_,
    plugin_data_t *plugin_
);

void plugin__terminate (
    plugin_manager_t *manager_,
    plugin_data_t *plugin_
); 
*/

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

//! initialize the structure
/// @return FUNC_OK if allocated, error code otherwise
func_error_t
plugin_data_new (plugin_data_t **plugin_sign_, const char * name_, offset_t sign_);

//! free the structure
/// @return FUNC_OK or error code
func_error_t
plugin_data_delete (plugin_data_t **plugin_sign_);

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

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
#endif /* AITOWN_plugin_data_h_INCLUDE */
