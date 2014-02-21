/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         aitown-index/main.c
  \date         September 2013
  \author       TNick
  
  \brief        Entry point for the application
  
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include "index_data.h"
#include <aitown/ini.h>
#include <aitown/cfgpath.h>
#include <aitown/utils.h>
#include <aitown/protobuf_wrapper.h>
#include <aitown/error_codes.h>
#include <aitown/zmq_wrapper.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! name of this program
#define APP_NAME "aitown-index"
#define APP_INI_FILE APP_NAME ".conf"

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

//! for CTRL+C handling
static int keep_running = 1;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

//! interrupt hendler
void intHandler(int dummy) {
	INDEX_UNUSED (dummy);
    keep_running = 0;
}

//! parse command line
static index_error parse_command_line (
    index_data_t* index_data_, int argc, char *argv[] )
{ dbg_message (__func__);
	// initialise the table of arguments
	struct arg_lit  *help    = arg_lit0 (NULL,"help",
	    "print this help and exit");
    struct arg_lit  *version = arg_lit0 (NULL,"version",
        "print version information and exit");
    struct arg_int  *port  = arg_int0 ("p","port",NULL,
        "the port to use (default is 29898)");
    struct arg_int  *max_serv  = arg_int0 ("m","max",NULL,
        "maximum number of servers (default is 65000)");
    struct arg_end  *end     = arg_end (20);
    void* argtable[] = {help,version,port,max_serv,end};
    const char* progname = APP_NAME;

	
	int nerrors;
	index_error exitcode = FUNC_OK;
	for (;;) {
		// verify the argtable[] entries were allocated sucessfully
		if (arg_nullcheck (argtable) != 0) {
			fprintf (stderr, "%s: insufficient memory\n", progname);
			exitcode = FUNC_MEMORY_ERROR;
			break;
		}

		// set any command line default values prior to parsing
		port->ival[0] = -1;
		max_serv->ival[0] = -1;
		
		// Parse the command line as defined by argtable[]
		nerrors = arg_parse (argc,argv,argtable);

		// special case: '--help' takes precedence over error reporting
		if (help->count > 0) {
			printf ("Usage: %s", progname);
			arg_print_syntax (stdout,argtable,"\n");
			printf ("This program runs a server index for aitown\n");
			arg_print_glossary (stdout,argtable,"  %-25s %s\n");
			break;
		}

		// special case: '--version' takes precedence error reporting
		if (version->count > 0) {
		/** @todo replace with proper version once the dynamic header is inplace */
			printf ("'%s' version %s\n", progname, "1.0.0");
			break;
		}

		// If the parser returned any errors then display them and exit
		if (nerrors > 0) {
			arg_print_errors (stderr,end,progname);
			fprintf (stderr, "Try '%s --help' for more information.\n",progname);
			exitcode = FUNC_GENERIC_ERROR;
			break;
		}

		// sanity checking
		if ( ( port->ival[0] <= 0 ) | ( port->ival[0] > 65535 ) ) {
			
			fprintf (stderr, "Port number must be a positive integer.\n");
			exitcode = FUNC_GENERIC_ERROR;
			break;
		}
		if ( max_serv->ival[0] <= 0 ) {
			
			fprintf (stderr, "Maximum number of connections must be a positive integer.\n");
			exitcode = FUNC_GENERIC_ERROR;
			break;
		}

		// copy values to proper place
		if ( port->ival[0] != -1 ) {
			index_data_->port = port->ival[0];
		} else if ( index_data_->port == 0 ) {
			index_data_->port = 29898;
		}
		if ( max_serv->ival[0] != -1 ) {
			index_data_->max_serv = max_serv->ival[0];
		} else if ( index_data_->max_serv == 0 ) {
			index_data_->max_serv = 65000;
		}
		
		break;
	}

	// deallocate each non-null entry in argtable[] and return
    arg_freetable (argtable,sizeof(argtable)/sizeof(argtable[0]));
    return exitcode;
}

