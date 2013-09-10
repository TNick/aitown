/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			index_data.c
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

#include "index_data.h"
#include "string.h"
#include <aitown/aitown_global.h>
#include <aitown/utils.h>

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
#define SERVER_COMPARATOR(e1, e2) strcmp(e1->name, e2->name)
SGLIB_DEFINE_DL_LIST_PROTOTYPES(server_data_t,SERVER_COMPARATOR,previous,next)
SGLIB_DEFINE_DL_LIST_FUNCTIONS(server_data_t,SERVER_COMPARATOR,previous,next)

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

index_error index_data_init (index_data_t *index_data_)
{ dbg_message (__func__);
	
	// clear all fields to zero
	memset (index_data_, 0, sizeof(index_data_t));
	
	// good to go
	return INDEX_OK;
}

void index_data_end (index_data_t *index_data_)
{ dbg_message (__func__);

	// delete all server data instances
	server_data_t *itr_crt;
	server_data_t *itr_next;
	struct sglib_server_data_t_iterator itr;
	itr_crt = sglib_server_data_t_it_init (&itr, index_data_->first);
	while (itr_crt!=NULL) {
		itr_next = sglib_server_data_t_it_next (&itr);
		server_data_delete (&itr_crt);
		itr_crt = itr_next;
	}
	
	// clear all fields to zero
	memset (index_data_, 0, sizeof(index_data_t));
}

void index_data_add_server (
    index_data_t *index_data_, server_data_t *server_data_)
{ dbg_message (__func__);

	// add the item in the list
	INDEX_ASSERT( sglib_server_data_t_is_member(
	    index_data_->first, server_data_) == 0 );
	sglib_server_data_t_add_before(
	    &index_data_->first, server_data_);
	index_data_->server_count++;
}

void index_data_rem_server (
    index_data_t *index_data_, server_data_t *server_data_)
{ dbg_message (__func__);

	// remove the item from the list
	INDEX_ASSERT( sglib_server_data_t_is_member(
	    index_data_->first, server_data_) != 0 );
	sglib_server_data_t_delete(
	    &index_data_->first, server_data_);
	index_data_->server_count--;
}

server_data_t * index_data_get_server (
    index_data_t *index_data_, const char * name_)
{
	server_data_t *itr_crt;
	struct sglib_server_data_t_iterator itr;
	
	itr_crt = sglib_server_data_t_it_init (&itr, index_data_->first);
	while (itr_crt!=NULL) {
		if ( strcmp(name_,itr_crt->name) == 0 ) {
			return itr_crt;
		}
		itr_crt = sglib_server_data_t_it_next(&itr);
	}
	return NULL;
}

int index_data_foreach_server (
    index_data_t *index_data_, index_data_foreach_t kb, void * user_data_)
{
	server_data_t *itr_crt;
	struct sglib_server_data_t_iterator itr;
	
	itr_crt = sglib_server_data_t_it_init (&itr, index_data_->first);
	while (itr_crt!=NULL) {
		int i = kb (index_data_, itr_crt, user_data_);
		if ( i != 0 )
			return i;
		itr_crt = sglib_server_data_t_it_next(&itr);
	}
	return 0;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
