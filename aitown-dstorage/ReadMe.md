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


Main Structure
--------------

To make use of aitown-dstorage first create a aitown_dstorage_t.
Initialisation function needs a source of settings - either
from a file or a config section. When no longer needed
it should be discarded:

    aitown_dstorage_t dstorage;
    aitown_dstorage_finit (&dstorage, settings_file);
    // ...
    aitown_dstorage_end (&dstorage);

The structure hosts an aitown-db structure inside that also gets
initialised using same source. All interactions with storage
devices are mediated through it.


Controllers
-----------

Controllers are ways of storing and retreiving data. There is exactly
one database associated with each controller, so the
aitown_dstorage_controller_t acts as a wrapper for aitown_db_t.

The list of controllers is stored under [dstorage/controllers] section
as an array:

    [dstorage/controllers]
    ; generic data for all controllers (undefined fight now)

    [dstorage/controllers/0]
      database = "db1"
      performance = 100

    [dstorage/controllers/1]
      database = "db2"
      status = TEMP_ERR
      performance = 30

Controllers are identified by their index. It is vital that
the index of a controler, once assigned, does not change. This
is because the index of a controller is used inside the ID database to
tell how to load the content for that ID.

Controllers that are no longer present on a system or have
become obsolete should declare their status as either
TEMP_ERR or PERM_ERR (OK is assumed by default). Thei should,
however, keep their place in the list.

Controllers are indexed by their performance, so that new
IDs are always allocated inside bets performing controller. Once
it becomes full next controller in the list is used and so on.
A future improvements to the code should consider moving
frequently used IDs in best performing controller and other
IDs to slower controllers.

Controllers can't be created at run-time. Future iterations that will
allow this should also create an entry in the appropriate
configuration setting.

aitown_dstorage_ctrl_mng_t is the management structure inside
aitown_dstorage_t that tracks controllers. Is initialised
using aitown_dstorage_ctrl_mng_init(), that also loads the list
of controllers from provided config section, and terminated with
aitown_dstorage_ctrl_mng_end().


Handles
-------

The handles are represented by the aitown_dstorage_handle_t structure.
Any ID is represented in memory by a handle that stores numeric value
for that ID and other characteristics, like the controller that manages it.

This mechanism of loading ID content in two steps, first the meta-data
and then actual data is intended to limit the ammount of memory used
(when an ID references another one, it may not necesary require its content).

Handles may have several statuses described by aitown_dstorage_sts_t.
It indicates if the content is loaded or not, if is lost and if it has
changed and should be written back when discarded.

Handles are managed by aitown_dstorage_h_mng_t. Insidem handles are organised
in a Red-Black Tree indexed by the id to allow for fater look-up. We use
aitown_dstorage_handle() to find a handle or to instantiate a new one based
on an ID (aitown_dstorage_id_t). It will look up the tre first, then it will
look into the database and load it if found.

The config file needs to indicate the database that should be used. A
coresponding entry in [databases] section should exist.

    [dstorage/handlers]
      database = "database_for_ids"
      next_id = 1
      handle_max = 1024
      free_max = 1024
      cache_max = 512

`handle_max` tells the total number of handles that are allowed to exist,
including freed and cached ones.Reaching this limit will
cause new requests to fail.

When a handle's reference count reaches 0 it is not removed imidiatelly.
Instead, a number of these (`cache_max`) are kept in the tree, in the hope that
a future request will need them.

When it is decided that it is time to free a handle, it returns to the pool
of free handles. If that pool is full (`free_max`) the memory for that
handle is actually freed.

In situations when there is no handle left in the free pool but there are
cached handles the tree is scanned for those and their memory is
reclaimed as free handles, thus refilling the pool. If there is no
cached handle and `handle_max` allows it, the system is querried for more
memory.


Memory
------

The content associated with an ID is always a counted buffer. It starts
with aitown_dstorage_data_t header structure and the raw data follows
right afterwards; this is the format used both in memory and inside database.

The content may either be retreived from the database or created as new
entries. The new ones are managed by this library while the content retrieved
from the database is managed by it. This status is tracked by the status of
the handle.


Working with handles
--------------------

Along a proper config file, the usage of this module is limited to a
handfull  of functions. Initialization and termination aside, there is a
function to create a new handle and id, with a buffer of given size:

    aitown_dstorage_handle_t * handle =
        aitown_dstorage_new_handle (dstorage_manager, 128);
    if (handle !== NULL) {
        const char * data = (const char *) aitown_dstorage_raw (handle);
        unsigned data_sz = aitown_dstorage_raw_size (handle);
        // ...
        aitown_dstorage_handle_decref (handle, handle);
        handle = NULL;
    }

To get a handle for a given `id` and modify its content use:

    aitown_dstorage_handle_t * handle =
        aitown_dstorage_handle (dstorage_manager, id);
    if (handle !== NULL) {
        const char new_value = "test string"
        aitown_dstorage_set_raw (handle, new_value, strlen (new_value));
        // ...
        aitown_dstorage_handle_decref (handle, handle);
        handle = NULL;
    }

This does not load old content, it just overwrites it. To get a handle
for an id and the data associated with it, use:

    aitown_dstorage_handle_t * handle =
        aitown_dstorage_handle (dstorage_manager, id);
    if (handle !== NULL) {
        func_error_t result =
            aitown_dstorage_resolve_handle (dstorage_manager, handle);
        if (result == FUNC_OK) {
            const char * data = (const char *) aitown_dstorage_raw (handle);
            unsigned data_sz = aitown_dstorage_raw_size (handle);
        }
    }

