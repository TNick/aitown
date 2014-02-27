/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			mysql.c
  \date			February 2014
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

#include "mysql-local.h"

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-mng.h>
#include <aitown/aitown-db-open.h>
#include <aitown/aitown-db-close.h>
#include <aitown/aitown-db-driver.h>
#include <aitown/utils.h>
#include <aitown/dbg_assert.h>
#include <aitown/struct_ini.h>

#include <stdlib.h>
#include <string.h>

#include <mysql.h>

/**
  @todo free sini when ours
  @todo read params from sini when not provided
*/

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes a db mysql driver
///
typedef struct _aitown_db_driver_mysql_t {
    aitown_db_driver_t      header; /**< standard header for all drivers */
} aitown_db_driver_mysql_t;

//! describes a mysql database
///
typedef struct _aitown_db_mysql_t {
    aitown_db_t             header; /**< standard header for all databases */
    MYSQL *                 con;
    char *                  name;
} aitown_db_mysql_t;


//! database suffix
#define MYSQL_SUFFIX    ".kch"

//! our name
#define MYSQL_NAME  "buildin-mysql"

//! the name of the key for user name
#define MYSQL_KEY_USERNAME      "username"

//! the name of the key for user password
#define MYSQL_KEY_PASS           "password"


//! querry for creating a database
#define MYSQL_Q_CREATE_DB        "CREATE DATABASE IF NOT EXISTS %s;"

//! querry for creating a table
#define MYSQL_Q_CREATE_TBL       "CREATE TABLE IF NOT EXISTS %s(Id VARCHAR(255) PRIMARY KEY, MasterValue MEDIUMBLOB);"
//#define MYSQL_Q_CREATE_TBL       "CREATE TABLE IF NOT EXISTS %s(Id INT PRIMARY KEY AUTO_INCREMENT, MasterValue MEDIUMBLOB);"

//! inserting values in a table
#define MYSQL_Q_INSERT          "INSERT INTO %s VALUES(%s,%s) ON DUPLICATE KEY UPDATE MasterValue=values(MasterValue);"


//! getting values from a table
#define MYSQL_Q_SELECT          "SELECT Data FROM %s WHERE Id='%s';"



//! append a literal string
#define MYSQL_APPEND_LITERAL(__iterator__,__s__) \
    memcpy (__iterator__, __s__, sizeof(__s__) ); \
    __iterator__ += sizeof(__s__) - 1


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

static func_error_t extend_buffer(char ** p, size_t * crt_sz, size_t req_sz)
{

    char * rel;
    if (*p == NULL) {
        rel = (char*)malloc(req_sz + 128);
        if (rel == NULL) {
            return FUNC_MEMORY_ERROR;
        }
    } else {
        if (*crt_sz > req_sz)
            return FUNC_OK;
        rel = (char*)malloc(req_sz + 128);
        if (rel == NULL) {
            return FUNC_MEMORY_ERROR;
        }
        free (*p);
    }
    if (rel == NULL) {
        return FUNC_MEMORY_ERROR;
    }
    *p = rel;
    *crt_sz = req_sz + 128;
    return FUNC_OK;
}

