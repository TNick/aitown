/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			utils.c
  \date			September 2013
  \author		TNick
  
  \brief		Entry point for the application
  
  
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

#include "utils.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

void
z_sleep (int msecs)
{
#if (defined (__WINDOWS__))
    Sleep (msecs);
#else
    struct timespec t;
    t.tv_sec  =  msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
#endif
}

int64_t
z_clock (void)
{
#if (defined (__WINDOWS__))
    SYSTEMTIME st;
    GetSystemTime (&st);
    return (int64_t) st.wSecond * 1000 + st.wMilliseconds;
#else
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

void
log_message (const char *format, ...)
{
    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char formatted[20];
    strftime (formatted, 20, "%y-%m-%d %H:%M:%S ", loctime);
    printf ("%s", formatted);

    va_list argptr;
    va_start (argptr, format);
    vprintf (format, argptr);
    va_end (argptr);
    printf ("\n");
}

void
err_message (const char *format, ...)
{
    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char formatted[64];
    strftime (formatted, 64, "%y-%m-%d %H:%M:%S ERROR! ", loctime);
    fprintf (stderr, "%s", formatted);

    va_list argptr;
    va_start (argptr, format);
    vfprintf (stderr, format, argptr);
    va_end (argptr);
    fprintf (stderr, "\n");
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
