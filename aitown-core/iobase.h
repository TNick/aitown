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
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/core_func.h>
#include <aitown/dbg_assert.h>
#include <aitown/pointer_aritmetic.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define IOBASE_DEFINE_PROTOTYPES(__name__) \
\
struct _core_ ## __name__ ## _t; \
struct _aitown_core_t; \
\
typedef void (*core_ ## __name__ ## _stream_t) ( \
        struct _aitown_core_t*, struct _core_ ## __name__ ## _t*, const void *); \
\
typedef struct _core_ ## __name__ ## _t { \
    struct _core_ ## __name__ ## _t*  next; \
    const char *            name; \
    const char *            description; \
    core_ ## __name__ ## _stream_t    kb; \
    unsigned                enabled; \
    unsigned                dimensions; \
} core_ ## __name__ ## _t; \
\
typedef struct _core_ ## __name__ ## _0_t { \
    core_ ## __name__ ## _t           header; \
} core_ ## __name__ ## _0_t; \
\
typedef struct _core_ ## __name__ ## _1_t { \
    core_ ## __name__ ## _t           header; \
    unsigned                width; \
} core_ ## __name__ ## _1_t; \
\
typedef struct _core_ ## __name__ ## _2_t { \
    core_ ## __name__ ## _t           header; \
    unsigned                width; \
    unsigned                height; \
} core_ ## __name__ ## _2_t; \
\
ATCORE_FUNC func_error_t \
core_ ## __name__ ## _init (\
        unsigned sz, \
        const char *name, \
        const char *description, \
        core_ ## __name__ ## _t ** __name__ ); \
 \
ATCORE_FUNC void \
core_ ## __name__ ## _end (core_ ## __name__ ## _t ** __name__ );


#define IOBASE_DEFINE_FUNCTIONS(__name__) \
\
func_error_t core_ ## __name__ ## _init ( \
        unsigned sz, const char *name, const char *description, \
        core_ ## __name__ ## _t ** __name__ ) \
{ \
    DBG_ASSERT ( __name__ != NULL); \
\
    unsigned sz_name = (name == NULL ? 0 : strlen (name)); \
    unsigned sz_descr = (name == NULL ? 0 : strlen (description)); \
\
    unsigned full_size = sz + sz_name + 1 + sz_descr + 1; \
    core_ ## __name__ ## _t * ret = (core_ ## __name__ ## _t*)malloc (full_size); \
    if (ret == NULL) { \
        return FUNC_MEMORY_ERROR; \
    } \
\
    memset (ret, 0, sz); \
    ret->enabled = 1; \
\
    ret->name = PTR_ADD(ret, sz); \
    ret->description = PTR_ADD(ret->name, sz_name+1); \
    if (name != NULL) memcpy((char*)ret->name, name, sz_name); \
    *(char*)PTR_ADD(ret->name, sz_name) = 0; \
    if (description != NULL) memcpy((char*)ret->description, description, sz_descr); \
    *(char*)PTR_ADD(ret->description, sz_descr) = 0; \
\
    * __name__  = ret; \
    return FUNC_OK; \
} \
\
void core_ ## __name__ ## _end (core_ ## __name__ ## _t ** __name__ ) \
{ \
    memset (* __name__ , 0, sizeof(core_ ## __name__ ## _t)); \
    free ((char*)* __name__ ); \
    * __name__  = NULL; \
}



/*  DEFINITIONS    ========================================================= */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
