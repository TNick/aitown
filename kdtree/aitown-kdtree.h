/* ========================================================================= */
/* ------------------------------------------------------------------------- */

/*
This file originally was  part of ``kdtree'', a library for working with kd-trees.
Copyright (C) 2007-2011 John Tsiombikas <nuclear@member.fsf.org>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

/* single nearest neighbor search written by Tamas Nepusz <tamas@cs.rhul.ac.uk> */
/* converted to "templated" form by Nicu Tofan <nicu.tofan@gmail.com> */

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#ifndef AITOWN_kdtree_h_INCLUDE
#define AITOWN_kdtree_h_INCLUDE
// part that gets included only once
// vvv

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#if defined(WIN32) || defined(__WIN32__)
#include <malloc.h>
#endif



/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


#ifdef KDTREE_DEBUG
#define KDTREE_DEBUG_ASSERT(__val__)    assert(__val__)
#else
#define KDTREE_DEBUG_ASSERT(__val__)
#endif

#ifdef USE_LIST_NODE_ALLOCATOR

#ifndef NO_PTHREADS
#include <pthread.h>
#else

#ifndef I_WANT_THREAD_BUGS
#error "You are compiling with the fast list node allocator, with pthreads disabled! This WILL break if used from multiple threads."
#endif	/* I want thread bugs */

#endif	/* pthread support */
#endif	/* use list node allocator */



//! square a number
#define SQ(x)			((x) * (x))



// we can use faster allocator from the stack, if available
#ifdef USE_LIST_NODE_ALLOCATOR
#   define alloc_resnode(__nm__)		alloc_resnode_func()
#   define free_resnode(__node__)		free_resnode_func(__node__)
#   define list_node_predef(__nm__) \
        static struct res_node_##__nm__ *alloc_resnode_func(void);
        static void free_resnode_func(struct res_node_##__nm__*);

#else
#   define alloc_resnode(__nm__)		(struct res_node_##__nm__*)malloc(sizeof(struct res_node_##__nm__))
#   define free_resnode(__node__)		free(__node__)
#   define list_node_predef(__nm__)
#endif



// pthreads safe or not
#ifndef NO_PTHREADS
#   define  aitown_kdtree_thread_static_mutex \
    static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER
#   define  aitown_kdtree_mutex_lock pthread_mutex_lock(&alloc_mutex)
#   define  aitown_kdtree_mutex_unlock pthread_mutex_unlock(&alloc_mutex)
#else
#   define  aitown_kdtree_thread_static_mutex
#   define  aitown_kdtree_mutex_lock
#   define  aitown_kdtree_mutex_unlock
#endif



#ifdef USE_LIST_NODE_ALLOCATOR
#   define list_node_func(__nm__)
         \
        /* special list node allocators. */ \
        static struct res_node_##__nm__ *free_nodes; \
         \
        aitown_kdtree_thread_static_mutex; \
         \
        static struct res_node_##__nm__ *alloc_resnode_func(void) \
        { \
            struct res_node_##__nm__ *node; \
         \
            aitown_kdtree_mutex_lock; \
         \
            if(!free_nodes) { \
                node = malloc(sizeof *node); \
            } else { \
                node = free_nodes; \
                free_nodes = free_nodes->next; \
                node->next = 0; \
            } \
         \
            aitown_kdtree_mutex_unlock; \
         \
            return node; \
        } \
         \
        static void free_resnode_func(struct res_node_##__nm__ *node) \
        { \
            aitown_kdtree_mutex_lock; \
         \
            node->next = free_nodes; \
            free_nodes = node; \
         \
            aitown_kdtree_mutex_unlock; \
         \
        }

#else
#   define list_node_func(__nm__)
#endif



#ifndef NO_ALLOCA
#   define aitown_kdtree_alloca(__sz__) \
        if(dim <= 256) { \
            bptr = buf = (int*)alloca(__sz__); \
        } else { \
            if(!(bptr = buf = (int*)malloc(__sz__))) { \
                return 0; \
            } \
        }
#   define aitown_kdtree_alloca_limit  256
#else
#   define aitown_kdtree_alloca(__sz__) \
        if(!(bptr = buf = (int*)malloc(__sz__))) { \
            return 0; \
        }
#   define aitown_kdtree_alloca_limit  16
#endif




#ifndef KDTREE_EXPORT
#define KDTREE_EXPORT
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

#ifdef __cplusplus
extern "C" {
#endif

//! defines a kdtree with specified data type and number of dimensions (use in header)
#define KDTREE_STATIC_DEFINE_PROTOTYPES(__nm__, __data_type__, __dims__, __user_space_on_node__) \
    \
struct kdres_##__nm__; \
    \
    \
    \
/** a node in a tree */\
typedef struct _kdtree_node_##__nm__##_t { \
   __data_type__                position[__dims__]; /**< all coordinates */\
   int                          dir; /**< direction */\
   void *                       data; /**< user defined data */\
   struct _kdtree_node_##__nm__##_t *  left, *right;	/**< negative/positive side */ \
} kdtree_node_##__nm__##_t; \
    \
    \
    \
/** A bounding (hyper)box */\
typedef struct _kdtree_rect_##__nm__##_t { \
    __data_type__               minimum[__dims__]; \
    __data_type__               maximum[__dims__]; \
} kdtree_rect_##__nm__##_t; \
    \
    \
    \
/** the callback used when a node gets deleted */\
typedef void (*kdtree_##__nm__##_node_destructor_t) ( \
    struct _kdtree_##__nm__##_t *   tree, /**< the parent tree */\
    kdtree_node_##__nm__##_t *      node  /**< the node to be deleted */\
);  \
    \
    \
    \
/** Represents a kdtree; this is the central structure */\
typedef struct _kdtree_##__nm__##_t { \
   kdtree_node_##__nm__##_t *           root; /**< the root node */\
   kdtree_rect_##__nm__##_t             rect; /**< bounding box */\
   int                                  rect_init; /**< was initialised or not */\
   kdtree_##__nm__##_node_destructor_t  destr; /**< user callback on destroy */\
} kdtree_##__nm__##_t; \
    \
    \
    \
