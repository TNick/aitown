/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			server_data.c
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

#include "server_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aitown/error_codes.h>

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

func_error_t server_data_new (server_data_t** server_data_,const char * name_, 
    const char * address_, int port_)
{
	INDEX_ASSERT (address_ != NULL);
	INDEX_ASSERT (port_ > 0);
	
	// allocate and clear the buffer
	server_data_t * ret_inst = malloc (sizeof(server_data_t));
	if (ret_inst==NULL)
		return FUNC_MEMORY_ERROR;
	memset(ret_inst,0,sizeof(server_data_t));
	
	// create a copy of the name, address and port
	if (name_==NULL) {
		int buf_size = strlen(address_) + 32;
		ret_inst->name = (const char*)malloc(buf_size);
		if (ret_inst->name==NULL) {
			free(ret_inst);
			return FUNC_MEMORY_ERROR;
		}
		sprintf ((char*)ret_inst->name, "%s:%d", address_, port_ );
	} else {
		ret_inst->name = strdup (name_);
		if (ret_inst->name==NULL) {
			free(ret_inst);
			return FUNC_MEMORY_ERROR;
		}
	}
	ret_inst->address = strdup (address_);
	if (ret_inst->address==NULL) {
		free ((void*)ret_inst->name);
		free (ret_inst);
		return FUNC_MEMORY_ERROR;
	}
	ret_inst->port = port_;
	
	// we're good
	*server_data_ = ret_inst;
	return FUNC_OK;
}

func_error_t server_data_delete (server_data_t** server_data_)
{
	server_data_t * the_inst = *server_data_;
	INDEX_ASSERT(the_inst != NULL);
	INDEX_ASSERT(the_inst->name != NULL);
	INDEX_ASSERT(the_inst->address != NULL);
	free ((void*)the_inst->name);
	free ((void*)the_inst->address);
	free (the_inst);
	*server_data_ = NULL;
	return FUNC_OK;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
