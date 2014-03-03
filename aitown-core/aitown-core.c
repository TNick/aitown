/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-core.c
  \date			September 2013
  \author		TNick

  \brief		implementation of the core aitown functionality


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

#include "aitown-core.h"
#include <aitown/dbg_assert.h>
#include <aitown/utils.h>

#include <string.h>

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




func_error_t aitown_core_init (
        aitown_core_t *core, aitown_cfg_sect_t * cfg_sect)
{
    func_error_t ret = FUNC_OK;

    for (;;) {

        // initialise
        memset (core, 0, sizeof(aitown_core_t));

        // start storage, including database
        ret = aitown_dstorage_init (&core->dstore, cfg_sect);
        if (ret != FUNC_OK) break;

        // start the brain, sensors and actuators
        core_brain_init (&core->brain);
        core_actuator_mng_init (&core->amng);
        core_sensor_mng_init (&core->smng);

        break;
    }

    return ret;
}

func_error_t aitown_core_finit (
        aitown_core_t * core, const char * file)
{
    func_error_t ret;

    // load config file
    aitown_cfg_t * cfg;
    ret = aitown_cfg_init (&cfg, file);
    if (ret != FUNC_OK) {
        return ret;
    }

    // load manager from the root section
    ret = aitown_core_init (core, &cfg->root);

    // release our reference (works both if previous call succeded or not)
    aitown_cfg_decref (cfg, cfg);

    core->manage_cfg = 1;

    return ret;
}

void aitown_core_end (aitown_core_t *core)
{
    // stop the brain, sensors and actuators
    core_sensor_mng_end (&core->smng);
    core_actuator_mng_end (&core->amng);
    core_brain_end (&core->brain);

    // stop storage, including database
   // aitown_dstorage_end (&core->dstore);

    // clear the structure
    memset (core, 0, sizeof(aitown_core_t));
}

void aitown_core_tick (aitown_core_t *core)
{
    /** @todo */
}

func_error_t aitown_core_add_sensor (
        aitown_core_t *core, const char *name, const char *description,
        unsigned dimension, unsigned *sizes, core_sensor_t ** out)
{
    DBG_ASSERT (core != NULL);
    DBG_ASSERT (out != NULL);

    // use dedicated methods to create the sensor
    switch (dimension) {
    case 0: return core_sensor_mng_add_0 (&core->smng, name, description, out);
    case 1: return core_sensor_mng_add_1 (&core->smng, name, description, sizes[0], out);
    case 2: return core_sensor_mng_add_2 (&core->smng, name, description, sizes, out);
    default: {
        log_message ("Can only create sensors of 0, 1 or 2 dimensions");
        return FUNC_MEMORY_ERROR;
    }
    }
}

func_error_t aitown_core_add_image_sensor (
        aitown_core_t *core, const char *name, const char *description,
        unsigned width, unsigned height, core_sensor_t ** out)
{
    DBG_ASSERT (core != NULL);
    DBG_ASSERT (out != NULL);

    return core_sensor_mng_add_image (
                &core->smng, name, description, width, height, out);

}


func_error_t aitown_core_add_actuator (
        aitown_core_t *core, const char *name, const char *description,
        unsigned dimension, unsigned *sizes, core_actuator_t ** out)
{
    DBG_ASSERT (core != NULL);
    DBG_ASSERT (out != NULL);

    // use dedicated methods to create the actuator
    switch (dimension) {
    case 0: return core_actuator_mng_add_0 (&core->amng, name, description, out);
    case 1: return core_actuator_mng_add_1 (&core->amng, name, description, sizes[0], out);
    case 2: return core_actuator_mng_add_2 (&core->amng, name, description, sizes, out);
    default: {
        log_message ("Can only create actuators of 0, 1 or 2 dimensions");
        return FUNC_MEMORY_ERROR;
    }
    }

    return FUNC_OK;
}

func_error_t aitown_core_rem_sensor (
        aitown_core_t *core, core_sensor_t ** out)
{
    return core_sensor_mng_rem (&core->smng, out);
}

func_error_t aitown_core_rem_actuator (
        aitown_core_t *core, core_actuator_t ** out)
{
    return core_actuator_mng_rem (&core->amng, out);
}


func_error_t aitown_core_disable_sensor (
        aitown_core_t *core, core_sensor_t * out)
{
    return core_sensor_mng_enable (&core->smng, out, 0);
}

func_error_t aitown_core_disable_actuator (
        aitown_core_t *core, core_actuator_t * out)
{
    return core_actuator_mng_enable (&core->amng, out, 0);
}

func_error_t aitown_core_enable_sensor (
        aitown_core_t *core, core_sensor_t * out)
{
    return core_sensor_mng_enable (&core->smng, out, 1);
}

func_error_t aitown_core_enable_actuator (
        aitown_core_t *core, core_actuator_t * out)
{
    return core_actuator_mng_enable (&core->amng, out, 1);
}

core_sensor_t * aitown_core_find_sensor ( aitown_core_t *core, const char *name)
{
    return core_sensor_mng_byname (&core->smng, name);
}

core_actuator_t * aitown_core_find_actuator (
        aitown_core_t *core, const char *name)
{
    return core_actuator_mng_byname (&core->amng, name);
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
