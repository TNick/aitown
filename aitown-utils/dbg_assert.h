/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dbg_assert.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef utils_dbg_assert_h_INCLUDE
#define utils_dbg_assert_h_INCLUDE

#include <assert.h>

//! our assert
#ifdef _DEBUG
#  define DBG_ASSERT(a) assert(a)
#else
#  define DBG_ASSERT(a)
#endif

#endif // utils_dbg_assert_h_INCLUDE
