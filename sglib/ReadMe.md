sglib                        {#sglib}
=====


[Sglib](http://sglib.sourceforge.net/) version 1.0.4 is a
simple generic library for C hosted at
[sglib.sourceforge.net](http://sglib.sourceforge.net/).


Sglib is a library defining useful macros for manipulating common data
structures. The library currently provides generic implementation for:

- sorting arrays
- manipulating linked lists
- manipulating sorted linked lists
- manipulating double linked lists
- manipulating red-black trees
- manipulating hashed containers

A basic set of functions (macros) is provided for each data structure. They
cover insertion, deletion, search and iterator traversal of elements. Moreover,
additional (specific) functions are provided for each data structure, such as,
concatenation, reverse or sort for lists.

Sglib provides algorithms, not data structures. Algorithms implemented in Sglib
operate on user's types, for example, list macros are operational for any data
structure containing a pointer to the same type.

Sglib consists of a single header file without any binary code. In order to use
it just put `#include <aitown/sglib.h>` into your source code.

The library is implemented in the C programming language and written for C
programmers, however it is freely inspired by the Standard Template Library.


The distribution contains:

- sglib.h - this is the generic library, the file can be directly used,
or you can move it to your include directory.

- doc - directory containing the documentation. The main page of the
documentation is stored in the doc/index.html.

- samples - directory storing simple executable files using sglib.h.
You can run all  the samples together with self-test routine
by running make in this directory.

