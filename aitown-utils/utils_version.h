/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			utils_version.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_utils_version_h_INCLUDE
#define AITOWN_utils_version_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
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

//! identify a version
typedef unsigned long version_t;

#define VERSION_MAJOR_FACTOR		((version_t)0x00100000)
#define VERSION_MINOR_FACTOR		((version_t)0x00001000)

#define VERSION_DEFAULT				((version_t)0x00000000)
#define VERSION_NOT_SPECIFIED		VERSION_DEFAULT

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

//! create a version value from three integers
static inline version_t
version_from_int ( int major, int minor, int patch ) {
	return ((version_t)major)*VERSION_MAJOR_FACTOR + 
	((version_t)minor)*VERSION_MINOR_FACTOR + 
	((version_t)patch);
}

//! create a version value from three integers
static inline version_t
version_from_int_array ( int *array ) {
	return ((version_t)array[0])*VERSION_MAJOR_FACTOR + 
	((version_t)array[1])*VERSION_MINOR_FACTOR + 
	((version_t)array[2]);
}

//! create three integers from a version
static inline void
version_to_int ( version_t ver, int *major, int *minor, int *patch ) {
	*major = ver / VERSION_MAJOR_FACTOR;
	*minor = (ver - *major * VERSION_MAJOR_FACTOR) / VERSION_MINOR_FACTOR;
	*patch = ver - *major * VERSION_MAJOR_FACTOR - *minor * VERSION_MINOR_FACTOR;
}

//! create three integers from a version
static inline void
version_to_int_array ( version_t ver, int *out ) {
	out[0] = ver / VERSION_MAJOR_FACTOR;
	out[1] = (ver - out[0] * VERSION_MAJOR_FACTOR) / VERSION_MINOR_FACTOR;
	out[2] = ver - out[0] * VERSION_MAJOR_FACTOR - out[1] * VERSION_MINOR_FACTOR;
}

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
#endif // AITOWN_utils_version_h_INCLUDE
