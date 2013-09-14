/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			accumulator.c
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

#include "accumulator.h"
#include "utils_null.h"
#include "dbg_assert.h"
#include "pointer_aritmetic.h"

#include <string.h>
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

void accumulator_init (accumulator_t* accum, size_t init_size)
{
	// allocate an initial ammount of memory
	accum->allocated = 0;
	if ( init_size > 0 ) {
		accum->data = malloc (init_size);
		if ( accum->data != NULL )
			accum->allocated = init_size;
	}
	accum->used = 0;
	accum->step = init_size;
}

void accumulator_end (accumulator_t* accum)
{
	// release the memory, if there is any
	if ( accum->data != NULL )
		free (accum->data);
	accum->data = NULL;
	accum->used = 0;
	accum->allocated = 0;
	accum->step = 0;	
}

void * accumulator_alloc (accumulator_t* accum, size_t sz )
{
	void *retptr = NULL;
	int new_size;
	
	// round to next word
	if ( ( sizeof(void*) == 4 ) || ( sizeof(void*) == 8 ) ) {
		sz = sz + ROUND_TO_PTR(sz);
	}
	
	// default to this size if nothing else was provided
	if ( accum->step == 0 )
		accum->step = sz * 8;
	
	// either initial allocation or re-allocation
	if ( accum->data == NULL ) {
		DBG_ASSERT(accum->allocated == 0);
		DBG_ASSERT(accum->used == 0);
		
		// attempt to allocate initial pointer
		accum->data = malloc(sz + accum->step);
		if ( accum->data == NULL )
			return NULL;
		retptr = accum->data;
		
		// save new state
		accum->used = sz;
		accum->allocated = sz + accum->step;
		
	} else {
		DBG_ASSERT(accum->allocated > 0);
		
		// check to see if there is space left
		new_size = accum->used + sz;
		if ( new_size > accum->allocated ) {
		
			// create a buffer large enough
			size_t new_alloc = accum->allocated + new_size + accum->step;
			void * tmp_ptr = realloc (accum->data, new_alloc);
			if ( tmp_ptr == NULL )
				return NULL;
			
			// update the state
			accum->data = tmp_ptr;
			accum->allocated = new_alloc;
		}
		
		// actual allocation
		retptr = PTR_ADD(accum->data,accum->used);
		accum->used = new_size;
	}
	return retptr;
}

char * accumulator_add_string (
		accumulator_t* accum, const char * src, size_t sz )
{
	char * ret_ptr = NULL;
	
	// get the lenght
	if ( sz == 0 ) {
		sz = strlen (src);
	}
	
	// get a buffer and copy "text" inside
	ret_ptr = (char*)accumulator_alloc (accum, sz+1);
	if ( ret_ptr == NULL )
		return NULL;
	memcpy(ret_ptr, src, sz);
	ret_ptr[sz] = 0;
	
	return ret_ptr;
}

func_error_t accumulator_free (accumulator_t* accum, size_t sz)
{
	if ( sz > accum->used )
		return FUNC_GENERIC_ERROR;
	accum->used -= sz;
	return FUNC_OK;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
