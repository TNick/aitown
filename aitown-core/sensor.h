/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			sensor.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_core_sensor_h_INCLUDE
#define AITOWN_core_sensor_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown-image.h>
#include <aitown/iobase.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitown_core_t;

IOBASE_DEFINE_PROTOTYPES(sensor)

//! an image sensor
/// the buffer for aitimage_t follows
typedef struct _core_sensor_image_t {
    core_sensor_t           header;     /**< common properties */
    aitimage_t              greyi;      /**< the image converted to integral grey */
} core_sensor_image_t; \

//! get a pointer to grey data
#define CORE_SENSOR_GREYI_IMAGE_DATA(__s__) PTR_ADD((__s__),sizeof(core_sensor_image_t))

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

//! process data from a video sensor
void kb_aitown_core_sensor_image (struct _aitown_core_t* core, core_sensor_t* sensor, const void * data);

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif // AITOWN_core_sensor_h_INCLUDE
