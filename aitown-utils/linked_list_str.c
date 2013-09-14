/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			linked_list_str.c
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

#include "linked_list_str.h"
#include <aitown/sglib.h>
#include <string.h>
#include <stdlib.h>

#include <aitown/dbg_assert.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

// will define functions for working with double-linked list:
//	sglib_type_add
//	sglib_type_add_before
//	sglib_type_add_after
//	sglib_type_add_if_not_member
//	sglib_type_add_before_if_not_member
//	sglib_type_add_after_if_not_member
//	sglib_type_concat
//	sglib_type_delete
//	sglib_type_delete_if_member
//	sglib_type_find_member
//	sglib_type_is_member
//	sglib_type_get_first
//	sglib_type_get_last
//	sglib_type_len
//	sglib_type_sort
//	sglib_type_reverse
//	sglib_type_it_init
//	sglib_type_it_init_on_equal
//	sglib_type_it_next 
#define LLSTR_COMPARATOR(e1, e2) \
	strcmp(linked_list_string(e1), linked_list_string(e2))
SGLIB_DEFINE_DL_LIST_PROTOTYPES(linked_list_str_t,LLSTR_COMPARATOR,previous,next)
SGLIB_DEFINE_DL_LIST_FUNCTIONS(linked_list_str_t,LLSTR_COMPARATOR,previous,next)

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

linked_list_str_t* linked_list_str_append (linked_list_str_t **out, const char *src, size_t sz)
{
	linked_list_str_t * ret_val;
	linked_list_str_new (&ret_val, src, sz);
	if ( ret_val == NULL )
		return NULL;
	if ( *out == NULL ) {
		sglib_linked_list_str_t_add(out,ret_val);
	} else {
		linked_list_str_t * iter = *out;
		linked_list_str_t * iter_next = iter->next;
		while ( iter_next != NULL ) {
			iter = iter_next;
			iter_next = iter->next;
		}
		sglib_linked_list_str_t_add_after(&iter,ret_val);
	}
	return ret_val;
}

linked_list_str_t* linked_list_str_prepend (linked_list_str_t **out, const char *src, size_t sz)
{
	linked_list_str_t * ret_val;
	linked_list_str_new (&ret_val, src, sz);
	if ( ret_val == NULL )
		return NULL;
	sglib_linked_list_str_t_add_before(out,ret_val);
	return ret_val;
}

void linked_list_str_new (linked_list_str_t **out, const char *src, size_t sz)
{
	linked_list_str_t * ret_val = NULL;
	char * str_ret;
	*out = (linked_list_str_t *)NULL;
	
	// adjust the size
	if ( sz == 0 ) {
		sz = strlen (src);
	}
	size_t sz_tot = sizeof(linked_list_str_t) + sz + 1;
	sz_tot = sz_tot + ROUND_TO_PTR(sz_tot);
	
	// allocate a chunk large enough
	ret_val = (linked_list_str_t*)malloc (sz_tot);
	if ( ret_val == NULL ) {
		DBG_ASSERT (0);
		return;
	}
	
	str_ret = linked_list_string (ret_val);
	*out = ret_val;
	
	// fill in the structure
	ret_val->next = NULL;
	ret_val->previous = NULL;
	memcpy (str_ret, src, sz);
	str_ret[sz] = 0;
	
}

void linked_list_str_delete (linked_list_str_t **first, linked_list_str_t **to_del)
{
	sglib_linked_list_str_t_delete(first, *to_del);
	free (*to_del);
	*to_del = NULL;
}

void linked_list_str_delete_all (linked_list_str_t **first)
{
	linked_list_str_t * iter = *first;
	if ( iter != NULL ) {
		linked_list_str_t * iter_next;
		while ( iter_next != NULL ) {
			iter_next = iter->next;
			free (iter);
			iter = iter_next;
		}
	}
	*first = NULL;
}

func_error_t linked_list_str_foreach ( linked_list_str_t *first_,
	linked_list_str_foreach_t kb_, void *user_data_)
{
	linked_list_str_t *itr_crt;
	struct sglib_linked_list_str_t_iterator itr;
	
	itr_crt = sglib_linked_list_str_t_it_init (&itr, first_);
	while (itr_crt!=NULL) {
		func_error_t i = kb_ (linked_list_string(itr_crt), user_data_);
		if ( i != FUNC_OK )
			return i;
		itr_crt = sglib_linked_list_str_t_it_next(&itr);
	}
	return FUNC_OK;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
