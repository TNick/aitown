/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-core.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_aitown_core_h_INCLUDE
#define AITOWN_aitown_core_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/error_codes.h>
#include <aitown/brain.h>
#include <aitown/actuator.h>
#include <aitown/sensor.h>
#include <aitown/actuator_mng.h>
#include <aitown/sensor_mng.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


//! an actuator (outgoing connection from a brain)
///
typedef struct _aitown_core_t {
    core_brain_t            brain;      /**< the brain object */
    core_actuator_mng_t     amng;       /**< manager for actuators */
    core_sensor_mng_t       smng;       /**< manager for sensors */
} aitown_core_t;

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

//! initialize core
AITOWN_EXPORT void
aitown_core_init (aitown_core_t *core);

//! terminate core
AITOWN_EXPORT void
aitown_core_end (aitown_core_t *core);


//! perform some opperations and return
AITOWN_EXPORT void
aitown_core_tick (aitown_core_t *core);


//! add a new sensor
AITOWN_EXPORT func_error_t
aitown_core_add_sensor (
        aitown_core_t *core,
        const char *name,
        const char *description,
        unsigned dimension,
        unsigned *sizes,
        core_sensor_t ** out);

//! add a new image sensor
AITOWN_EXPORT func_error_t
aitown_core_add_image_sensor (
        aitown_core_t *core,
        const char *name,
        const char *description,
        unsigned width,
        unsigned height,
        core_sensor_t ** out);

//! add a new sensor; it will provide a stream of 64-bit integers
static inline func_error_t
aitown_core_add_sensor_0 (
        aitown_core_t *core,
        const char *name,
        const char *description,
        core_sensor_t ** out)
{
    return aitown_core_add_sensor (core, name, description, 0, NULL, out);
}

//! add a new sensor; it will provide a stream of 64-bit vectors
static inline func_error_t
aitown_core_add_sensor_1 (
        aitown_core_t *core,
        const char *name,
        const char *description,
        unsigned size,
        core_sensor_t ** out)
{
    return aitown_core_add_sensor (core, name, description, 1, &size, out);
}

//! add a new sensor; it will provide a stream of 64-bit matrices
static inline func_error_t
aitown_core_add_sensor_2 (
        aitown_core_t *core,
        const char *name,
        const char *description,
        unsigned size1,
        unsigned size2,
        core_sensor_t ** out)
{
    unsigned sizes[2];
    sizes[0] = size1;
    sizes[1] = size2;
    return aitown_core_add_sensor (core, name, description, 2, sizes, out);
}


//! add a new actuator
AITOWN_EXPORT func_error_t
aitown_core_add_actuator (
        aitown_core_t *core,
        const char *name,
        const char *description,
        unsigned dimension,
        unsigned *sizes,
        core_actuator_t ** out);


//! remove a sensor
AITOWN_EXPORT func_error_t
aitown_core_rem_sensor (
        aitown_core_t *core,
        core_sensor_t ** out);

//! remove an actuator
AITOWN_EXPORT func_error_t
aitown_core_rem_actuator (
        aitown_core_t *core,
        core_actuator_t ** out);


//! disable a sensor
AITOWN_EXPORT func_error_t
aitown_core_disable_sensor (
        aitown_core_t *core,
        core_sensor_t * out);

//! disable an actuator
AITOWN_EXPORT func_error_t
aitown_core_disable_actuator (
        aitown_core_t *core,
        core_actuator_t * out);

//! enable a sensor
AITOWN_EXPORT func_error_t
aitown_core_enable_sensor (
        aitown_core_t *core,
        core_sensor_t * out);

//! enable an actuator
AITOWN_EXPORT func_error_t
aitown_core_enable_actuator (
        aitown_core_t *core,
        core_actuator_t * out);


//! find a sensor by name
AITOWN_EXPORT core_sensor_t *
aitown_core_find_sensor (
        aitown_core_t *core,
        const char *name);

//! find an actuator by name
AITOWN_EXPORT core_actuator_t *
aitown_core_find_actuator (
        aitown_core_t *core,
        const char *name);


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
#endif // AITOWN_aitown_core_h_INCLUDE
