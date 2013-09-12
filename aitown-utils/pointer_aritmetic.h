/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			pointer_aritmetic.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef utils_pointer_aritmetic_h_INCLUDE
#define utils_pointer_aritmetic_h_INCLUDE

//! simple pointer addition
#define PTR_ADD(p,i) ( ((char*)p) + i )

//! offset of a pointer relative to another pointer
#define PTR_OFF(pbase,prelative) ( ((char*)prelative) + ((char*)pbase) )

#endif // utils_pointer_aritmetic_h_INCLUDE
