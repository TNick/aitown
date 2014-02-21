aitown-dejavu                         {#aitown_dejavu}
=============

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-dejavu.html).

The main structure for the module is aitown_dejavu_t. It must be prepared prior
to use and terminated when no longer needed:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
aitown_dejavu_t dejavu;
aitown_dejavu_init (&dejavu);
// ...
aitown_dejavu_end (&dejavu);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
