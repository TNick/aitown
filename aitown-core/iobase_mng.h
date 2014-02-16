/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
 \file			iobase_mng.h
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
#include <aitown/sglib.h>

#include <string.h>
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

#define IOBASE_MNG_DEFINE_PROTOTYPES(__name__) \
\
struct _core_ ## __name__ ## _t; \
\
typedef struct _core_ ## __name__ ## _mng_t { \
    struct _core_ ## __name__ ## _t*  list; \
} core_ ## __name__ ## _mng_t; \
\
typedef func_error_t (*core_ ## __name__ ## _mng_foreach_t) (core_ ## __name__ ## _mng_t *, struct _core_ ## __name__ ## _t *, void *); \
\
ATCORE_FUNC void \
core_ ## __name__ ## _mng_init (core_ ## __name__ ## _mng_t * __name__ ## _mng); \
\
ATCORE_FUNC void \
core_ ## __name__ ## _mng_end (core_ ## __name__ ## _mng_t * __name__ ## _mng); \
\
ATCORE_FUNC func_error_t \
core_ ## __name__ ## _mng_add_0 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, const char *name, \
        const char *description, struct _core_ ## __name__ ## _t ** out); \
\
ATCORE_FUNC func_error_t \
core_ ## __name__ ## _mng_add_1 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, const char *name, \
        const char *description, unsigned width, struct _core_ ## __name__ ## _t ** out); \
\
ATCORE_FUNC func_error_t \
core_ ## __name__ ## _mng_add_2 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, const char *name, \
        const char *description, unsigned *sizes, struct _core_ ## __name__ ## _t ** out); \
\
AITOWN_EXPORT func_error_t \
core_ ## __name__ ## _mng_rem ( \
        core_ ## __name__ ## _mng_t *mng, \
        struct _core_ ## __name__ ## _t ** s); \
\
AITOWN_EXPORT func_error_t \
core_ ## __name__ ## _mng_enable ( \
        core_ ## __name__ ## _mng_t *mng, \
        struct _core_ ## __name__ ## _t * s, \
        unsigned enabled); \
\
AITOWN_EXPORT func_error_t \
core_ ## __name__ ## _mng_foreach ( \
        core_ ## __name__ ## _mng_t *mng, \
        core_ ## __name__ ## _mng_foreach_t kb, \
        void * user); \
\
AITOWN_EXPORT struct _core_ ## __name__ ## _t * \
core_ ## __name__ ## _mng_byname ( \
        core_ ## __name__ ## _mng_t *mng, \
        const char *name); \
\
AITOWN_EXPORT struct _core_ ## __name__ ## _t * \
core_ ## __name__ ## _mng_bydescr ( \
        core_ ## __name__ ## _mng_t *mng, \
        const char *descr); \


