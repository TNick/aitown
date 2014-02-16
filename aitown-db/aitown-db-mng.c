/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-mng.c
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

#include "aitown-db-mng.h"

#include <aitown/aitown-db.h>
#include <aitown/aitown-db-driver.h>

#include <stdlib.h>
#include <string.h>

#include <aitown/sglib.h>
#include <aitown/cfgpath.h>
#include <aitown/struct_ini.h>
#include <aitown/utils.h>
#include <aitown/dbg_assert.h>


#ifdef KYOTOCABINET_FOUND
#include "buildin/kyoto.h"
#endif // KYOTOCABINET_FOUND

#ifdef TOKYOCABINET_FOUND
#include "buildin/tokyo.h"
#endif // TOKYOCABINET_FOUND

#ifdef MYSQL_FOUND
#include "buildin/mysql-local.h"
#endif // MYSQL_FOUND

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define DRIVER_COMPARATOR(a,b) (strcmp(a->name, b->name))
SGLIB_DEFINE_LIST_PROTOTYPES (aitown_db_driver_t,DRIVER_COMPARATOR, next)
SGLIB_DEFINE_LIST_FUNCTIONS (aitown_db_driver_t,DRIVER_COMPARATOR, next)

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

void aitown_db_mng_init (aitown_db_mng_t *db_mng)
{

    // clear the structure
    memset (db_mng, 0, sizeof(aitown_db_mng_t));

    // add build-in implementations
#   ifdef KYOTOCABINET_FOUND
    aitown_db_kyoto_init (db_mng);
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    aitown_db_tokyo_init (db_mng);
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    aitown_db_mysql_init (db_mng);
#   endif // MYSQL_FOUND


}

void aitown_db_mng_end (aitown_db_mng_t *db_mng)
{

    // remove build-in implementations
#   ifdef KYOTOCABINET_FOUND
    aitown_db_kyoto_init (db_mng);
#   endif // KYOTOCABINET_FOUND

#   ifdef TOKYOCABINET_FOUND
    aitown_db_tokyo_end (db_mng);
#   endif // TOKYOCABINET_FOUND

#   ifdef MYSQL_FOUND
    aitown_db_mysql_end (db_mng);
#   endif // MYSQL_FOUND


    // clear the structure
    memset (db_mng, 0, sizeof(aitown_db_mng_t));
}

static func_error_t aitown_db_read_ini (
        aitown_db_open_t * data, struct_ini_t *sini)
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // there must be a driver name
        size_t drv_len = strlen (data->driver);
        if (drv_len == 0) {
            err_message ("A driver name must be provided for any database");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // the path to config file
        char computed_name[MAX_PATH];
        int tolerant;
        const char * cfg_path;
        if (data->cfg_file == NULL) {
            computed_name[0] = 0;
            get_user_config_folder (computed_name, MAX_PATH-drv_len-1, "aitown_db");
            size_t cpth = strlen (computed_name);
            memcpy (&computed_name[cpth], data->driver, drv_len+1);
            tolerant = 1;
        } else {
            cfg_path = data->cfg_file;
            tolerant = 0;
        }

        // parse it
        int sini_err = struct_ini_init_and_parse (sini, cfg_path);
        if ((0 != sini_err) && (!tolerant)) {
            err_message ("Failed to parse configuration file (error code: %d)", sini_err);
            ret = FUNC_BAD_INPUT;
            break;
        }

        break;
    }
    return ret;

}

