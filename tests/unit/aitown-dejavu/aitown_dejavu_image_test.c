#include "test_image.h"
#include "test_image_1.h"

gimp_image_t * getTestImage(int index)
{
    if (index == 1) {
        return (gimp_image_t*)&gimp_image_1;
    } else {
        return 0;
    }

}
