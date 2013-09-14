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
	size_t to_alloc = sizeof(void*) + init_size+ROUND_TO_PTR(init_size);
	accum->allocated = 0;
	if ( to_alloc > 0 ) {
		accum->data = malloc (to_alloc);
		if ( accum->data != NULL )
			accum->allocated = to_alloc;
	}
	accum->used = sizeof(void*);
	accum->step = to_alloc;
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

offset_t accumulator_alloc (accumulator_t* accum, size_t sz )
{
	void *ret_ptr = accum->data;
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
		size_t to_alloc = sizeof(void*) + sz + accum->step;
		accum->data = malloc(to_alloc);
		if ( accum->data == NULL )
			return ACCUMULATOR_BAD_OFFSET;
		ret_ptr = PTR_ADD(accum->data,sizeof(void*));
		
		// save new state
		accum->used = sizeof(void*) + sz;
		accum->allocated = to_alloc;
		
	} else {
		DBG_ASSERT(accum->allocated > 0);
		
		// check to see if there is space left
		new_size = accum->used + sz;
		if ( new_size > accum->allocated ) {
		
			// create a buffer large enough
			size_t new_alloc = accum->allocated + new_size + accum->step;
			void * tmp_ptr = realloc (accum->data, new_alloc);
			if ( tmp_ptr == NULL )
				return ACCUMULATOR_BAD_OFFSET;
			
			// update the state
			accum->data = tmp_ptr;
			accum->allocated = new_alloc;
		}
		
		// actual allocation
		ret_ptr = PTR_ADD(accum->data,accum->used);
		accum->used = new_size;
	}
	return PTR_OFF(accum->data, ret_ptr);
}

offset_t accumulator_add_string (
		accumulator_t* accum, const char * src, size_t sz )
{
	char * ret_ptr = NULL;
	offset_t ret_off = ACCUMULATOR_BAD_OFFSET;
	
	// get the lenght
	if ( sz == 0 ) {
		sz = strlen (src);
	}
	
	// get a buffer and copy "text" inside
	ret_off = accumulator_alloc (accum, sz+1);
	if ( ret_off == ACCUMULATOR_BAD_OFFSET )
		return ret_off;
	ret_ptr = accumulator_to_char (accum, ret_off);
	memcpy(ret_ptr, src, sz);
	ret_ptr[sz] = 0;
	
	return ret_off;
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
