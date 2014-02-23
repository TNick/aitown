aitown-db                         {#aitown_db}
=========

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-dejavu.html).


aitown-db is a library that provides a common interface for various databases.

The main structure is the manager (aitown_db_mng_t) that needs to be
initialised once when the application starts and terminated at the end. It
contains a list of providers (aitown_db_driver_t) that are referenced by name
(string) in code. Drivers can be added and removed dynamically and the
implementation contains drivers for KyotoCabinet, TokyoCabinet and MySQL. If
corresponding libraries are found at cmake time them these drivers are build
and loaded when the manager gets initialised.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
aitown_db_mng_t db_mng;
aitown_db_mng_init (&db_mng);
// ...
aitown_db_mng_end (&db_mng);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a connection to a database (aitown_db_t) is requested the driver
is looked up by name and the database instance is created by it. Options to fine-tune
the database are provided through aitown_db_open_t (like fixed size keys and
values) but the driver is not required to implement them (it may use generic
implementation).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// will receive a pointer to our database
aitown_db_t *db_handle;

// open querry
aitown_db_open_t open_data;

// set the manager
open_data.db_mng = &dbmng;

// if the database is to be created, it will have
// variable-lenght keys and values
open_data.flags = AITOWN_DB_KEY_VARIABLE | AITOWN_DB_VALUE_VARIABLE;

// thus there's no need to set following parameters to any special value
open_data.key_len = 0;
open_data.value_len = 0;

// use build-in kyoto driver
open_data.driver = "buildin-kyoto";

// name of this database (meaning is driver-dependent)
open_data.db_name = "test1";

// path for the database (meaning is driver-dependent)
open_data.db_path_hint = "/tmp";

// a configuration file (ini style) to pass to the driver
// the manager opens the file, creates a tree, sets this to NULL
// and resulted tree in cfg
open_data.cfg_file = cfg_file;

// a preloaded ini tree (would be ignored in this case) to pass to the driver
// some functions may not use this member - check docs
open_data.cfg = NULL;

// output handle
open_data.out = &db_handle;

// now go ahead and open it
func_error_t ret_open = aitown_db_open (&open_data);

// ...

// then close it when you're done
aitown_db_close (&dbmng, &db_handle);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Writing and reading are done by referencing the manager and using counted
buffers of bytes. No type is assumed about the key or the value in the general
form of the functions. If the user provides a callback then the call may be
asynchronous, depending on the driver. If the user does not provide a callback
the functions will block until a response is received or a time-out occurs,
even if underlying driver performs an asynchronous operation.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// have some data at hand
char test_1_key[] = "test key 1";
unsigned test_1_keysz = strlen (test_1_key);
char test_1_value[] = "test value 1";
unsigned test_1_value_sz = strlen (test_1_value);

// control structure
aitown_db_io_t write_data;

// set the manager and the database
write_data.db_mng = &dbmng;
write_data.db = db_handle;

// set data (key and value)
write_data.key.ptr = (const void*)test_1_key;
write_data.key_sz = test_1_keysz;
write_data.val.p = test_1_value;
write_data.val_sz.d = test_1_value_sz;

// we could use a callback here (NULL means wait for result, then return)
write_data.kb.kb_write = NULL;

// payload to be passed to the callback
write_data.user = NULL;

// go ahead and write it
func_error_t ret_write = aitown_db_cwrite (&write_data);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// have some data at hand
char test_1_key[] = "test key 1";
unsigned test_1_keysz = strlen (test_1_key);

// getting the result back in these variables
const void * databack = NULL;
unsigned databack_size = 0;

// control structure
aitown_db_io_t read_data;

// set the manager and the database
read_data.db_mng = &dbmng;
read_data.db = db_handle;

// set data (key and where to receive the result)
read_data.key.ptr = (const void*)test_1_key;
read_data.key_sz = test_1_keysz;
read_data.val.pp = &databack;
read_data.val_sz.pd = &databack_size;

// we could use a callback here (NULL means wait for result, then return)
read_data.kb.kb_read = NULL;
// payload to be passed to the callback
read_data.user = &open_data;

// go ahead and perform this request
func_error_t ret_read = aitown_db_cread (&read_data);

// ...

// then free the chunk
aitown_db_free (&dbmng, db_handle, &databack);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


The driver (aitown_db_driver_t) is basically a structure that holds pointers to
functions that perform required tasks as asked by the manager. The user only
interacts with the manager and it selects appropriate method from driver table.
Take a look inside <em>buildin</em> directory for example of drivers:

- aitown_db_driver_kyoto_t
- aitown_db_driver_tokyo_t
- aitown_db_driver_mysql_t