//! parse config file line by line
static int handler(void* user_, const char* section_, const char* name_,
				   const char* value_)
{
	index_data_t* index_data = (index_data_t*)user_;
	dbg_message ("Config file %s=%s", section_, name_);
	 
	if ( strcmp (section_, "general") ) {
		if ( strcmp (name_, "port") ) {
			index_data->port = atoi (value_);
		} else if ( strcmp (name_, "maximum_number_of_servers") ) {
			index_data->max_serv = atoi (value_);
		} else {
			return 0;
		}
	} else {
		return 0;
	}
	return 1;
}

//! locate config file and load it
static index_error load_config ( index_data_t* index_data_ )
{ dbg_message (__func__);

	FILE *fp;
	char cfg_file[MAX_PATH*2];
	for (;;) {
		
		// we assume following cases:
		// - if a config file is present in current directory, we use that one
		fp = fopen (APP_INI_FILE, "r");
		if ( fp != NULL ) {
			memcpy (cfg_file, APP_INI_FILE, sizeof(APP_INI_FILE));
			break;
		}
		
		// - if a file is present in config directory, use that
		get_user_config_file (cfg_file, sizeof(cfg_file), APP_NAME);
		if (cfg_file[0] == 0)
			break;
		fp = fopen (cfg_file, "r");
		if ( fp != NULL )
			break;
		
		// - gave up
		break;
	}
	
	// load the file if present
	if ( fp != NULL ) {
		dbg_message ("loading config file %s", cfg_file);
		if ( ini_parse (cfg_file, handler, index_data_) < 0 ) {
			fprintf (stderr, "Can't load configuration file %s\n", cfg_file);
			return FUNC_GENERIC_ERROR;
		}
	} else {
		dbg_message ("no config file to load" );
	}
	
	return FUNC_OK;
}

//! free a buffer after it has been send by ZMQ
static void
zmq_buffer_free (void *data, void *hint) {
	free (data);
	INDEX_UNUSED (hint);
}

//! send a message from server to client
static index_error send_reply (AiTownIndexReply * input_msg, void *server)
{ dbg_message (__func__);
	// serialize in a buffer
	void *buff;
	size_t sz = AiTownIndexReply__pack (input_msg, &buff);
	if ( sz == 0 ) {
		return FUNC_MEMORY_ERROR;
	}
	
	// Create a new message, allocating space for message content
	zmq_msg_t msg;
	int rc = zmq_msg_init_data( &msg, buff, sz, zmq_buffer_free, NULL);
	INDEX_ASSERT (rc == 0);
	
	// Send the message to the socket
	rc = zmq_msg_send (&msg, server, 0); 
	if (rc != (int)sz) {
		err_message( "Failed to send message to client");
	}
	
	return FUNC_OK;
}

//! send an OK reply to the sender
static index_error send_ok (void *server)
{ dbg_message (__func__);

	// prepare message structure
	AiTownIndexReply msg = AI_TOWN_INDEX_REPLY__INIT;
	msg.type = AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_OK;
	msg.version = INDEX_CURRENT_PROTOCOL_VERSION;
	
	// and send it
	return send_reply (&msg, server);
}

//! send an error reply to the sender and log to stdout
static index_error send_error (void *server, const char * message)
{ dbg_message (__func__);
	
	// prepare message structure
	AiTownIndexReply msg = AI_TOWN_INDEX_REPLY__INIT;
	msg.type = AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ERROR;
	msg.version = INDEX_CURRENT_PROTOCOL_VERSION;
	msg.error_message = (char*)message;
	
	// and send it
	err_message(message);
	return send_reply (&msg, server);
}

