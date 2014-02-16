/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			char_buff.c
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

#include "char_buff.h"
#include "utils_null.h"
#include "dbg_assert.h"
#include "pointer_aritmetic.h"
#include <aitown/error_codes.h>

#include <string.h>
#include <stdlib.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define CHAR_BUFFER_INCREASE_STEP   128

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

void char_buff_init (char_buff_t* cbuff, size_t init_size)
{
	// allocate an initial ammount of memory
	cbuff->allocated = 0;
	if ( init_size > 0 ) {
		cbuff->data = malloc (init_size);
		if ( cbuff->data != NULL ) {
			cbuff->allocated = init_size;
			cbuff->data[0] = 0;
		}
	}
	cbuff->used = 0;
}

func_error_t char_buff_from_str (char_buff_t* cbuff, const char * src)
{
	size_t sz = strlen(src);
	size_t sz_alloc = sz + 64 + ROUND_TO_PTR(sz);

	cbuff->allocated = 0;
	cbuff->used = 0;
	cbuff->data = malloc (sz_alloc);
	if ( cbuff->data != NULL ) {
		cbuff->allocated = sz_alloc;
		cbuff->used = sz;
		memcpy (cbuff->data, src, sz);
		cbuff->data[sz] = 0;
		return FUNC_OK;
	} else {
		return FUNC_MEMORY_ERROR;
	}
}

void char_buff_end (char_buff_t* cbuff)
{
	// release the memory, if there is any
	if ( cbuff->data != NULL )
		free (cbuff->data);
	cbuff->data = NULL;
	cbuff->used = 0;
	cbuff->allocated = 0;
}

func_error_t char_buff_add_string (
		char_buff_t* cbuff, const char * src, size_t sz )
{
	// get the lenght
	if ( sz == 0 ) {
		sz = strlen (src);
	}

    offset_t loc;
    func_error_t ret = char_buff_alloc (cbuff, sz, &loc);
    if (FUNC_OK != ret) {
        return ret;
    }

    // append the string
    char * dest_buf = cbuff->data + loc;
	memcpy (dest_buf, src, sz);
	dest_buf[sz] = 0;
	
	return FUNC_OK;
}

func_error_t char_buff_alloc (char_buff_t* cbuff, size_t sz, offset_t *out )
{
    char * tmp_buf;
    size_t new_used = cbuff->used + sz;

    // enlarge the buffer if required
    if ( new_used >= cbuff->allocated ) {
        size_t new_alloc = new_used + CHAR_BUFFER_INCREASE_STEP + ROUND_TO_PTR(new_used);
        tmp_buf = (char*)realloc(cbuff->data, new_alloc);
        if ( tmp_buf == NULL )
            return FUNC_MEMORY_ERROR;
        cbuff->data = tmp_buf;
        cbuff->allocated = new_alloc;
    }
    *out = cbuff->used;
    cbuff->used = new_used;
    return FUNC_OK;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
