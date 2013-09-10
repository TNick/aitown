/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			utils.h
  \date			September 2013
  \author		TNick
    
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_utils_h_INCLUDE
#define AITOWN_utils_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#if !defined _WIN32_WCE
#include <errno.h>
#endif
#include <stddef.h>
#include <stdio.h>
#if defined _WIN32
#include <winsock2.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//!  Provide random number from 0..(num-1)
/// from zhelpers.h
#if (defined (__WINDOWS__))
#   define randof(num)  (int) ((float) (num) * rand () / (RAND_MAX + 1.0))
#else
#   define randof(num)  (int) ((float) (num) * random () / (RAND_MAX + 1.0))
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

//!  Sleep for a number of milliseconds
/// from zhelpers.h
extern void
z_sleep (int msecs);

//!  Return current system clock as milliseconds
/// from zhelpers.h
extern int64_t
z_clock (void);

//!  Print formatted string to stdout, prefixed by date/time and
//!  terminated with a newline.
/// from zhelpers.h
extern void
log_message (const char *format, ...);

//!  Print formatted string to stdout, prefixed by date/time and
//!  terminated with a newline.
/// from zhelpers.h
extern void
err_message (const char *format, ...);

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
#endif // AITOWN_utils_h_INCLUDE