//! helper for send_list
static int foreach_send_list (index_data_t *index_data_, 
    server_data_t *server_data, void * user_data)
{
	INDEX_UNUSED (index_data_);
	AiTownIndexReply *msg = (AiTownIndexReply*)user_data;
	
	// allocate a structure for this one
	AiTownIndexAdd *add_struct = (AiTownIndexAdd*)malloc (sizeof(AiTownIndexAdd));
	if ( add_struct == NULL )
		return -1;
	msg->list[msg->n_list] = add_struct;
	AiTownIndexAdd__init (add_struct);
	
	// fill in the specifics
	add_struct->name = (char*)server_data->name;
	add_struct->address = (char*)server_data->address;
	add_struct->has_port = 1;
	add_struct->port = server_data->port;	
	
	// step to next
	msg->n_list++;
	return 0;
}

//! send an error reply to the sender and log to stdout
static index_error send_list (index_data_t *index_data_, void *server)
{ dbg_message (__func__);

	index_error err_code;
	int i;
	
	// prepare message structure
	AiTownIndexReply msg = AI_TOWN_INDEX_REPLY__INIT;
	msg.type = AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_LIST;
	msg.version = INDEX_CURRENT_PROTOCOL_VERSION;
	
	// allocate list of pointers
	// msg.n_list = 0; - will use it as index in array
	msg.list = (AiTownIndexAdd **)malloc(index_data_->server_count*sizeof(void*));
	if (msg.list == NULL)
		return FUNC_MEMORY_ERROR;
	
	// iterate in all servers
	index_data_foreach_server (index_data_, foreach_send_list, &msg);
	if ( msg.n_list != index_data_->server_count ) {
		err_code = FUNC_MEMORY_ERROR;
	} else {
		// and send it
		err_code = send_reply (&msg, server);
	}
	
	// free dynamic things
	for ( i = 0; i < msg.n_list; i++ ) {
		free (msg.list[i]);
	}
	free (msg.list);
	
	// and that should do it
	return err_code;
}

//! process a request message
static index_error process_request ( index_data_t *index_data_, 
    void *server, void *data, size_t data_sz)
{ dbg_message (__func__);

	index_error errcode = FUNC_OK;
	
	// decode the message
	AiTownIndex * incoming = AiTownIndex__unpack (data_sz, data);
	if ( incoming == NULL ) {
		send_error (server, "Error decoding incoming message");
		return FUNC_OK;
	}
	
	// check the version - the only one that we know of is 1
	if ( incoming->version != 1 ) {
		send_error (server, "Unsupported version");
		AiTownIndex__free_unpacked (incoming);
		return FUNC_OK;
	}
	
	// type based processing
	switch ( incoming->type ) {
	case   AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ADD: {
	
		// check the sanity of incoming data
		if ( ( incoming->add == NULL ) || 
		      ( incoming->add->name == NULL ) ||
		      ( incoming->add->address == NULL ) ||
		      ( !incoming->add->has_port ) ||
		      ( incoming->add->port <= 0 ) ) {
			send_error (server, "Malformed add request");
			break;
		}
		
		// allocate a new server structure for this
		server_data_t * sd;
		server_data_t * sd_other;
		errcode = server_data_new (&sd, incoming->add->name, 
		    incoming->add->address, incoming->add->port);
		if ( errcode != FUNC_OK ) {
			send_error (server, "Internal error");
			break;
		}
		
		// see if a server with this name already exists and remove it if so
		sd_other = index_data_get_server (index_data_, sd->name);
		if ( sd_other != NULL ) {
			index_data_rem_server (index_data_, sd_other );
			server_data_delete (&sd_other);
		}
		index_data_add_server (index_data_, sd);
		log_message ("Added server %s", sd->name);
		send_ok (server);
		break; }
		
	case   AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_REM: {
	
		// check the sanity of incoming data
		if ( ( incoming->rem == NULL ) ||
		      ( incoming->rem->name == NULL ) ) {
			send_error (server, "Malformed remove request");
			break;
		}
		
		// find the server in question
		const char * name = incoming->rem->name;
		server_data_t * sd;
		sd = index_data_get_server (index_data_, name);
		
		// and remove it
		if ( sd == NULL ) {
			send_error (server, "Attempt to close unexisting server");
			err_message (name);
		} else {
			index_data_rem_server (index_data_, sd );
			server_data_delete (&sd);
			log_message ("Removed server %s", name);
			send_ok (server);
		}
		break; }
	case   AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_LIST: {
		send_list (index_data_, server);
		break; }
	default:
		send_error (server, "Unknown incoming message type");
		err_message( "Requested type was %d", incoming->type );
	}

	// release serialized data
	AiTownIndex__free_unpacked (incoming);
	return errcode;
}

