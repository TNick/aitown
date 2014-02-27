aitown-dstorage                         {#aitown_dstorage}
===============

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-dstorage.html).

This module uses aitown-db module for storage management. It
builds on top of it to be able to provide unique IDs and storage
capabilities associated with each ID.

The module manages a central database that uses IDs as keys and
the way data can be retrieved as values. The value includes
a controller index and controller specific data.


Controllers
-----------

Controllers are ways of storing and retreiving data. There is exactly
one database associated with each controller, so the
aitown_dstorage_ctrl_t acts as a wrapper for aitown_db_t.

The list of controllers is stored under [dstorage/controllers] section
as an array:

    [dstorage/controllers]
    ; generic data for all controllers (undefined fight now)

    [dstorage/controllers/0]
      database = "db1"

    [dstorage/controllers/1]
      database = "db2"

