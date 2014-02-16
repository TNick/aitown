/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			char_buff.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_char_buff_h_INCLUDE
#define AITOWN_char_buff_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <stddef.h>
#include <aitown/error_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! header structure for a character buffer (a single string)
/// instead of using pointers, one should use offsets with this structure
typedef struct _char_buff_t {
	char *		data;
	size_t		allocated;
	size_t		used;
} char_buff_t;

#ifndef OFFSET_TYPE_DEFINED
typedef size_t offset_t;
#define OFFSET_TYPE_DEFINED 1
#endif

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

//! initialize the buffer with some memory and empty content
AITOWN_EXPORT void
char_buff_init (char_buff_t* cbuff, size_t init_size);

//! initialize the buffer from a string
AITOWN_EXPORT func_error_t
char_buff_from_str (char_buff_t* cbuff, const char * src);

//! terminate the manager
AITOWN_EXPORT void
char_buff_end (char_buff_t* cbuff);

//! add a string in the char_buff
/// if the size is 0 the lenght is computed
AITOWN_EXPORT func_error_t
char_buff_add_string (char_buff_t* cbuff, const char * src, size_t sz );

//! add a string in the char_buff
/// the lenght is computed by the function
static inline func_error_t
char_buff_add (char_buff_t* cbuff, const char * src ) {
	return char_buff_add_string (cbuff, src, 0);
}

//! allocate space
AITOWN_EXPORT func_error_t
char_buff_alloc (char_buff_t* cbuff, size_t sz, offset_t * out);


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
#endif // AITOWN_char_buff_h_INCLUDE