/* ------------------------------------------------------------------------- */
//! application entry point
int            main            ( int argc, char *argv[] )
{
	index_error exitcode;
	index_data_t index_data;
	char buffer[64];
	signal(SIGINT, intHandler);
	for (;;) {
		// let the user know we're running
		log_message (APP_NAME " started.");
	
		// initialize index structure for this program
		exitcode = index_data_init (&index_data);
		if ( exitcode != FUNC_OK )
			break;
	
		// get values from configuration file and command line
		exitcode = load_config (&index_data);
		if ( exitcode != FUNC_OK )
			break;
		exitcode = parse_command_line (&index_data, argc, argv);
		if ( exitcode != FUNC_OK )
			break;

		
	                                                                            dbg_message ("mark 6");
		// prepare zmq
		void *context = zmq_ctx_new ();
		if ( context == NULL )
			break;
	                                                                            dbg_message ("mark 7");
		void *server = zmq_socket (context, ZMQ_REP);
		if ( server == NULL )
			break;
	                                                                            dbg_message ("mark 8");
		sprintf (buffer, "tcp://*:%d", index_data.port);
	                                                                            dbg_message ("mark 81");
		if ( zmq_bind (server, buffer) != 0 ) {
			fprintf (stderr, "Failed to bind to port %d\n", index_data.port);
			break;
		}
		
	                                                                            dbg_message ("mark 9");
		// let the user know we're running
		log_message (APP_NAME " listening to port %d.", index_data.port);
	                                                                            dbg_message ("mark 10");
		
		// wait for requests and process them
		while ( exitcode == FUNC_OK ) {
			int64_t more;
			size_t more_size = sizeof more;
			do {
				//! react to CTRL+C
				if ( !keep_running ) {
					log_message ("Exit on user request.");
					break;
				}
				
				// Create an empty ØMQ message to hold the message part
				zmq_msg_t part;
				int rc = zmq_msg_init (&part);
				INDEX_ASSERT (rc == 0);
				INDEX_UNUSED (rc);
				
				// Block until a message is available to be received from socket
				rc = zmq_msg_recv (&part, server, 0);
				if ( rc == -1 ) {
					rc = zmq_errno();
					if ( rc == 4 ) {
						log_message ("Exit on user request.");
					} else {
						err_message ( "code %d while receiving message: %s", 
						    zmq_errno(), zmq_strerror(rc));
						exitcode = FUNC_GENERIC_ERROR;
					}
					break;
				}
				
				// process this message
				void *data = zmq_msg_data (&part);
				size_t data_sz = zmq_msg_size (&part);
				if ( data_sz > 0 ) {
					INDEX_ASSERT (data != NULL);
					exitcode = process_request (&index_data, server,data, data_sz);
					if ( exitcode != FUNC_OK ) {
						log_message ("Exit on internal error");
						break;
					}
				}
				
				// Determine if more message parts are to follow
				rc = zmq_getsockopt (server, ZMQ_RCVMORE, &more, &more_size);
				INDEX_ASSERT (rc == 0);
				zmq_msg_close (&part);
			} while (more);
			if ( !keep_running )
				break;
		}
		
		// terminate zmq
		zmq_close (server);
		zmq_ctx_destroy (context);
		break;
	}
	log_message (APP_NAME " is being closed.");
	
	// terminate the structure
	index_data_end (&index_data);
	
	return exitcode;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