/** create a kd-tree for "k"-dimensional data */ \
KDTREE_EXPORT kdtree_##__nm__##_t * \
kdtree_##__nm__##_create (); \
    \
    \
    \
/** free the struct */ \
KDTREE_EXPORT void \
kdtree_##__nm__##_free ( \
    kdtree_##__nm__##_t *       tree /**< The tree to free; is cleared, first*/\
);  \
    \
    \
    \
/** remove all the elements from the tree */ \
KDTREE_EXPORT void \
kdtree_##__nm__##_clear ( \
    kdtree_##__nm__##_t *       tree /**< The tree to clear*/\
);  \
    \
    \
    \
/** will be called on data pointers (see kdtree_insert) when nodes are removed */ \
KDTREE_EXPORT void \
kdtree_##__nm__##_data_destructor ( \
    kdtree_##__nm__##_t *       tree,  /**< The tree in question*/\
    void                        (*destr)(void*) \
);  \
    \
    \
    \
/** insert a node, specifying its position, and optional data */ \
KDTREE_EXPORT int \
kdtree_##__nm__##_insert ( \
    kdtree_##__nm__##_t *       tree, /**< The tree in question*/\
    const __data_type__ *       pos, \
    void *data \
);  \
    \
    \
    \
/** Find an exact match or return NULL */ \
KDTREE_EXPORT kdtree_node_##__nm__##_t * \
kdtree_##__nm__##_find_exact ( \
    kdtree_##__nm__##_t *       tree, /**< The tree in question*/\
    const __data_type__ *       pos \
);  \
    \
    \
    \
/** Find an exact match or return NULL */ \
KDTREE_EXPORT void * \
kdtree_##__nm__##_find_exact_data ( \
    kdtree_##__nm__##_t *       kd, \
    const __data_type__ *       pos \
);  \
    \
    \
    \
/** Find the nearest node from a given point */ \
KDTREE_EXPORT struct kdres_##__nm__ * \
kdtree_##__nm__##_nearest ( \
    kdtree_##__nm__##_t *       tree, \
    const __data_type__ *       pos \
);  \
    \
    \
    \
/** Find any nearest nodes from a given point within a range */ \
KDTREE_EXPORT struct kdres_##__nm__ * \
kdtree_##__nm__##_nearest_range ( \
    kdtree_##__nm__##_t *       tree, \
    const __data_type__ *       pos, \
    __data_type__               range \
);  \
    \
    \
    \
