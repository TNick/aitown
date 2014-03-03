aitown-dejavu                         {#aitown_dejavu}
=============

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-dejavu.html).

The main structure for the module is aitown_dejavu_t. It must be prepared prior
to use and terminated when no longer needed:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
aitown_dejavu_t dejavu;
aitown_dejavu_finit (&dejavu, settings_file);
// ...
aitown_dejavu_end (&dejavu);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To send data to the sensor use aitown_dejavu_feed() function with an
aitimage_t as parameter.

Attention Rectangle \f$\mathcal{AR}\f$ is represented by a aitown_dejavu_area_t.


Implementation notes
--------------------

The implementation is open for optimisation. A naive implementation was
partially implemented to allow testing the concept.

Sudden changes are handles by the aitown_dejavu_change_t substructure
and associated functions:

- aitown_dejavu_change_init()
- aitown_dejavu_change_end()
- aitown_dejavu_change_reinit()
- aitown_dejavu_change_detect()

There are for cases at the top level: the image is either larger
than our grid, one side or the other is smaller or both sides are smaller.
Only first and last cases were implemented.


Tunning
-------

Current implementation has a sequential
implementation for sudden changes / ar processing,
with OpenMP being first choice for paralelization.


A set of definitions control the way this libray is generated:

<dl>
    <dt>AITOWN_DEJAVU_INPUT_IS_VARIABLE</dt>
    <dd>
        decides wheter aitown_dejavu_feed() checks on every call the
        size of the input and adjusts internal workings if a change is
        detected.
    </dd>
    <dt>AITOWN_DEJAVU_INPUT_FIX_WIDTH</dt>
    <dd>
        if defined, it must be a positive integer indicating the width of the
        input image; if undefined or 0 the width is assumed to be either
        set at initialisation time via aitown_dejavu_init() or fully variable.
    </dd>
    <dt>AITOWN_DEJAVU_INPUT_FIX_HEIGHT </dt>
    <dd>
        same as AITOWN_DEJAVU_INPUT_FIX_WIDTH;
    </dd>
    <dt>AITOWN_DEJAVU_GRID_IS_SQUARE</dt>
    <dd>
        tells if the \f$\mathcal{AR}\f$ is a square (w=h) or a rectangle
    </dd>
</dl>