#define IOBASE_MNG_DEFINE_FUNCTIONS(__name__) \
\
void core_ ## __name__ ## _mng_init (core_ ## __name__ ## _mng_t * __name__ ## _mng) \
{ \
    memset ( __name__ ## _mng, 0, sizeof(core_ ## __name__ ## _mng_t)); \
} \
\
void core_ ## __name__ ## _mng_end (core_ ## __name__ ## _mng_t * __name__ ## _mng) \
{ \
    core_ ## __name__ ## _t *s = __name__ ## _mng->list; \
    core_ ## __name__ ## _t *s_next; \
    while (s != NULL) { \
        s_next = (core_ ## __name__ ## _t*)s->next; \
        core_ ## __name__ ## _end (&s); \
        s = s_next; \
    } \
    memset ( __name__ ## _mng, 0, sizeof(core_ ## __name__ ## _mng_t)); \
} \
\
static inline void core_ ## __name__ ## _mng_add (core_ ## __name__ ## _mng_t * __name__ ## _mng, core_ ## __name__ ## _t *s) \
{ \
    sglib_core_ ## __name__ ## _t_add (& __name__ ## _mng->list, s); \
} \
\
func_error_t core_ ## __name__ ## _mng_add_0 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, \
        const char *name, const char *description, \
        core_ ## __name__ ## _t ** out) \
{ \
    DBG_ASSERT ( __name__ ## _mng != NULL); \
    DBG_ASSERT (out != NULL); \
    func_error_t ret = core_ ## __name__ ## _init( \
                sizeof(core_ ## __name__ ## _0_t), name, description, (core_ ## __name__ ## _t**)out); \
    if (ret!= FUNC_OK) { \
        return ret; \
    } \
    core_ ## __name__ ## _mng_add ( __name__ ## _mng, *out); \
    return ret; \
} \
\
func_error_t core_ ## __name__ ## _mng_add_1 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, const char *name, const char *description, \
        unsigned width, core_ ## __name__ ## _t ** out) \
{ \
    DBG_ASSERT ( __name__ ## _mng != NULL); \
    DBG_ASSERT (out != NULL); \
    func_error_t ret = core_ ## __name__ ## _init( \
                sizeof(core_ ## __name__ ## _1_t), name, description, (core_ ## __name__ ## _t**)out); \
    if (ret!= FUNC_OK) { \
        return ret; \
    } \
    core_ ## __name__ ## _1_t * s = (core_ ## __name__ ## _1_t *)*out; \
    s->width = width; \
    core_ ## __name__ ## _mng_add ( __name__ ## _mng, *out); \
    return ret; \
} \
\
func_error_t core_ ## __name__ ## _mng_add_2 ( \
        core_ ## __name__ ## _mng_t * __name__ ## _mng, const char *name, const char *description, \
        unsigned *sizes, core_ ## __name__ ## _t ** out) \
{ \
    DBG_ASSERT ( __name__ ## _mng != NULL); \
    DBG_ASSERT (out != NULL); \
    func_error_t ret = core_ ## __name__ ## _init( \
                sizeof(core_ ## __name__ ## _2_t), name, description, (core_ ## __name__ ## _t**)out); \
    if (ret!= FUNC_OK) { \
        return ret; \
    } \
    core_ ## __name__ ## _2_t * s = (core_ ## __name__ ## _2_t *)*out; \
    s->width = sizes[0]; \
    s->height = sizes[1]; \
    core_ ## __name__ ## _mng_add ( __name__ ## _mng, *out); \
    return ret; \
} \
\
func_error_t core_ ## __name__ ## _mng_rem ( \
        core_ ## __name__ ## _mng_t *mng, core_ ## __name__ ## _t ** s) \
{ \
    DBG_ASSERT (sglib_core_ ## __name__ ## _t_is_member (mng->list, *s)); \
    sglib_core_ ## __name__ ## _t_delete(&mng->list,*s); \
    core_ ## __name__ ## _end (s); \
    return FUNC_OK; \
} \
\
func_error_t core_ ## __name__ ## _mng_enable ( \
        core_ ## __name__ ## _mng_t *mng, core_ ## __name__ ## _t * s, unsigned enabled) \
{ \
    DBG_ASSERT (sglib_core_ ## __name__ ## _t_is_member (mng->list, s)); \
    s->enabled = enabled; \
    return FUNC_OK; \
} \
\
func_error_t core_ ## __name__ ## _mng_foreach ( \
        core_ ## __name__ ## _mng_t *mng, \
        core_ ## __name__ ## _mng_foreach_t kb, \
        void * user) \
{ \
    core_ ## __name__ ## _t *s = mng->list; \
    func_error_t ret = FUNC_OK; \
    while (s != NULL) { \
        ret = kb (mng, s, user); \
        if (ret != FUNC_OK) break; \
        s = s->next; \
    } \
    return ret; \
} \
\
core_ ## __name__ ## _t * core_ ## __name__ ## _mng_byname ( \
        core_ ## __name__ ## _mng_t *mng, \
        const char *name) \
{ \
    core_ ## __name__ ## _t *s = mng->list; \
    while (s != NULL) { \
        if (!strcmp(s->name, name)) \
            return s; \
        s = s->next; \
    } \
    return NULL; \
} \
\
core_ ## __name__ ## _t * core_ ## __name__ ## _mng_bydescr ( \
        core_ ## __name__ ## _mng_t *mng, \
        const char *descr) \
{ \
    core_ ## __name__ ## _t *s = mng->list; \
    while (s != NULL) { \
        if (!strcmp(s->description, descr)) \
            return s; \
        s = s->next; \
    } \
    return NULL; \
} \


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
