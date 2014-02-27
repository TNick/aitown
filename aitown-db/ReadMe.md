aitown-db                         {#aitown_db}
=========

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-dejavu.html).


Opening and Closing Databases
-----------------------------

Parameters regarding a database are kept in a config file. When requesting
to open a database, we tell its name to aitown_db_open() and expect it to be opened
for us with arguments extracted from config file.

    aitown_db_t * db_handle;
    aitown_db_open (manager, "db1", &db_handle);
    // ...
    aitown_db_close (db_handle);

If one or more parameters must be enforced, calling code may do that using
aitown_db_open_ovr() with a aitown_db_open_t structure. Here's an example
call that does the same thing as previous example, except that it
requires the keys to be 8 bytes (64 bits) in lenght.

    aitown_db_t * db_handle;
    aitown_db_open_t ovr;

    aitown_db_preopen (&ovr, manager, "db1", &db_handle);
    ovr.key_len = 8;
    ovr.driver = "buildin-kyoto";
    ovr.path_hint = "/tmp";
    aitown_db_open_ovr (&ovr);
    // ...
    aitown_db_close (db_handle);


Database configuration should be stored in config file under [databases]
top level section, with one subsection for each database. Note that the
manager is subject to config stacking.


Reading and Writing
-------------------

    const char * val = aitown_db_read (db_handle, "key1");
    aitown_db_write (db_handle, "key1", "new value");


Drivers
-------

The driver (aitown_db_driver_t) is basically a structure that holds pointers to
functions that perform required tasks as asked by the manager. The user only
interacts with the manager and it selects appropriate method from driver table.
Take a look inside <em>buildin</em> directory for example of drivers:

- aitown_db_driver_kyoto_t
- aitown_db_driver_tokyo_t
- aitown_db_driver_mysql_t

A plug-in that provides a database driver must run aitown_db_driver_init()
over its instance, then add it using aitown_db_driver_add(). At unload time
the reverse should happen via aitown_db_driver_rem() and aitown_db_driver_end().

The driver needs not and should not use its own mechanism for settings. Instead,
it should querry config tree within the manager, using [drivers] top level
section and a subsection with same name as the driver.

    [drivers]
    ; generic configuration for all drivers

    [drivers/mydriver]
    ; configuration for driver "mydriver"


Config file is saved when aitown_db_mng_end is called, so drivers can
use it for persistent settings storage.
