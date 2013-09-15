/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_definition.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_definition_h_INCLUDE
#define AITOWN_plugin_definition_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/plugin_manager.h>

#ifdef __cplusplus
extern "C" {
#endif 

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

#if defined AITOWN_WIN32
#       define PLUGIN_EXPORT __declspec(dllexport)
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define PLUGIN_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define PLUGIN_EXPORT __attribute__ ((visibility("default")))
#   else
#       define PLUGIN_EXPORT
#   endif
#endif

PLUGIN_EXPORT func_error_t *
plugin__initialize (
    plugin_manager_t *manager_,
    plugin_data_t *plugin_
);

PLUGIN_EXPORT void 
plugin__terminate (
    plugin_manager_t *manager_,
    plugin_data_t *plugin_
);

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
#endif /* AITOWN_plugin_definition_h_INCLUDE */
