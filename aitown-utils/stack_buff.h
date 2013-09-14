/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			stack_buff.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef utils_stack_buff_h_INCLUDE
#define utils_stack_buff_h_INCLUDE

//! if the space is enough then stack buffer is used, otherwise a dynamic one is allocated
#define STACKBUFF_INIT(type, varbase, bufsz, eval_sz) \
	type varbase ## _array[bufsz]; \
	type * varbase ## _ptr = varbase ## _array; \
	int varbase ## _is_dynamic = 0; \
	size_t varbase ## _actual_sz = eval_sz; \
	if ( varbase ## _actual_sz > bufsz ) { \
	    varbase ## _ptr = (type*)malloc (sizeof(type) * varbase ## _actual_sz); \
	    varbase ## _is_dynamic = 1; \
	}

#define STACKBUFF_END(varbase) \
	if ( varbase ## _is_dynamic != 0 ) {\
	    free (varbase ## _ptr); \
	    varbase ## _ptr = NULL; \
	}

#endif // utils_stack_buff_h_INCLUDE
