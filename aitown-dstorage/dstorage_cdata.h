/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_cdata.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_dstorage_cdata_h_INCLUDE
#define AITOWN_dstorage_cdata_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/dstorage_func.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! the block of bytes stored along with each id in the database managed by
//! dstorage_lookup_t
typedef struct _dstorage_cdata_t {
    char    f1[8];
    char    f2[8];
} dstorage_cdata_t;

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

//! terminate the structure
DSTORAGE_FUNC void
dstorage_cdata_init (dstorage_cdata_t * cdata);

//! terminate the structure
DSTORAGE_FUNC void
dstorage_cdata_end (dstorage_cdata_t * cdata);

//! get the index of the controller
#define dstorage_cdata_ctrl(cd) (*((uint32_t*)(&(cd)->f1[0])))

//! set the index of the controller
#define dstorage_cdata_ctrl_set(cd,v)   dstorage_cdata_ctrl(cd) = (v)


//! get the first 32-bit value
#define dstorage_cdata_1(cd) (*((uint32_t*)(&(cd)->f1[4])))

//! set the first 32-bit value
#define dstorage_cdata_1_set(cd,v)   dstorage_cdata_1(cd) = (v)

//! get the second 32-bit value
#define dstorage_cdata_2(cd) (*((uint32_t*)(&(cd)->f2[0])))

//! set the second 32-bit value
#define dstorage_cdata_2_set(cd,v)   dstorage_cdata_2(cd) = (v)

//! get the third 32-bit value
#define dstorage_cdata_3(cd) (*((uint32_t*)(&(cd)->f2[4])))

//! set the third 32-bit value
#define dstorage_cdata_3_set(cd,v)   dstorage_cdata_3(cd) = (v)

//! get the 64-bit value (2 and 3 combined)
#define dstorage_cdata_64(cd) (*((uint64_t*)(&(cd)->f2[0])))

//! set the 64-bit value (2 and 3 combined)
#define dstorage_cdata_64_set(cd,v)   dstorage_cdata_64(cd) = (v)




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
#endif // AITOWN_dstorage_cdata_h_INCLUDE