func_error_t aitown_db_mysql_open (aitown_db_open_t*data)
{
    DBG_ASSERT (data != NULL);
    DBG_ASSERT (data->db_mng != NULL);

    func_error_t    ret = FUNC_OK;

    aitown_db_mysql_t * new_db = NULL;
    char * querry_buffer = NULL;
    size_t querry_buffer_sz;
    size_t querry_computed;

    aitown_cfg_leaf_t * cfg_mysql_user = NULL;
    aitown_cfg_leaf_t * cfg_mysql_pass = NULL;
    const char * mysql_user;
    const char * mysql_pass;

    for (;;) {

        // a database is required
        if (data->db_name == NULL) {
            err_message ("mysql driver requires a database name");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // get a buffer for our querries
        querry_buffer = (char*)malloc(1024);
        querry_buffer_sz = 1024;
        if (querry_buffer == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        int b_cfg_ok = 0;
        for (;;) {
            aitown_db_mng_t *db_mng = data->db_mng;

            // get root section
            aitown_cfg_sect_t * root_cfg_sect = db_mng->cfg_sect;
            if (root_cfg_sect == NULL)
                break;

            // get databases section
            aitown_cfg_sect_t * cfg_sect_databases =
                    aitown_cfg_get_sect( root_cfg_sect, "databases");
            if (cfg_sect_databases == NULL)
                break;

            // get the section for our database
            aitown_cfg_sect_t * cfg_sect_ourdb =
                    aitown_cfg_get_sect( cfg_sect_databases, data->db_name);
            if (cfg_sect_ourdb == NULL)
                break;

            cfg_mysql_user =
                    aitown_cfg_get_leaf( cfg_sect_ourdb, MYSQL_KEY_USERNAME );
            if ((cfg_mysql_user != NULL) && (cfg_mysql_user->value != NULL))
                break;

            cfg_mysql_pass =
                    aitown_cfg_get_leaf( cfg_sect_ourdb, MYSQL_KEY_PASS );
            if ((cfg_mysql_pass != NULL) && (cfg_mysql_pass->value != NULL))
                break;

            b_cfg_ok = 1;
            break;
        }
        if (!b_cfg_ok) {
            err_message (
                        "mysql driver requires configuration to open a "
                        "database (at least user and password)");
            ret = FUNC_BAD_INPUT;
            break;
        }

        if (*cfg_mysql_user->value == 0) {
            mysql_user = NULL;
        } else {
            mysql_user = cfg_mysql_user->value;
        }
        if (*cfg_mysql_pass->value == 0) {
            mysql_pass = NULL;
        } else {
            mysql_pass = cfg_mysql_pass->value;
        }

        // allocate a new database structure
        new_db = (aitown_db_mysql_t*)malloc (
                    sizeof(aitown_db_mysql_t));
        if (new_db == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // copy the name
        new_db->name = strdup (data->db_name);

        // start a new connection
        new_db->con = mysql_init(NULL);
        if (new_db->con == NULL) {
            err_message (mysql_error(new_db->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // attempt to connect
        if (mysql_real_connect(
                new_db->con, data->path_hint,
                mysql_user, mysql_pass,
                NULL, 0, NULL, 0) == NULL)
        {
            err_message (mysql_error(new_db->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // create database if it does not exist
        querry_computed = sizeof(MYSQL_Q_CREATE_DB) + strlen(data->db_name);
        ret = extend_buffer (&querry_buffer, &querry_buffer_sz, querry_computed);
        if (ret != FUNC_OK) {
            break;
        }
        sprintf (querry_buffer, MYSQL_Q_CREATE_DB, data->db_name);
        if (mysql_query(new_db->con, querry_buffer))
        {
            err_message (mysql_error(new_db->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }
        mysql_close (new_db->con);

        // attempt to connect
        new_db->con = mysql_init(NULL);
        if (mysql_real_connect(
                new_db->con, data->path_hint,
                cfg_mysql_user->value, cfg_mysql_pass->value,
                data->db_name, 0, NULL, 0) == NULL)
        {
            err_message (mysql_error(new_db->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // create table if it does not exist
        querry_computed = sizeof(MYSQL_Q_CREATE_TBL) + strlen(data->db_name);
        ret = extend_buffer (&querry_buffer, &querry_buffer_sz, querry_computed);
        if (ret != FUNC_OK) {
            break;
        }
        sprintf (querry_buffer, MYSQL_Q_CREATE_TBL, data->db_name);
        if (mysql_query(new_db->con, querry_buffer))
        {
            err_message (mysql_error(new_db->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        *data->out = (aitown_db_t*)new_db;
        break;
    }

    if (querry_buffer != NULL) {
        free (querry_buffer);
    }

    // clean up in case of error
    if (ret != FUNC_OK) {
        if (new_db != NULL) {
            if (new_db->con != NULL) {
                mysql_close (new_db->con);
            }
            if (new_db->name != NULL) {
                free (new_db->name);
            }
            aitown_db_end (&new_db->header);
            free (new_db);
        }
    }
    return ret;
}

func_error_t aitown_db_mysql_close (aitown_db_t*db)
{
    DBG_ASSERT (db != NULL);
    DBG_ASSERT (db->driver != NULL);

    func_error_t ret = FUNC_OK;
    for (;;) {

        // assume it's our kind
        aitown_db_mysql_t * database = (aitown_db_mysql_t*)db;

        // close the connection
        if (database->con != NULL) {
            mysql_close (database->con);
        }

        // release the name
        if (database->name != NULL) {
            free (database->name);
        }

        // free basic stuff
        aitown_db_end (&database->header);
        free (database);
        break;
    }
    return ret;
}

func_error_t aitown_db_mysql_read (aitown_db_read_t * request)
{
    DBG_ASSERT (request != NULL);

    char * querry_buffer = NULL;
    char * iterator;
    size_t querry_buffer_sz;
    size_t querry_computed;
    MYSQL_RES *result = NULL;

    int vsiz = 0;
    void *vbuf = NULL;

    func_error_t    ret = FUNC_OK;
    for (;;) {

        // assume it's our kind
        aitown_db_mysql_t * database = (aitown_db_mysql_t*)request->db;
        DBG_ASSERT (database != NULL);
        DBG_ASSERT (database->con != NULL);

        // approximate our querry size
        size_t db_len = strlen(database->name);
        querry_computed =
                sizeof(MYSQL_Q_SELECT) +
                db_len +
                request->key_sz*2 +
                2;

        // allocate
        ret = extend_buffer(&querry_buffer, &querry_buffer_sz, querry_computed);
        if (ret != FUNC_OK)
            break;
        iterator = querry_buffer;


        // copy first part
        MYSQL_APPEND_LITERAL(iterator,"SELECT MasterValue FROM ");

        // copy the name of the table
        memcpy (iterator, database->name, db_len);
        iterator += db_len;

        // copy second part
        MYSQL_APPEND_LITERAL(iterator," WHERE Id='");

        // copy the key
        iterator += mysql_real_escape_string(
                    database->con, iterator, request->key, request->key_sz);

        // copy second part
        MYSQL_APPEND_LITERAL(iterator,"';");

        *iterator = 0;

        // perform this querry
        if (mysql_query(database->con, querry_buffer))
        {
            err_message (mysql_error(database->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // get the result
        result = mysql_store_result(database->con);
        if (result == NULL)
        {
            err_message (mysql_error(database->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // get characteristics
        MYSQL_ROW row = mysql_fetch_row (result);
        unsigned long *lengths = mysql_fetch_lengths(result);
        if (lengths == NULL) {
            err_message (mysql_error(database->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // resize
        ret = extend_buffer(&querry_buffer, &querry_buffer_sz, lengths[0]);
        if (ret != FUNC_OK)
            break;
        memcpy(querry_buffer, row[0], lengths[0]);
        vbuf = querry_buffer;
        vsiz = lengths[0];
        querry_buffer = NULL;

        break;
    }

    if (result != NULL) {
        mysql_free_result(result);
    }

    if (querry_buffer != NULL) {
        free (querry_buffer);
    }

    // either use the callback or set the value in request
    if (request->kb == NULL) {
        *request->val = vbuf;
        *request->val_sz = vsiz;
    } else {
        request->kb (
                    request->db,
                    ret,
                    request->user,
                    vbuf,
                    vsiz
                    );
    }
    return ret;
}

func_error_t aitown_db_mysql_write (aitown_db_write_t * request)
{
    func_error_t    ret = FUNC_OK;
    char * querry_buffer = NULL;
    char * iterator;
    size_t querry_buffer_sz;
    size_t querry_computed;

    for (;;) {

        // assume it's our kind
        aitown_db_mysql_t * database = (aitown_db_mysql_t*)request->db;
        DBG_ASSERT (database != NULL);
        DBG_ASSERT (database->con != NULL);

        // approximate our querry size
        size_t db_len = strlen(database->name);
        querry_computed =
                sizeof(MYSQL_Q_INSERT) +
                db_len +
                request->key_sz*2 +
                request->val_sz*2 +
                2;

        // allocate
        ret = extend_buffer(&querry_buffer, &querry_buffer_sz, querry_computed);
        if (ret != FUNC_OK)
            break;
        iterator = querry_buffer;

        // copy first part
        MYSQL_APPEND_LITERAL(iterator,"INSERT INTO ");

        // copy the name of the table
        memcpy (iterator, database->name, db_len );
        iterator += db_len;

        // copy second part
        MYSQL_APPEND_LITERAL(iterator," (Id, MasterValue) VALUES('");

        // copy the key
        iterator += mysql_real_escape_string(
                    database->con, iterator, request->key, request->key_sz);

        // copy separator
        MYSQL_APPEND_LITERAL(iterator,"','");

        // copy the value
        iterator += mysql_real_escape_string(
                    database->con, iterator, request->val, request->val_sz);

        // ending part
        MYSQL_APPEND_LITERAL(iterator,"') ON DUPLICATE KEY UPDATE MasterValue=values(MasterValue);");

        *iterator = 0;

        // perform this querry
        if (mysql_query(database->con, querry_buffer)) {
            err_message (mysql_error(database->con));
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        break;
    }

    if (querry_buffer != NULL) {
        free (querry_buffer);
    }

    // either use the callback or set the value in request
    if (request->kb != NULL) {
        request->kb (
                    request->db,
                    ret,
                    request->user
                    );
    }
    return ret;
}

func_error_t aitown_db_mysql_free_chunk (
        aitown_db_t*db, void * chunk)
{
    free (chunk);
    return FUNC_OK;
}

//func_error_t aitown_db_mysql_read_k64 (aitown_db_io_t * request)
//{
//    aitown_db_io_t internal = *request;
//    internal.key.ptr = &request->key.u64;
//    internal.key_sz = 8;
//    return aitown_db_mysql_read (&internal);
//}

//func_error_t aitown_db_mysql_write_k64 (aitown_db_io_t * request)
//{
//    aitown_db_io_t internal = *request;
//    internal.key.ptr = &request->key.u64;
//    internal.key_sz = 8;
//    return aitown_db_mysql_write (&internal);
//}

func_error_t aitown_db_mysql_init (aitown_db_mng_t *db_mng)
{

    // allocate memory for us
    aitown_db_driver_mysql_t * ret = (aitown_db_driver_mysql_t*)malloc (
                sizeof(aitown_db_driver_mysql_t));
    if (ret == NULL) {
        dbg_message ("Mysql failed to start due to a memory failure");
        return FUNC_MEMORY_ERROR;
    }

    // initialize it as a standard driver
    aitown_db_driver_init ((aitown_db_driver_t*)ret);
    ret->header.name = MYSQL_NAME;

    ret->header.open = aitown_db_mysql_open;
    ret->header.close = aitown_db_mysql_close;
    ret->header.read = aitown_db_mysql_read;
    ret->header.write = aitown_db_mysql_write;
    ret->header.free_chunk = aitown_db_mysql_free_chunk;
//    ret->header.read_k64 = aitown_db_mysql_read_k64;
//    ret->header.write_k64 = aitown_db_mysql_write_k64;

    // inform the manager about it
    aitown_db_driver_add (db_mng, (aitown_db_driver_t*)ret);
    return FUNC_OK;
}

void aitown_db_mysql_end (aitown_db_mng_t *db_mng)
{
    // ask the manager to remove us from the list
    aitown_db_driver_t * instance;
    if (FUNC_OK == aitown_db_driver_rem_n (
            db_mng, MYSQL_NAME, &instance))
    {
        aitown_db_driver_mysql_t * mysql = (aitown_db_driver_mysql_t*)instance;

        // terminate the structure
        aitown_db_driver_end (instance);

        // release memory used by the structure
        free (mysql);
    }

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