func_error_t aitown_db_open (aitown_db_open_t * original_data)
{
    DBG_ASSERT (original_data->db_mng != NULL);

    aitown_db_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // parse the config file
        struct_ini_t    sini;
        ret = aitown_db_read_ini (&data, &sini);
        if (ret != FUNC_OK) {
            break;
        }

        // the driver must be loaded
        aitown_db_driver_t * db_driver;
        if (aitown_db_mng_driver_find (
                data.db_mng, data.driver, &db_driver)) {
            err_message ("No database driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        data.cfg = &sini;
        ret = db_driver->open (&data);
        if (ret != FUNC_OK) {
            break;
        }

        // save the driver
        aitown_db_t * outdb = *original_data->out;
        outdb->driver = db_driver;

        break;
    }
    return ret;
}

func_error_t aitown_db_cfgopen (aitown_db_open_t * original_data)
{
    DBG_ASSERT (original_data->db_mng != NULL);

    aitown_db_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // the driver must be loaded
        aitown_db_driver_t * db_driver;
        if (aitown_db_mng_driver_find (
                data.db_mng, data.driver, &db_driver)) {
            err_message ("No database driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        ret = db_driver->open (&data);

        break;
    }
    original_data->out = data.out;
    return ret;
}

func_error_t aitown_db_autopen (aitown_db_open_t * original_data)
{
    DBG_ASSERT (original_data->db_mng != NULL);

    aitown_db_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // parse the config file
        struct_ini_t    sini;
        ret = aitown_db_read_ini (&data, &sini);
        if (ret != FUNC_OK) {
            break;
        }

        // get general section and database_driver key
        struct_ini_sect_t * sect_general =
            struct_ini_find_section_0 (&sini, "general");
        if (sect_general == NULL) {
            err_message ("GENERAL section not found in config file");
            break;
        }

        // get the driver to use
        struct_ini_value_t * database_driver =
            struct_ini_find_value_0 (sect_general, "database_driver");
        if ((database_driver == NULL) || (database_driver->value == NULL)) {
            err_message ("GENERAL/database_driver key not found in config file");
            break;
        }

        // the driver must be loaded
        aitown_db_driver_t * db_driver;
        if (aitown_db_mng_driver_find (
                data.db_mng, data.driver, &db_driver)) {
            err_message ("No database driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        data.cfg = &sini;
        ret = db_driver->open (&data);

        break;
    }
    original_data->out = data.out;
    return ret;
}

void aitown_db_close (aitown_db_mng_t * db_mng, aitown_db_t** db)
{
    DBG_ASSERT (db_mng != NULL);
    DBG_ASSERT (db != NULL);
    aitown_db_t *database = *db;

    if (database != NULL) {
        DBG_ASSERT (database->driver != NULL);
        DBG_ASSERT (database->driver->close != NULL);
        if (FUNC_OK == database->driver->close (db_mng, database)) {
            *db = NULL;
        }
    }
}

func_error_t aitown_db_cwrite (aitown_db_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->db_mng != NULL);
    DBG_ASSERT (request->db != NULL);
    DBG_ASSERT (request->db->driver != NULL);
    DBG_ASSERT (request->key.ptr != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        DBG_ASSERT (request->db->driver->write != NULL);
        ret = request->db->driver->write (request);
        break;
    }
    return ret;
}

func_error_t aitown_db_cwrite_k64 (aitown_db_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->db_mng != NULL);
    DBG_ASSERT (request->db != NULL);
    DBG_ASSERT (request->db->driver != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        if (request->db->driver->read_k64 == NULL) {
            ret = FUNC_NOT_IMPLEMENTED;
        } else {
            ret = request->db->driver->write_k64 (request);
        }
        break;
    }
    return ret;
}

func_error_t aitown_db_cread (aitown_db_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->db_mng != NULL);
    DBG_ASSERT (request->db != NULL);
    DBG_ASSERT (request->db->driver != NULL);
    DBG_ASSERT (request->key.ptr != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        DBG_ASSERT (request->db->driver->read != NULL);
        ret = request->db->driver->read (request);
        break;
    }
    return ret;
}

func_error_t aitown_db_cread_k64 (aitown_db_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->db_mng != NULL);
    DBG_ASSERT (request->db != NULL);
    DBG_ASSERT (request->db->driver != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        if (request->db->driver->read_k64 == NULL) {
            ret = FUNC_NOT_IMPLEMENTED;
        } else {
            ret = request->db->driver->read_k64 (request);
        }
        break;
    }
    return ret;
}



void aitown_db_free (
        aitown_db_mng_t * db_mng, aitown_db_t*db,
        const void ** chunk)
{
    DBG_ASSERT (chunk != NULL);
    DBG_ASSERT (db_mng != NULL);
    DBG_ASSERT (db != NULL);
    DBG_ASSERT (db->driver != NULL);
    DBG_ASSERT (db->driver->free_chunk != NULL);

    if (*chunk == NULL) {
        return;
    }

    if (FUNC_OK == db->driver->free_chunk (db_mng, db, (void*)*chunk)) {
        *chunk = NULL;
    }

}

func_error_t aitown_db_mng_driver_add (
        aitown_db_mng_t * db_mng, aitown_db_driver_t * db_driver )
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // make sure it implements required callbacks
        if ( (db_driver->open == NULL) ||
             (db_driver->close == NULL) ||
             (db_driver->read == NULL) ||
             (db_driver->write == NULL) ||
             (db_driver->free_chunk == NULL) ||
             (db_driver->name == NULL) ) {

            err_message ("Driver %s does not meet minimum requirements", db_driver->name);
            break;
        }

        // make sure parameters are ok
        if ((db_mng == NULL) || (db_driver == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // add if not already there
        aitown_db_driver_t * found;
        if (!sglib_aitown_db_driver_t_add_if_not_member (
                &db_mng->first_driver, db_driver, &found)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        break;
    }
    return ret;
}

func_error_t aitown_db_mng_driver_rem (
        aitown_db_mng_t * db_mng, aitown_db_driver_t * db_driver )
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((db_mng == NULL) || (db_driver == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // make sure is a member
        if (!sglib_aitown_db_driver_t_is_member (
                db_mng->first_driver, db_driver)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // remove it
        sglib_aitown_db_driver_t_delete (&db_mng->first_driver, db_driver);
        break;
    }
    return ret;
}

func_error_t aitown_db_mng_driver_rem_n (
        aitown_db_mng_t * db_mng, const char* driver_name,
        aitown_db_driver_t ** instance)
{
    func_error_t ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((db_mng == NULL) || (driver_name == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // remove it
        aitown_db_driver_t fake;
        fake.name = driver_name;
        fake.next = NULL;
        if (!sglib_aitown_db_driver_t_delete_if_member (
                &db_mng->first_driver, &fake, instance)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        break;
    }
    return ret;
}

func_error_t aitown_db_mng_driver_find (
        aitown_db_mng_t * db_mng, const char * driver_name,
        aitown_db_driver_t ** db_driver )
{
    func_error_t ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((db_mng == NULL) || (driver_name == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // find it
        aitown_db_driver_t fake;
        fake.name = driver_name;
        fake.next = NULL;
        *db_driver = sglib_aitown_db_driver_t_find_member (
                db_mng->first_driver, &fake);
        if (*db_driver == NULL) {
            ret = FUNC_BAD_INPUT;
        }

        break;
    }
    return ret;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