/** frees a result set returned by kdtree_nearest_range() */ \
KDTREE_EXPORT void  \
kdtree_##__nm__##_res_free ( \
    struct kdres_##__nm__ *     set \
);  \
    \
    \
    \
/** returns the size of the result set (in elements) */ \
KDTREE_EXPORT int  \
kdtree_##__nm__##_res_size ( \
    struct kdres_##__nm__ *     set \
);  \
    \
    \
    \
/** rewinds the result set iterator */ \
KDTREE_EXPORT void  \
kdtree_##__nm__##_res_rewind ( \
    struct kdres_##__nm__ *     set \
);  \
    \
    \
    \
/** returns non-zero if the set iterator reached the end after the last element */ \
KDTREE_EXPORT int  \
kdtree_##__nm__##_res_end ( \
    struct kdres_##__nm__ *     set \
);  \
    \
    \
    \
/** returns non-zero on success, zero if there are no more elements */ \
KDTREE_EXPORT int  \
kdtree_##__nm__##_res_next ( \
    struct kdres_##__nm__ *     set \
);  \
    \
    \
    \
/** returns the data pointer (can be null) of the current result set item and optionally sets its position to the pointers(s) if not null */ \
KDTREE_EXPORT void * \
kdtree_##__nm__##_res_item ( \
    struct kdres_##__nm__ *     set, \
    __data_type__ *pos \
);  \
    \
    \
    \
/** equivalent to kdtree_res_item(set, 0) */ \
KDTREE_EXPORT void * \
kdtree_##__nm__##_res_item_data ( \
    struct kdres_##__nm__ *     set \
);




//! defines a kdtree with specified data type and number of dimensions (use in source file)
#define KDTREE_STATIC_DEFINE_FUNCTIONS(__nm__, __data_type__, __dims__, __user_space_on_node__) \
    \
list_node_predef(__nm__) \
    \
struct res_node_##__nm__ { \
    kdtree_node_##__nm__##_t *      item; \
    __data_type__                   dist_sq; \
    struct res_node_##__nm__ *      next; \
};  \
    \
struct kdres_##__nm__ { \
    kdtree_##__nm__##_t *tree; \
    struct res_node_##__nm__ *rlist, *riter; \
    int size; \
};  \
    \
static void clear_rec_##__nm__(kdtree_##__nm__##_t *tree, kdtree_node_##__nm__##_t *node); \
static int insert_rec##__nm__(kdtree_node_##__nm__##_t **node, const __data_type__ *pos, void *data, int dir/* DYNDIM, int dim*/); \
static int rlist_##__nm__##_insert(struct res_node_##__nm__ *list, kdtree_node_##__nm__##_t *item, __data_type__ dist_sq); \
static void clear_##__nm__##_results(struct kdres_##__nm__ *set); \
    \
static void hyperrect_##__nm__##_duplicate(const kdtree_rect_##__nm__##_t *src, kdtree_rect_##__nm__##_t *rect); \
static void hyperrect_##__nm__##_extend(kdtree_rect_##__nm__##_t *rect, const __data_type__ *pos); \
static __data_type__ hyperrect_##__nm__##_dist_sq(kdtree_rect_##__nm__##_t *rect, const __data_type__ *pos); \
    \
    \
