/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			sensor.c
  \date			September 2013
  \author		TNick

  \brief		implementation of an sensor


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
#include <string.h>
#include <aitown/pointer_aritmetic.h>
#include <aitown/utils_unused.h>
#include <aitown/aitown-image.h>

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


IOBASE_DEFINE_FUNCTIONS (sensor)

void kb_aitown_core_sensor_image (
        aitown_core_t* core, core_sensor_t* sensor, const void * data)
{
    DBG_ASSERT (core != NULL);
    DBG_ASSERT (sensor != NULL);
    DBG_ASSERT (data != NULL);
    core_sensor_image_t * s2 = (core_sensor_image_t*)sensor;

    // fake image for imput
    aitimage_t src;
    src.width = s2->greyi.width;
    src.height = s2->greyi.height;
    src.type = AITIMAGE_RGBA8;
    src.bufsz = AITIMAGE_GET_RGBA8_SIZE (&src);

    // get the integral grey representation
    func_error_t ret = aitimage_greyi_from_rgba8_ptr (
                &src,
                &s2->greyi,
                data,
                (double *)AITIMAGE_GET_DATA (&s2->greyi)
                );
    DBG_ASSERT (ret == FUNC_OK);
    VAR_UNUSED (ret);

    // TODO: locate sudden changes in data

    // TODO: identify characteristic points based on attention

    // TODO: match points against known ids; attribute new ids

    // TODO: send the list of ids to the visual cortex

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
