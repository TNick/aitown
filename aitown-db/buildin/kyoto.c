/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			kyoto.c
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

#include "kyoto.h"

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-driver.h>
#include <aitown/utils.h>
#include <aitown/dbg_assert.h>

#include <stdlib.h>
#include <string.h>

#include <kclangc.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a db kyoto driver
///
typedef struct _aitown_db_driver_kyoto_t {
    aitown_db_driver_t      header; /**< standard header for all drivers */
} aitown_db_driver_kyoto_t;

//! describes a kyoto database
///
typedef struct _aitown_db_kyoto_t {
    aitown_db_t             header; /**< standard header for all databases */
    KCDB*                   db;
    char *                  path;
} aitown_db_kyoto_t;


//! database suffix
#define KYOTO_SUFFIX    ".kch"

//! our name
#define KYOTO_NAME  "buildin-kyoto"

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

func_error_t aitown_db_kyoto_open (aitown_db_open_t*data)
{
    DBG_ASSERT (data != NULL);
    DBG_ASSERT (data->db_mng != NULL);

    func_error_t    ret = FUNC_OK;

    aitown_db_kyoto_t * new_db = NULL;
    char * path = NULL;

    for (;;) {

        // make sure prerequisites are in place
        if ( (data->db_path_hint == NULL) || (data->db_name == NULL) ) {
            ret = FUNC_BAD_INPUT;
            err_message("A database name and a path hint are required to open a kyoto database");
            break;
        }

        // allocate a new database structure
        new_db = (aitown_db_kyoto_t*)malloc (
                    sizeof(aitown_db_kyoto_t));
        if (new_db == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // initialise as database
        aitown_db_init (&new_db->header);

        // compute the path and name of the database
        size_t path_l = strlen (data->db_path_hint);
        size_t name_l = strlen (data->db_name);
        size_t suffix_l = sizeof(KYOTO_SUFFIX);
        path = (char*)malloc(path_l + 1 + name_l + suffix_l);
        if (path == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // copy the path
        memcpy (path, data->db_path_hint, path_l);
        if ( (path[path_l-1] != '/') && (path[path_l-1] != '\\')) {
            path[path_l] = '/';
            path_l++;
        }

        // copy the name and suffix
        memcpy (path+path_l, data->db_name, name_l);
        memcpy (path+path_l+name_l, KYOTO_SUFFIX, sizeof(KYOTO_SUFFIX));

        // now open it
        new_db->db = kcdbnew();
        if (!kcdbopen (new_db->db, path, KCOWRITER | KCOCREATE)) {
            ret = FUNC_BAD_INPUT;
            err_message("Kyoto database open error: %s\n",
                        kcecodename (kcdbecode (new_db->db)));
            break;
        }

        new_db->path = path;
        *data->out = (aitown_db_t*)new_db;
        break;
    }

    // clean up in case of error
    if (ret != FUNC_OK) {
        if (path != NULL) {
            free (path);
        }
        if (new_db != NULL) {
            if (new_db->db != NULL) {
                kcdbdel (new_db->db);
            }
            aitown_db_end (&new_db->header);
            free (new_db);
        }
    }

    return ret;
}

func_error_t aitown_db_kyoto_close (aitown_db_mng_t * db_mng, aitown_db_t*db)
{
    DBG_ASSERT (db != NULL);
    DBG_ASSERT (db->driver != NULL);
    DBG_ASSERT (db_mng != NULL);

    func_error_t ret = FUNC_OK;
    for (;;) {

        // assume it's our kind
        aitown_db_kyoto_t * database = (aitown_db_kyoto_t*)db;
        DBG_ASSERT (database->db != NULL);

        if (!kcdbclose (database->db)) {
            err_message("Kyoto database close error: %s",
                        kcecodename (kcdbecode (database->db)));
            ret = FUNC_GENERIC_ERROR;
            break;
        }
        kcdbdel (database->db);

        // free other resources
        DBG_ASSERT (database->path != NULL);
        free (database->path);
        aitown_db_end (&database->header);
        free (database);

        break;
    }
    return ret;
}

func_error_t aitown_db_kyoto_read (aitown_db_io_t * request)
{
    DBG_ASSERT (request != NULL);

    func_error_t    ret = FUNC_OK;
    size_t vsiz;
    void *vbuf ;
    for (;;) {

        // assume it's our kind
        aitown_db_kyoto_t * database = (aitown_db_kyoto_t*)request->db;
        DBG_ASSERT (database->db != NULL);

        // read data
        vbuf = (void*)kcdbget (
                    database->db,
                    (const char*)request->key.ptr,
                    request->key_sz,
                    &vsiz);
        if (vbuf == NULL) {
            err_message("Kyoto database - error reading record : %s",
                  kcecodename (kcdbecode (database->db)));
            ret = FUNC_GENERIC_ERROR; break;
        }

        break;
    }

    // either use the callback or set the value in request
    if (request->kb.kb_read == NULL) {
        *request->val.pp = vbuf;
        *request->val_sz.pd = vsiz;
    } else {
        request->kb.kb_read (
                    request->db_mng,
                    request->db,
                    ret,
                    request->user,
                    vbuf,
                    vsiz
                    );
    }

    return ret;
}

func_error_t aitown_db_kyoto_write (aitown_db_io_t * request)
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // assume it's our kind
        aitown_db_kyoto_t * database = (aitown_db_kyoto_t*)request->db;
        DBG_ASSERT (database->db != NULL);

        // do the writing
        if (!kcdbset (
                database->db,
                (const char*)request->key.ptr,
                request->key_sz,
                (const char*)request->val.p,
                request->val_sz.d )) {

            err_message("Kyoto database - error writing record : %s",
                  kcecodename(kcdbecode(database->db)));
            ret = FUNC_GENERIC_ERROR; break;
        }

        break;
    }

    // either use the callback or set the value in request
    if (request->kb.kb_write != NULL) {
        request->kb.kb_write (
                    request->db_mng,
                    request->db,
                    ret,
                    request->user
                    );
    }
    return ret;
}

