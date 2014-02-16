/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			sensor_mng.c
  \date			September 2013
  \author		TNick

  \brief		implementation of an sensor_mng


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

#include "sensor_mng.h"
#include <aitown/aitown-core.h>
#include <aitown/aitown-image.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define SENSOR_COMPARATOR(a,b) (((int64_t)a) - ((int64_t)b))
SGLIB_DEFINE_LIST_PROTOTYPES (core_sensor_t,SENSOR_COMPARATOR, next)
SGLIB_DEFINE_LIST_FUNCTIONS (core_sensor_t,SENSOR_COMPARATOR, next)

IOBASE_MNG_DEFINE_FUNCTIONS (sensor)

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

func_error_t core_sensor_mng_add_image (
        core_sensor_mng_t * sensor_mng, const char *name,
        const char *description, unsigned width, unsigned height,
        struct _core_sensor_t ** out)
{
    DBG_ASSERT (sensor_mng != NULL);
    DBG_ASSERT (out != NULL);
    DBG_ASSERT ((width > 0) && (width <= 1024));
    DBG_ASSERT ((height > 0) && (height <= 1024));

    // compute the size of the sensor chunk
    //      core_sensor_image_t
    //      buffer for greyi
    unsigned grey_sz = AITIMAGE_GREYI_SIZE(width, height);
    unsigned sz = sizeof(core_sensor_image_t) + grey_sz;

    // allocate this chunk
    func_error_t ret = core_sensor_init(
                sz, name, description, (core_sensor_t**)out);
    if (ret != FUNC_OK) {
        return ret;
    }

    // fill specific information
    core_sensor_image_t * s = (core_sensor_image_t *)*out;
    s->header.kb = kb_aitown_core_sensor_image;
    aitimage_init (&s->greyi, width, height, AITIMAGE_GREYI, grey_sz);

    // add it to the chain
    core_sensor_mng_add ( sensor_mng, *out);
    return ret;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
