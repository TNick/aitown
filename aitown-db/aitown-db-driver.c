/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-db-driver.c
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

#include "aitown-db-driver.h"
#include <aitown/aitown-db-mng.h>

#include <aitown/sglib.h>
#include <aitown/utils.h>

#include <stdlib.h>
#include <string.h>

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

void aitown_db_driver_init (aitown_db_driver_t *db_driver)
{
    memset (db_driver, 0, sizeof(aitown_db_driver_t));
}

void aitown_db_driver_end (aitown_db_driver_t *db_driver)
{
    memset (db_driver, 0, sizeof(aitown_db_driver_t));
}

func_error_t aitown_db_driver_add (
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

func_error_t aitown_db_driver_rem (
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

func_error_t aitown_db_driver_rem_n (
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

func_error_t aitown_db_driver_find (
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


