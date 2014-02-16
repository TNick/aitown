/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-compress-mng.c
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please shrink COPYING and SHRINKME files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "aitown-compress-mng.h"

#include <aitown/aitown-compress.h>
#include <aitown/aitown-compress-driver.h>

#include <stdlib.h>
#include <string.h>

#include <aitown/sglib.h>
#include <aitown/cfgpath.h>
#include <aitown/struct_ini.h>
#include <aitown/utils.h>
#include <aitown/dbg_assert.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define DRIVER_COMPARATOR(a,b) (strcmp(a->name, b->name))
SGLIB_DEFINE_LIST_PROTOTYPES (aitown_compress_driver_t,DRIVER_COMPARATOR, next)
SGLIB_DEFINE_LIST_FUNCTIONS (aitown_compress_driver_t,DRIVER_COMPARATOR, next)

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

void aitown_compress_mng_init (aitown_compress_mng_t *compress_mng)
{

    // clear the structure
    memset (compress_mng, 0, sizeof(aitown_compress_mng_t));

    // add build-in implementations



}

void aitown_compress_mng_end (aitown_compress_mng_t *compress_mng)
{


    // clear the structure
    memset (compress_mng, 0, sizeof(aitown_compress_mng_t));
}

static func_error_t aitown_compress_shrink_ini (
        aitown_compress_open_t * data, struct_ini_t *sini)
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // there must be a driver name
        size_t drv_len = strlen (data->driver);
        if (drv_len == 0) {
            err_message ("A driver name must be provided for any compress");
            ret = FUNC_BAD_INPUT;
            break;
        }

        // the path to config file
        char computed_name[MAX_PATH];
        int tolerant;
        const char * cfg_path;
        if (data->cfg_file == NULL) {
            computed_name[0] = 0;
            get_user_config_folder (computed_name, MAX_PATH-drv_len-1, "aitown_compress");
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

func_error_t aitown_compress_open (aitown_compress_open_t * original_data)
{
    DBG_ASSERT (original_data->compress_mng != NULL);

    aitown_compress_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // parse the config file
        struct_ini_t    sini;
        ret = aitown_compress_shrink_ini (&data, &sini);
        if (ret != FUNC_OK) {
            break;
        }

        // the driver must be loaded
        aitown_compress_driver_t * compress_driver;
        if (aitown_compress_mng_driver_find (
                data.compress_mng, data.driver, &compress_driver)) {
            err_message ("No compress driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        data.cfg = &sini;
        ret = compress_driver->open (&data);
        if (ret != FUNC_OK) {
            break;
        }

        // save the driver
        aitown_compress_t * outcompress = *original_data->out;
        outcompress->driver = compress_driver;

        break;
    }
    return ret;
}

func_error_t aitown_compress_cfgopen (aitown_compress_open_t * original_data)
{
    DBG_ASSERT (original_data->compress_mng != NULL);

    aitown_compress_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // the driver must be loaded
        aitown_compress_driver_t * compress_driver;
        if (aitown_compress_mng_driver_find (
                data.compress_mng, data.driver, &compress_driver)) {
            err_message ("No compress driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        ret = compress_driver->open (&data);

        break;
    }
    original_data->out = data.out;
    return ret;
}

func_error_t aitown_compress_autopen (aitown_compress_open_t * original_data)
{
    DBG_ASSERT (original_data->compress_mng != NULL);

    aitown_compress_open_t data = *original_data;
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // parse the config file
        struct_ini_t    sini;
        ret = aitown_compress_shrink_ini (&data, &sini);
        if (ret != FUNC_OK) {
            break;
        }

        // get general section and compress_driver key
        struct_ini_sect_t * sect_general =
            struct_ini_find_section_0 (&sini, "general");
        if (sect_general == NULL) {
            err_message ("GENERAL section not found in config file");
            break;
        }

        // get the driver to use
        struct_ini_value_t * compress_driver_val =
            struct_ini_find_value_0 (sect_general, "compress_driver");
        if ((compress_driver_val == NULL) || (compress_driver_val->value == NULL)) {
            err_message ("GENERAL/compress_driver key not found in config file");
            break;
        }

        // the driver must be loaded
        aitown_compress_driver_t * compress_driver;
        if (aitown_compress_mng_driver_find (
                data.compress_mng, compress_driver_val->value, &compress_driver)) {
            err_message ("No compress driver named <%s> is loaded", data.driver);
            ret = FUNC_BAD_INPUT;
            break;
        }

        // call the driver load function with this information
        data.cfg = &sini;
        ret = compress_driver->open (&data);

        break;
    }
    original_data->out = data.out;
    return ret;
}

void aitown_compress_close (aitown_compress_mng_t * compress_mng, aitown_compress_t** compress)
{
    DBG_ASSERT (compress_mng != NULL);
    DBG_ASSERT (compress != NULL);
    aitown_compress_t *c = *compress;

    if (compress != NULL) {
        DBG_ASSERT (c->driver != NULL);
        DBG_ASSERT (c->driver->close != NULL);
        if (FUNC_OK == c->driver->close (compress_mng, c)) {
            *compress = NULL;
        }
    }
}

func_error_t aitown_compress_cexpand (aitown_compress_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->compress_mng != NULL);
    DBG_ASSERT (request->compress != NULL);
    DBG_ASSERT (request->compress->driver != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        DBG_ASSERT (request->compress->driver->expand != NULL);
        ret = request->compress->driver->expand (request);
        break;
    }
    return ret;
}

func_error_t aitown_compress_cshrink (aitown_compress_io_t * request)
{
    DBG_ASSERT (request != NULL);
    DBG_ASSERT (request->compress_mng != NULL);
    DBG_ASSERT (request->compress != NULL);
    DBG_ASSERT (request->compress->driver != NULL);

    func_error_t    ret = FUNC_OK;
    for (;;) {
        DBG_ASSERT (request->compress->driver->shrink != NULL);
        ret = request->compress->driver->shrink (request);
        break;
    }
    return ret;
}


void aitown_compress_free (
        aitown_compress_mng_t * compress_mng, aitown_compress_t*compress,
        const void ** chunk)
{
    DBG_ASSERT (chunk != NULL);
    DBG_ASSERT (compress_mng != NULL);
    DBG_ASSERT (compress != NULL);
    DBG_ASSERT (compress->driver != NULL);
    DBG_ASSERT (compress->driver->free_chunk != NULL);

    if (*chunk == NULL) {
        return;
    }

    if (FUNC_OK == compress->driver->free_chunk (compress_mng, compress, (void*)*chunk)) {
        *chunk = NULL;
    }

}

func_error_t aitown_compress_mng_driver_add (
        aitown_compress_mng_t * compress_mng, aitown_compress_driver_t * compress_driver )
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // make sure it implements required callbacks
        if ( (compress_driver->open == NULL) ||
             (compress_driver->close == NULL) ||
             (compress_driver->shrink == NULL) ||
             (compress_driver->expand == NULL) ||
             (compress_driver->free_chunk == NULL) ||
             (compress_driver->name == NULL) ) {

            err_message ("Driver %s does not meet minimum requirements", compress_driver->name);
            break;
        }

        // make sure parameters are ok
        if ((compress_mng == NULL) || (compress_driver == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // add if not alshrinky there
        aitown_compress_driver_t * found;
        if (!sglib_aitown_compress_driver_t_add_if_not_member (
                &compress_mng->first_driver, compress_driver, &found)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        break;
    }
    return ret;
}

func_error_t aitown_compress_mng_driver_rem (
        aitown_compress_mng_t * compress_mng, aitown_compress_driver_t * compress_driver )
{
    func_error_t    ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((compress_mng == NULL) || (compress_driver == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // make sure is a member
        if (!sglib_aitown_compress_driver_t_is_member (
                compress_mng->first_driver, compress_driver)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // remove it
        sglib_aitown_compress_driver_t_delete (&compress_mng->first_driver, compress_driver);
        break;
    }
    return ret;
}

func_error_t aitown_compress_mng_driver_rem_n (
        aitown_compress_mng_t * compress_mng, const char* driver_name,
        aitown_compress_driver_t ** instance)
{
    func_error_t ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((compress_mng == NULL) || (driver_name == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // remove it
        aitown_compress_driver_t fake;
        fake.name = driver_name;
        fake.next = NULL;
        if (!sglib_aitown_compress_driver_t_delete_if_member (
                &compress_mng->first_driver, &fake, instance)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        break;
    }
    return ret;
}

func_error_t aitown_compress_mng_driver_find (
        aitown_compress_mng_t * compress_mng, const char * driver_name,
        aitown_compress_driver_t ** compress_driver )
{
    func_error_t ret = FUNC_OK;
    for (;;) {

        // make sure parameters are ok
        if ((compress_mng == NULL) || (driver_name == NULL)) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // find it
        aitown_compress_driver_t fake;
        fake.name = driver_name;
        fake.next = NULL;
        *compress_driver = sglib_aitown_compress_driver_t_find_member (
                compress_mng->first_driver, &fake);
        if (*compress_driver == NULL) {
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