kdtree_##__nm__##_t *kdtree_##__nm__##_create() \
{ \
    kdtree_##__nm__##_t *tree; \
    \
    if(!(tree = (kdtree_##__nm__##_t*)malloc(sizeof *tree))) { \
        return 0; \
    } \
    \
    /* DYNDIM tree->dim = __dims__;*/ \
    tree->root = 0; \
    tree->destr = 0; \
    tree->rect_init = 0; \
    \
    return tree; \
} \
    \
void kdtree_##__nm__##_free(kdtree_##__nm__##_t *tree) \
{ \
    if(tree) { \
        kdtree_##__nm__##_clear(tree); \
        free(tree); \
    } \
} \
    \
static void clear_rec_##__nm__(kdtree_##__nm__##_t *tree, kdtree_node_##__nm__##_t *node) \
{ \
    if(!node) return; \
    \
    clear_rec_##__nm__(tree, node->left); \
    clear_rec_##__nm__(tree, node->right); \
    \
    if(tree->destr) { \
        tree->destr(tree, node); \
    } \
    free(node); \
} \
    \
void kdtree_##__nm__##_clear(kdtree_##__nm__##_t *tree) \
{ \
    if (tree->root != NULL) { \
        clear_rec_##__nm__(tree, tree->root); \
        tree->root = NULL; \
    } \
    tree->rect_init = 0; \
    \
    /*if (tree->rect) { */\
    /*    hyperrect_##__nm__##_free(tree->rect); */\
    /*    tree->rect = 0; */\
    /*} */\
} \
    \
void kdtree_##__nm__##_data_destructor(kdtree_##__nm__##_t *tree, kdtree_##__nm__##_node_destructor_t destr) \
{ \
    tree->destr = destr; \
} \
    \
    \
static int insert_rec##__nm__(kdtree_node_##__nm__##_t **nptr, const __data_type__ *pos, void *data, int dir/* DYNDIM, int dim*/) \
{ \
    int new_dir; \
    kdtree_node_##__nm__##_t *node; \
    \
    if(!*nptr) { \
        if(!(node = (kdtree_node_##__nm__##_t *)malloc(sizeof(kdtree_node_##__nm__##_t) + __user_space_on_node__))) { \
            return -1; \
        } \
        /* DYNDIM if(!(node->pos = (__data_type__*)malloc(dim * sizeof *node->pos))) { */\
        /* DYNDIM     free(node); */\
        /* DYNDIM     return -1; */\
        /* DYNDIM } */\
        memcpy(/* DYNDIM node->pos*/ &node->position, pos, /* DYNDIM dim */ __dims__ * sizeof *node->position); \
        node->data = data; \
        node->dir = dir; \
        node->left = node->right = 0; \
        *nptr = node; \
        return 0; \
    } \
    \
    node = *nptr; \
    new_dir = (node->dir + 1) % /* DYNDIM dim */ __dims__; \
    if(pos[node->dir] < node->position[node->dir]) { \
        return insert_rec##__nm__(&(*nptr)->left, pos, data, new_dir/* DYNDIM, dim*/); \
    } \
    return insert_rec##__nm__(&(*nptr)->right, pos, data, new_dir/* DYNDIM, dim*/); \
} \
    \
int kdtree_##__nm__##_insert(kdtree_##__nm__##_t *tree, const __data_type__ *pos, void *data) \
{ \
    KDTREE_DEBUG_ASSERT(tree != NULL); \
    if (insert_rec##__nm__(&tree->root, pos, data, 0 /* DYNDIM, tree->dim*/)) { \
        return -1; \
    } \
    \
    /* if (tree->rect == 0) { */ \
    /*    tree->rect = hyperrect_##__nm__##_create( tree->dim,  pos, pos); */\
    /*} else { */\
    /*    hyperrect_##__nm__##_extend(tree->rect, pos); */\
    /*} */\
    if (tree->rect_init) { \
        hyperrect_##__nm__##_extend(&tree->rect, pos);\
    } else { \
        int size = __dims__ * sizeof(__data_type__); \
        memcpy(tree->rect.minimum, pos, size); \
        memcpy(tree->rect.maximum, pos, size); \
        tree->rect_init = 1;\
    } \
    \
    return 0; \
} \
    \
static int find_##__nm__##_nearest(kdtree_node_##__nm__##_t *node, \
    const __data_type__ *pos, __data_type__ range, \
    struct res_node_##__nm__ *list, int ordered, int dim) \
{ \
    KDTREE_DEBUG_ASSERT(node != NULL); \
    KDTREE_DEBUG_ASSERT(pos != NULL); \
    __data_type__ dist_sq, dx; \
    int i, ret, added_res = 0; \
    \
    if(!node) return 0; \
    \
    dist_sq = 0; \
    for(i=0; i</* DYNDIM dim*/ __dims__; i++) { \
        dist_sq += SQ(node->position[i] - pos[i]); \
    } \
    if(dist_sq <= SQ(range)) { \
        if(rlist_##__nm__##_insert(list, node, ordered ? dist_sq : -1.0) == -1) { \
            return -1; \
        } \
        added_res = 1; \
    } \
    \
    dx = pos[node->dir] - node->position[node->dir]; \
    \
    ret = find_##__nm__##_nearest(dx <= 0.0 ? node->left : node->right, pos, range, list, ordered, /* DYNDIM dim*/ __dims__); \
    if(ret >= 0 && fabs(dx) < range) { \
        added_res += ret; \
        ret = find_##__nm__##_nearest(dx <= 0.0 ? node->right : node->left, pos, range, list, ordered, /* DYNDIM dim*/ __dims__); \
    } \
    if(ret == -1) { \
        return -1; \
    } \
    added_res += ret; \
    \
    return added_res; \
} \
    \
static void kdtree_##__nm__##_nearest_i( \
    kdtree_node_##__nm__##_t *node, const __data_type__ *pos, \
    kdtree_node_##__nm__##_t **result, __data_type__ *result_dist_sq, \
    kdtree_rect_##__nm__##_t* rect) \
{ \
    KDTREE_DEBUG_ASSERT(node != NULL); \
    KDTREE_DEBUG_ASSERT(pos != NULL); \
    int dir = node->dir; \
    int i; \
    __data_type__ dummy, dist_sq; \
    kdtree_node_##__nm__##_t *nearer_subtree, *farther_subtree; \
    __data_type__ *nearer_hyperrect_coord, *farther_hyperrect_coord; \
    \
    /* Decide whether to go left or right in the tree */ \
    dummy = pos[dir] - node->position[dir]; \
    if (dummy <= 0) { \
        nearer_subtree = node->left; \
        farther_subtree = node->right; \
        nearer_hyperrect_coord = &rect->maximum[dir]; \
        farther_hyperrect_coord = &rect->minimum[dir]; \
    } else { \
        nearer_subtree = node->right; \
        farther_subtree = node->left; \
        nearer_hyperrect_coord = &rect->minimum[dir]; \
        farther_hyperrect_coord = &rect->maximum[dir]; \
    } \
    \
    if (nearer_subtree) { \
        /* Slice the hyperrect to get the hyperrect of the nearer subtree */ \
        dummy = *nearer_hyperrect_coord; \
        *nearer_hyperrect_coord = node->position[dir]; \
        /* Recurse down into nearer subtree */ \
        kdtree_##__nm__##_nearest_i(nearer_subtree, pos, result, result_dist_sq, rect); \
        /* Undo the slice */ \
        *nearer_hyperrect_coord = dummy; \
    } \
    \
    /* Check the distance of the point at the current node, compare it \
     * with our best so far */ \
    dist_sq = 0; \
    for(i=0; i < /* DYNDIM rect->dim*/ __dims__; i++) { \
        dist_sq += SQ(node->position[i] - pos[i]); \
    } \
    if (dist_sq < *result_dist_sq) { \
        *result = node; \
        *result_dist_sq = dist_sq; \
    } \
    \
    if (farther_subtree) { \
        /* Get the hyperrect of the farther subtree */ \
        dummy = *farther_hyperrect_coord; \
        *farther_hyperrect_coord = node->position[dir]; \
        /* Check if we have to recurse down by calculating the closest \
         * point of the hyperrect and see if it's closer than our \
         * minimum distance in result_dist_sq. */ \
        if (hyperrect_##__nm__##_dist_sq(rect, pos) < *result_dist_sq) { \
            /* Recurse down into farther subtree */ \
            kdtree_##__nm__##_nearest_i(farther_subtree, pos, result, result_dist_sq, rect); \
        } \
        /* Undo the slice on the hyperrect */ \
        *farther_hyperrect_coord = dummy; \
    } \
} \
    \
struct kdres_##__nm__ *kdtree_##__nm__##_nearest( \
    kdtree_##__nm__##_t *kd, const __data_type__ *pos) \
{ \
    KDTREE_DEBUG_ASSERT(kd != NULL); \
    KDTREE_DEBUG_ASSERT(pos != NULL); \
    kdtree_rect_##__nm__##_t rect; \
    kdtree_node_##__nm__##_t *result; \
    struct kdres_##__nm__ *rset; \
    __data_type__ dist_sq; \
    int i; \
    \
    if (!kd) return 0; \
    if (!kd->rect_init) return 0; \
    \
    /* Allocate result set */ \
    if(!(rset = (struct kdres_##__nm__ *)malloc(sizeof *rset))) { \
        return 0; \
    } \
    rset->rlist = alloc_resnode(__nm__); \
    if(rset->rlist == NULL) { \
        free(rset); \
        return 0; \
    } \
    rset->rlist->next = 0; \
    rset->tree = kd; \
    \
    /* Duplicate the bounding hyperrectangle, we will work on the copy */ \
    /*if (!(rect = hyperrect_##__nm__##_duplicate(&kd->rect))) { */\
    /*    kdtree_##__nm__##_res_free(rset); */ \
    /*    return 0; */\
    /*} */\
    hyperrect_##__nm__##_duplicate(&kd->rect, &rect);\
    \
    /* Our first guesstimate is the root node */ \
    result = kd->root; \
    dist_sq = 0; \
    for (i = 0; i < /* DYNDIM kd->dim*/ __dims__ ; i++) \
        dist_sq += SQ(result->position[i] - pos[i]); \
    \
    /* Search for the nearest neighbour recursively */ \
    kdtree_##__nm__##_nearest_i(kd->root, pos, &result, &dist_sq, &rect); \
    \
    /* Free the copy of the hyperrect */ \
    /* hyperrect_##__nm__##_free(rect); */\
    \
    /* Store the result */ \
    if (result) { \
        if (rlist_##__nm__##_insert(rset->rlist, result, -1.0) == -1) { \
            kdtree_##__nm__##_res_free(rset); \
            return 0; \
        } \
        rset->size = 1; \
        kdtree_##__nm__##_res_rewind(rset); \
        return rset; \
    } else { \
        kdtree_##__nm__##_res_free(rset); \
        return 0; \
    } \
} \
    \
kdtree_node_##__nm__##_t *kdtree_##__nm__##_find_exact(kdtree_##__nm__##_t *kd, const __data_type__ *pos) \
{ \
    \
    kdtree_rect_##__nm__##_t rect; \
    kdtree_node_##__nm__##_t *result; \
    __data_type__ dist_sq; \
    int i; \
    \
    if (!kd) return 0; \
    if (!kd->rect_init) return 0; \
    \
    /* Duplicate the bounding hyperrectangle, we will work on the copy */ \
    /* if (!(rect = hyperrect_##__nm__##_duplicate(&kd->rect))) { */ \
    /*    return NULL; */ \
    /* } */ \
    hyperrect_##__nm__##_duplicate(&kd->rect, &rect);\
    \
    \
    /* Our first guesstimate is the root node */ \
    result = kd->root; \
    dist_sq = 0; \
    for (i = 0; i < /* DYNDIM kd->dim*/ __dims__; i++) \
        dist_sq += SQ(result->position[i] - pos[i]); \
    \
    /* Search for the nearest neighbour recursively */ \
    kdtree_##__nm__##_nearest_i(kd->root, pos, &result, &dist_sq, &rect); \
    \
    /* Free the copy of the hyperrect */ \
    /* hyperrect_##__nm__##_free(rect);*/ \
    \
    return dist_sq == 0 ? result : NULL; \
    \
} \
    \
void *kdtree_##__nm__##_find_exact_data(kdtree_##__nm__##_t *kd, const __data_type__ *pos) \
{ \
    kdtree_node_##__nm__##_t *result = kdtree_##__nm__##_find_exact(kd, pos); \
    return result == NULL ? NULL : result->data; \
} \
    \
struct kdres_##__nm__ *kdtree_##__nm__##_nearest_range( \
    kdtree_##__nm__##_t *kd, const __data_type__ *pos, __data_type__ range) \
{ \
    KDTREE_DEBUG_ASSERT(kd != NULL); \
    KDTREE_DEBUG_ASSERT(pos != NULL); \
    int ret; \
    struct kdres_##__nm__ *rset; \
    \
    if(!(rset = (struct kdres_##__nm__ *)malloc(sizeof *rset))) { \
        return 0; \
    } \
    if(!(rset->rlist = alloc_resnode(__nm__))) { \
        free(rset); \
        return 0; \
    } \
    rset->rlist->next = 0; \
    rset->tree = kd; \
    \
    if((ret = find_##__nm__##_nearest(kd->root, pos, range, rset->rlist, 0, /* DYNDIM kd->dim*/ __dims__)) == -1) { \
        kdtree_##__nm__##_res_free(rset); \
        return 0; \
    } \
    rset->size = ret; \
    kdtree_##__nm__##_res_rewind(rset); \
    return rset; \
} \
    \
void kdtree_##__nm__##_res_free(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    clear_##__nm__##_results(rset); \
    free_resnode(rset->rlist); \
    free(rset); \
} \
    \
int kdtree_##__nm__##_res_size(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    return (rset->size); \
} \
    \
void kdtree_##__nm__##_res_rewind(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    KDTREE_DEBUG_ASSERT(rset->rlist != NULL); \
    rset->riter = rset->rlist->next; \
} \
    \
int kdtree_##__nm__##_res_end(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    return rset->riter == 0; \
} \
    \
int kdtree_##__nm__##_res_next(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    KDTREE_DEBUG_ASSERT(rset->riter != NULL); \
    rset->riter = rset->riter->next; \
    return rset->riter != 0; \
} \
    \
void *kdtree_##__nm__##_res_item(struct kdres_##__nm__ *rset, __data_type__ *pos) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    if(rset->riter) { \
        if(pos) { \
            memcpy(pos, rset->riter->item->position, /* DYNDIM rset->tree->dim*/ __dims__ * sizeof *pos); \
        } \
        return rset->riter->item->data; \
    } \
    return 0; \
} \
    \
void *kdtree_##__nm__##_res_item_data(struct kdres_##__nm__ *rset) \
{ \
    KDTREE_DEBUG_ASSERT(rset != NULL); \
    return kdtree_##__nm__##_res_item(rset, 0); \
} \
    \
static void hyperrect_##__nm__##_duplicate( \
    const kdtree_rect_##__nm__##_t *rect, \
    kdtree_rect_##__nm__##_t *dest) \
{ \
    /*return hyperrect_##__nm__##_create( DYNDIM rect->dim,  rect->minimum, rect->maximum); */\
    *dest = *rect;\
} \
    \
static void hyperrect_##__nm__##_extend( \
    kdtree_rect_##__nm__##_t *rect, const __data_type__ *pos) \
{ \
    int i; \
    \
    for (i=0; i < /* DYNDIM rect->dim*/ __dims__ ; i++) { \
        if (pos[i] < rect->minimum[i]) { \
            rect->minimum[i] = pos[i]; \
        } \
        if (pos[i] > rect->maximum[i]) { \
            rect->maximum[i] = pos[i]; \
        } \
    } \
} \
    \
static __data_type__ hyperrect_##__nm__##_dist_sq( \
    kdtree_rect_##__nm__##_t *rect, const __data_type__ *pos) \
{ \
    int i; \
    __data_type__ result = 0; \
    \
    for (i=0; i < /* DYNDIM rect->dim*/ __dims__; i++) { \
        if (pos[i] < rect->minimum[i]) { \
            result += SQ(rect->minimum[i] - pos[i]); \
        } else if (pos[i] > rect->maximum[i]) { \
            result += SQ(rect->maximum[i] - pos[i]); \
        } \
    } \
    \
    return result; \
} \
    \
/* ---- static helpers ---- */ \
    \
/* inserts the item. if dist_sq is >= 0, then do an ordered insert */ \
/* TODO make the ordering code use heapsort */ \
static int rlist_##__nm__##_insert( \
    struct res_node_##__nm__ *list, kdtree_node_##__nm__##_t *item, \
    __data_type__ dist_sq) \
{ \
    struct res_node_##__nm__ *rnode; \
    \
    if(!(rnode = alloc_resnode(__nm__))) { \
        return -1; \
    } \
    rnode->item = item; \
    rnode->dist_sq = dist_sq; \
    \
    if(dist_sq >= 0.0) { \
        while(list->next && list->next->dist_sq < dist_sq) { \
            list = list->next; \
        } \
    } \
    rnode->next = list->next; \
    list->next = rnode; \
    return 0; \
} \
    \
static void clear_##__nm__##_results(struct kdres_##__nm__ *rset) \
{ \
    struct res_node_##__nm__ *tmp, *node = rset->rlist->next; \
    \
    while(node) { \
        tmp = node; \
        node = node->next; \
        free_resnode(tmp); \
    } \
    \
    rset->rlist->next = 0; \
} \
    \
list_node_func(__nm__) \

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

// ^^^
// part that gets included only once
#endif // AITOWN_kdtree_h_INCLUDE
