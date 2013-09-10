/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			protobuf_wrapper.c
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

#include "protobuf_wrapper.h"
#include <stdlib.h>

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

size_t AiTownIndex__pack ( const AiTownIndex *message, void ** buffer_out )
{
	size_t sz = ai_town_index__get_packed_size (message);
	uint8_t * ret_b = (uint8_t*)malloc (sz);
	if ( ret_b == NULL ) {
		return 0;
	}
	size_t sz_out = ai_town_index__pack (message, ret_b);
	*buffer_out = (void*)ret_b;
	return sz_out;
}

AiTownIndex * AiTownIndex__unpack ( size_t len, const uint8_t *data )
{
	return ai_town_index__unpack (NULL, len, data);
}

void AiTownIndex__free_unpacked ( AiTownIndex *message )
{
	ai_town_index__free_unpacked (message, NULL);
}

size_t AiTownIndexReply__pack ( const AiTownIndexReply *message, void ** buffer_out )
{
	size_t sz = ai_town_index_reply__get_packed_size (message);
	uint8_t * ret_b = (uint8_t*)malloc (sz);
	if ( ret_b == NULL ) {
		return 0;
	}
	size_t sz_out = ai_town_index_reply__pack (message, ret_b);
	*buffer_out = (void*)ret_b;
	return sz_out;
}

AiTownIndexReply * AiTownIndexReply__unpack ( size_t len_, const uint8_t *data_ )
{
	return ai_town_index_reply__unpack (NULL, len_, data_);
}

void AiTownIndexReply__free_unpacked ( AiTownIndexReply *message_ )
{
	ai_town_index_reply__free_unpacked (message_, NULL);
}

void AiTownIndexAdd__init (AiTownIndexAdd * message_)
{
	ai_town_index_add__init (message_);
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
