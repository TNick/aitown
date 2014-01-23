/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_clist.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_clist_h_INCLUDE
#define AITOWN_dstorage_clist_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/utils.h>
#include <aitown/error_codes.h>
#include <aitown/pivotal_gmtime.h>
#include <aitown/dstorage_id.h>
#include <aitown/dstorage_func.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _dstorage_t;
struct _dstorage_ctrl_t;

//! describes a controller list structure
///
///
typedef struct _dstorage_clist_t {
    void *                      array;      /**< the dynamic array */
    size_t                      allocated;  /**< the size of the array */
    size_t                      used;       /**< top used in array */

    struct _dstorage_ctrl_t *   priority;   /**< controllers sorted by priority */
    int                         pri_dirty;  /**< the cost changed and list should be sortred */
} dstorage_clist_t;

struct _struct_ini_t;

//! iterator for the list; return 0 to stop
typedef int (*dstorage_clist_iterator) (struct _dstorage_ctrl_t* itr, void* user);

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

//! initialize a controller list structure;
DSTORAGE_FUNC void
dstorage_clist_init (struct _dstorage_t * dstorage);

//! terminate the controller list structure;
DSTORAGE_FUNC void
dstorage_clist_end (dstorage_clist_t *clist);

//! iterate in each controller
DSTORAGE_FUNC int
dstorage_clist_foreach (
        dstorage_clist_t *clist,
        dstorage_clist_iterator kb,
        void* user);

//! get the controller at specified index
DSTORAGE_FUNC struct _dstorage_ctrl_t*
dstorage_clist_get (
        dstorage_clist_t *clist,
        unsigned index);

//! get best controller
DSTORAGE_FUNC struct _dstorage_ctrl_t*
dstorage_clist_get_best (
        dstorage_clist_t *clist);

//! get the index of a controller or -1 if not found
DSTORAGE_FUNC int
dstorage_clist_index (
        dstorage_clist_t *clist,
        struct _dstorage_ctrl_t *controller
        );


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
#endif // AITOWN_dstorage_clist_h_INCLUDE
