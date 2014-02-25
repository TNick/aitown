aitown
======

a town for artificial intelligence

This package consists of several libraries and applications related
to Artificial Intelligence. For a high level overview and design
please check out the site at
[tnick.github.io/aitown](http://tnick.github.io/aitown/).


Dependencies
============


ZeroMQ
------

Hosted at [zeromq.org](http://zeromq.org/); looks like an embeddable
networking library but acts like a concurrency framework.


ArgTable
--------

Hosted at [argtable.sourceforge.net](http://argtable.sourceforge.net/)


ProtobufC
---------

Hosted at [github.com/protobuf-c/protobuf-c](https://github.com/protobuf-c/protobuf-c),
is a C implementation of Google Protocol Buffers.


Blas
----

[OpenBlas](http://www.openblas.net/) is recomended.


OpenMP
------

The OpenMP® API specification for parallel programming
are hosted at [openmp.org](http://openmp.org/wp/openmp-specifications/).


Databases
---------

Code to make use of following database providers is implemented:
[KyotoCabinet](http://fallabs.com/kyotocabinet/),
TokyoCabinet, MySQL. At cmake time an attempt is
made to find them.


googletest
----------

Code for unit testing uses [googletest](https://code.google.com/p/googletest/).



Components
==========

Embedded libraries
------------------

The code in this group originates in other open source projects.
The source files were sometimes modified and files were added in some places.
As a rule of thumb the libraries who'se name don't start with `aitown-`
are considered "semi-external" and no dependency on other parts of
aitown project should be induced. If an dependency is unavoidable then the
directory should be renamed. For small changes a wrapper in
[aitown-utils](http://tnick.github.io/aitown/reference/aitown-utils.html)
can be created.

- [sglib](sglib.html)
- [inih](inih.html)
- [cfgpath](cfgpath.html)
- [kdtree](kdtree.html)


Modules
-------

- [aitown-core](http://tnick.github.io/aitown/reference/aitown-core.html) - [implementation](aitown_core.html)
- [aitown-utils](http://tnick.github.io/aitown/reference/aitown-utils.html) - [implementation](aitown_utils.html)
- [aitown-plugin](http://tnick.github.io/aitown/reference/aitown-plugin.html) - [implementation](aitown_plugin.html)
- [aitown-db](http://tnick.github.io/aitown/reference/aitown-db.html) - [implementation](aitown_db.html)
- [aitown-dstorage](http://tnick.github.io/aitown/reference/aitown-dstorage.html) - [implementation](aitown_dstorage.html)
- [aitown-image](http://tnick.github.io/aitown/reference/aitown-image.html) - [implementation](aitown_image.html)
- [aitown-compress](http://tnick.github.io/aitown/reference/aitown-compress.html) - [implementation](aitown_compress.html)
- [aitown-dejavu](http://tnick.github.io/aitown/reference/aitown-dejavu.html) - [implementation](aitown_dejavu.html)


Applications
------------

- [aitown-index](http://tnick.github.io/aitown/reference/aitown-index.html) - [implementation](aitown_index.html)
- [aitown-client](http://tnick.github.io/aitown/reference/aitown-client.html) - [implementation](aitown_client.html)
- [aitown-server](http://tnick.github.io/aitown/reference/aitown-server.html) - [implementation](aitown_server.html)

