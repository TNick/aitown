/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			iobase.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_core_iobase_h_INCLUDE
#define AITOWN_core_iobase_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/core_func.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _core_iobase_t;
struct _aitown_core_t;

//! the callback used to communicate data in and out
///
typedef void (*core_iobase_stream_t) (
        struct _aitown_core_t*, struct _core_iobase_t*, void *);

//! base for sensors and actuators
///
typedef struct _core_iobase_t {
    struct _core_iobase_t*  next;           /**< internal use for linked list */
    const char *            name;           /**< the name of this one (part of the chunk */
    const char *            description;    /**< the description of this one (part of the chunk */
    core_iobase_stream_t    kb;             /**< the callback used to send data */
    unsigned                enabled;        /**< set to 0 to disable */
    unsigned                dimensions;     /**< the dimensionality of data */
} core_iobase_t;

//! base for a stream of numbers
///
typedef struct _core_iobase_0_t {
    core_iobase_t           header;         /**< common iobase data */
} core_iobase_0_t;

//! base for a stream of vectors
///
typedef struct _core_iobase_1_t {
    core_iobase_t           header;         /**< common iobase data */
    unsigned                width;          /**< numbers in each vector */
} core_iobase_1_t;

//! base for a stream of matrices
///
typedef struct _core_iobase_2_t {
    core_iobase_t           header;         /**< common iobase data */
    unsigned                width;          /**< numbers in each vector */
    unsigned                height;         /**< vectors in each matrix */
} core_iobase_2_t;

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

//! initialize a iobase
ATCORE_FUNC func_error_t
core_iobase_init (
        unsigned sz,
        const char *name,
        const char *description,
        core_iobase_t **iobase);


//! terminate a iobase
ATCORE_FUNC void
core_iobase_end (core_iobase_t **iobase);

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
#endif // AITOWN_core_iobase_h_INCLUDE
