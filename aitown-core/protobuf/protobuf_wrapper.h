/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			protobuf_wrapper.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_protobuf_wrapper_h_INCLUDE
#define AITOWN_protobuf_wrapper_h_INCLUDE
#ifdef __cplusplus
extern "C" {
#endif 
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "aitown_messages.pb-c.h"
#include <aitown/aitown_global.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

// these methods exist because we want to make use of the protobuf serializer
// in a shared library

//! pack a buffer into an array
AITOWN_EXPORT size_t
AiTownIndex__pack (const AiTownIndex *message, void ** buffer_out);

//! unpack a message to a structure
AITOWN_EXPORT AiTownIndex *
AiTownIndex__unpack (size_t len, const uint8_t *data);

//! free previously unpacked data           
AITOWN_EXPORT void
AiTownIndex__free_unpacked (AiTownIndex *message );

//! prepare an add structure (for index server)
AITOWN_EXPORT void
AiTownIndexAdd__init (AiTownIndexAdd *message );


//! pack a buffer into an array
AITOWN_EXPORT size_t
AiTownIndexReply__pack (const AiTownIndexReply *message, void ** buffer_out);

//! unpack a message to a structure
AITOWN_EXPORT AiTownIndexReply *
AiTownIndexReply__unpack (size_t len, const uint8_t *data);

//! free previously unpacked data           
AITOWN_EXPORT void
AiTownIndexReply__free_unpacked (AiTownIndexReply *message );



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
#endif // AITOWN_protobuf_wrapper_h_INCLUDE
