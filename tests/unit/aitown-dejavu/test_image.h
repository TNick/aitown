
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _gimp_image_t{
    unsigned int 	 width;
    unsigned int 	 height;
    unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
    unsigned char	 pixel_data[1];
} gimp_image_t;
#define GIMP_IMAGE_GET_DATA(__p__) (&(__p__)->pixel_data)

gimp_image_t * getTestImage(int index);

#ifdef __cplusplus
}
#endif
