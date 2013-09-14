/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			linked_list_str.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef linked_list_str_h_INCLUDE
#define linked_list_str_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <stddef.h>
#include "pointer_aritmetic.h"
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

//! header structure for a linked list element
typedef struct _linked_list_str_t {
     struct _linked_list_str_t *previous, *next;
} linked_list_str_t;

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

//! get the actual string for an entry
static inline char * linked_list_string (linked_list_str_t *entry)
{ return PTR_ADD(entry,sizeof(linked_list_str_t)); }

//! create a new entry with provided content and append it as last entry
AITOWN_EXPORT linked_list_str_t*
linked_list_str_append (linked_list_str_t **first, const char *src, size_t sz);

//! create a new entry with provided content and make it first entry
AITOWN_EXPORT linked_list_str_t*
linked_list_str_prepend (linked_list_str_t **first, const char *src, size_t sz);

//! create a new entry with provided content
AITOWN_EXPORT void
linked_list_str_new (linked_list_str_t **out, const char *src, size_t sz);

//! remove the entry from the chain and free the memory for this entry
AITOWN_EXPORT void
linked_list_str_delete (linked_list_str_t **first, linked_list_str_t **to_del);

//! frees the memory for each and every entry
AITOWN_EXPORT void
linked_list_str_delete_all (linked_list_str_t **first);


//! the callback informed about each string
/// @return FUNC_OK to continue, anything else to break
typedef func_error_t (*linked_list_str_foreach_t) (
    const char*, void*);

//! inform the callback about each string
AITOWN_EXPORT func_error_t
linked_list_str_foreach (
	linked_list_str_t *first_, 
    linked_list_str_foreach_t kb_, 
    void *user_data_);

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
#endif // linked_list_str_h_INCLUDE
