aitown-image                         {#aitown_image}
============

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-image.html).





TODO: PixFC-SSE
---------------

Look at [code.google.com/p/pixfc-sse](http://code.google.com/p/pixfc-sse/)

PixFC-SSE is a cross-platform C library providing SSE-optimised functions to
convert between various RGB and YUV formats.

You specify the input & output formats, PixFC-SSE selects the best conversion
function to use based on the features supported by your CPU. In the worst case
scenario where the CPU does not support any SSE instructions, PixFC-SSE falls
back to a standard, non-SSE conversion function. This way, PixFC-SSE guarantees
the conversion will happen regardless of the platform. PixFC-SSE works on
Linux, Mac and Windows.


TODO: vectorial
---------------

Look at [github.com/scoopr/vectorial](https://github.com/scoopr/vectorial)

Vectorial - vector math library; supports NEON, SSE, scalar and generic gcc
vector extension. Most basic vector and matrix math is available, but not quite
yet full featured.


TODO: libimagequant
-------------------

Look at [pngquant.org/lib/](http://pngquant.org/lib/)

Small, portable C library for high-quality conversion of RGBA
images to 8-bit indexed-color (palette) images. It's powering pngquant2.

TODO: libdivide
---------------

Look at [github.com/ridiculousfish/libdivide](https://github.com/ridiculousfish/libdivide)



TODO: simd-cph
---------------

Look at [sourceforge.net/p/simd-cph](http://sourceforge.net/p/simd-cph)

Cross-platform SIMD C Headers