func_error_t aitown_db_kyoto_free_chunk (
        aitown_db_mng_t * db_mng, aitown_db_t*db, void * chunk)
{
    kcfree (chunk);
    return FUNC_OK;
}

func_error_t aitown_db_kyoto_read_k64 (aitown_db_io_t * request)
{
    aitown_db_io_t internal = *request;
    internal.key.ptr = &request->key.u64;
    internal.key_sz = 8;
    return aitown_db_kyoto_read (&internal);
}

func_error_t aitown_db_kyoto_write_k64 (aitown_db_io_t * request)
{
    aitown_db_io_t internal = *request;
    internal.key.ptr = &request->key.u64;
    internal.key_sz = 8;
    return aitown_db_kyoto_write (&internal);
}

void aitown_db_kyoto_init (aitown_db_mng_t *db_mng)
{

    // allocate memory for us
    aitown_db_driver_kyoto_t * ret = (aitown_db_driver_kyoto_t*)malloc (
                sizeof(aitown_db_driver_kyoto_t));
    if (ret == NULL) {
        dbg_message ("Kyoto cabinet failed to start due to a memory failure");
        return;
    }

    // initialize it as a standard driver
    aitown_db_driver_init ((aitown_db_driver_t*)ret);
    ret->header.name = KYOTO_NAME;

    ret->header.open = aitown_db_kyoto_open;
    ret->header.close = aitown_db_kyoto_close;
    ret->header.read = aitown_db_kyoto_read;
    ret->header.write = aitown_db_kyoto_write;
    ret->header.free_chunk = aitown_db_kyoto_free_chunk;
    ret->header.read_k64 = aitown_db_kyoto_read_k64;
    ret->header.write_k64 = aitown_db_kyoto_write_k64;

    // inform the manager about it
    aitown_db_mng_driver_add (db_mng, (aitown_db_driver_t*)ret);
}

void aitown_db_kyoto_end (aitown_db_mng_t *db_mng)
{
    // ask the manager to remove us from the list
    aitown_db_driver_t * instance;
    if (FUNC_OK == aitown_db_mng_driver_rem_n (
            db_mng, KYOTO_NAME, &instance))
    {
        aitown_db_driver_kyoto_t * kyoto = (aitown_db_driver_kyoto_t*)instance;

        // terminate the structure
        aitown_db_driver_end (instance);

        // release memory used by the structure
        free (kyoto);
    }

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


