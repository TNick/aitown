aitown-utils                         {#aitown_utils}
============

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-utils.html).


accumulator
-----------

The accumulator_t structure and associated functions implement an extendable
buffer. On accumulator_alloc() calls memory is allocated at the end of the
buffer if there is enough space, otherwise the buffer is reallocated  to be
large enough. Note that this implementation is not particulary fast, as
sometimes (most of the times) realloc() will have to allocated a new
chunk, copy old content, then free old chunk.
The acumulator allows for a customised increase step.


character buffer
----------------

char_buff_t structure and functions are very similar to accumulator above.
The increase step is fixed.

debug assertions
----------------

DBG_ASSERT macro produces no code in release builds (tracks _DEBUG define).


iterating in directories
------------------------

dir_iterator() function allows a customised iteration in files and/or in
sub-directories (may also be recursive).


directory utilities
-------------------

A set of cross-platform utilities are provided, including:
changing and retreiving current directory, creating deleting and clearing
directories, checking files and directories for existence.


string lists
------------

An sglib-based linked list of strings.


pivotal gmtime
--------------

pivotal_gmtime_r is a replacement for gmtime/localtime/mktime that works around
the 2038 bug on 32-bit systems. See
[its documentation](http://2038bug.com/pivotal_gmtime_doc.html).

Copyright (C) 2009  Paul Sheer. Terms and conditions apply.
Visit [2038bug.com](http://2038bug.com/) for more info.


pointer aritmetic
-----------------

Treat any pointer as a char pointer with PTR_ADD; get the offset
inside a buffer with PTR_OFF; round a buffer size for pointers with ROUND_TO_PTR.


stack buffer
------------

Defines STACKBUFF_INIT and STACKBUFF_END. STACKBUFF_INIT defines a
fixed size array in the stack; if run-time size is larger than allocated one
a buffer is malloc()'ed, otherwise the stack is used. STACKBUFF_END will
free dynamically allocated buffers.


messages
--------

utils.h defines following logging functions: log_message(), err_message() and
dbg_message().


time
----

Use z_sleep() to Sleep for a number of milliseconds; use z_clock() to
get current time in miliseconds.


zeromq
------

To allow for both 2 and 3 series of ZeroMQ to work with our code use
definitions in this file.


platform-specific
-----------------

Most of the platform-specific parts are in dedicated files (only utils_win.h and
utils_posix.h for now).


Other small ones
----------------

- *error codes* Generic error codes are implemented as func_error_t type.
- *offset_t* offset in a buffer
- *min, max* following are defined: aitown_min and aitown_max
- *random float* with randof()











