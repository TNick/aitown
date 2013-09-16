/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			main.c
  \date			September 2013
  \author		TNick
  
  \brief		Entry point for the application
  
  
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

#include <zmq.h>
#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include "aiserver_data.h"
#include <aitown/ini.h>
#include <aitown/cfgpath.h>
#include <aitown/utils.h>
#include <aitown/protobuf_wrapper.h>
#include <aitown/error_codes.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! name of this program
#define APP_NAME "aitown-server"
#define APP_INI_FILE APP_NAME ".ini"

//! number of times to retry to connect to index server
#define AISERVER_SERVER_RETRIES 3
//! timeout for connecting to index server
#define AISERVER_SERVER_TIMEOUT 3000

#ifndef ZMQ_DONTWAIT
#   define ZMQ_DONTWAIT   ZMQ_NOBLOCK
#endif
#ifndef ZMQ_RCVHWM
#   define ZMQ_RCVHWM     ZMQ_HWM
#endif
#ifndef ZMQ_SNDHWM
#   define ZMQ_SNDHWM     ZMQ_HWM
#endif
#if ZMQ_VERSION_MAJOR == 2
#   define more_t int64_t
#   define zmq_ctx_destroy(context) zmq_term(context)
#   define zmq_msg_send(msg,sock,opt) zmq_send (sock, msg, opt)
#   define zmq_msg_recv(msg,sock,opt) zmq_recv (sock, msg, opt)
#   define ZMQ_POLL_MSEC    1000        //  zmq_poll is usec
#elif ZMQ_VERSION_MAJOR == 3
#   define more_t int
#   define ZMQ_POLL_MSEC    1           //  zmq_poll is msec
#endif

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
	VAR_UNUSED (dummy);
    keep_running = 0;
}

//! parse command line
static func_error_t parse_command_line (
    aiserver_data_t* server_data_, int argc, char *argv[] )
{
	// initialise the table of arguments
	struct arg_lit  *help    = arg_lit0 (NULL,"help",
	    "print this help and exit");
    struct arg_lit  *version = arg_lit0 (NULL,"version",
        "print version information and exit");
    struct arg_int  *index_port  = arg_int0 (NULL,"index_port",NULL,
        "the port where we announce our existance (default is 29898)");
    struct arg_str  *index_addr  = arg_str0 (NULL,"index_addr",NULL,
        "the address where we announce our existance (default is localhost)");
    struct arg_int  *port  = arg_int0 ("p","port",NULL,
        "the port to use (default is 29897)");
    struct arg_str  *addr  = arg_str0 ("a","addr",NULL,
        "our address (default is localhost)");
    struct arg_str  *name  = arg_str0 ("n","name",NULL,
        "our name");
    struct arg_end  *end     = arg_end (20);
    void* argtable[] = {help,version,index_port,index_addr,port,addr,name, end};
    const char* progname = APP_NAME;
    
	int nerrors;
	func_error_t exitcode = FUNC_OK;
	for (;;) {
		// verify the argtable[] entries were allocated sucessfully
		if (arg_nullcheck (argtable) != 0) {
			fprintf (stderr, "%s: insufficient memory\n", progname);
			exitcode = FUNC_MEMORY_ERROR;
			break;
		}
		
		// set any command line default values prior to parsing
		index_port->ival[0] = -1;
		port->ival[0] = -1;
		
		// Parse the command line as defined by argtable[]
		nerrors = arg_parse (argc,argv,argtable);
		
		// special case: '--help' takes precedence over error reporting
		if (help->count > 0) {
			printf ("Usage: %s", progname);
			arg_print_syntax (stdout,argtable,"\n");
			printf ("This program runs a server aiserver for aitown\n");
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
		if ( ( index_port->ival[0] <= 0 ) | ( index_port->ival[0] > 65535 ) ) {
			fprintf (stderr, "Port number must be a positive integer.\n");
			exitcode = FUNC_GENERIC_ERROR;
			break;
		}
		if ( index_addr->sval[0] == NULL ) {
			if ( index_addr->sval[0]  != NULL )
				free ((void*)index_addr->sval[0]);
			index_addr->sval[0] = strdup ("localhost");
		}
		if ( addr->sval[0] == NULL ) {
			if ( addr->sval[0]  != NULL )
				free ((void*)addr->sval[0]);
			addr->sval[0] = strdup ("localhost");
		}
		
#define ARG_STR_DEFAULT(serv_member,arg_member,default_val)		\
		if ( server_data_->serv_member != NULL ) \
			free ((void*)server_data_->serv_member); \
		if ( arg_member->sval[0][0] == 0 ) \
			server_data_->serv_member = strdup (default_val); \
		else \
			server_data_->serv_member = strdup (arg_member->sval[0]);
#define ARG_STR(serv_member,arg_member)		\
		if ( server_data_->serv_member != NULL ) \
			free ((void*)server_data_->serv_member); \
		if ( arg_member->sval[0][0] != 0 ) \
			server_data_->serv_member = strdup (arg_member->sval[0]);
		
		// copy values to proper place
		if ( index_port->ival[0] != -1 ) {
			server_data_->index_server_port = index_port->ival[0];
		} else if ( server_data_->index_server_port == 0 ) {
			server_data_->index_server_port = 29898;
		}
		if ( port->ival[0] != -1 ) {
			server_data_->port = port->ival[0];
		} else if ( server_data_->port == 0 ) {
			server_data_->port = 29898;
		}
		ARG_STR_DEFAULT(index_server_address, index_addr,"localhost");
		ARG_STR_DEFAULT(address, addr,"localhost");
		ARG_STR(name, name);
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
	aiserver_data_t* server_data = (aiserver_data_t*)user_;
	
	if ( strcmp (section_, "general") ) {
		if ( strcmp (name_, "port") ) {
			server_data->port = atoi (value_);
		} else if ( strcmp (name_, "address") ) {
			if ( server_data->address != NULL )
				free ((void*)server_data->address);
			server_data->address = strdup (value_);
		} else if ( strcmp (name_, "name") ) {
			if ( server_data->name != NULL )
				free ((void*)server_data->name);
			server_data->name = strdup (value_);
		} else {
			return 0;
		}
	} else if ( strcmp (section_, "index") ) {
		if ( strcmp (name_, "port") ) {
			server_data->index_server_port = atoi (value_);
		} else if ( strcmp (name_, "address") ) {
			if ( server_data->index_server_address != NULL )
				free ((void*)server_data->index_server_address);
			server_data->index_server_address = strdup (value_);
		} else {
			return 0;
		}
	} else {
		return 0;
	}
	return 1;
}

//! locate config file and load it
static func_error_t load_config ( aiserver_data_t* server_data_ )
{

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
		if ( ini_parse (cfg_file, handler, server_data_) < 0 ) {
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
	VAR_UNUSED (hint);
}

//! inform the index server about our existance (if one is defined)
static func_error_t inform_index_server (aiserver_data_t *server_data_, AiTownIndex*input_msg)
{
	int linger = 0;
	char *addr_buf = NULL;
	void *client = NULL;
	int rc;
	void *content_buff;
	zmq_msg_t msg_send;
	func_error_t err_code = FUNC_GENERIC_ERROR;
	
	// if not required, exit
	if ( (server_data_->index_server_address == NULL) || 
	      (server_data_->index_server_port <= 0)) {
		return err_code;
	}
	
	for (;;) {
		// Create a new message, allocating space for message content
		zmq_msg_init (&msg_send);
		AISERVER_ASSERT (rc == 0);
	
		// serialize in a buffer
		size_t sz = AiTownIndex__pack (input_msg, &content_buff);
		if ( sz == 0 ) {
			break;
		}
		rc = zmq_msg_init_data (&msg_send, content_buff, sz, NULL, NULL);
		AISERVER_ASSERT (rc == 0);
		
		
		// allocate a buffer for full address; tcp://ADDRESS:PORT -> 6+len(a)+36+1
		int addr_buf_sz = 6+strlen(server_data_->index_server_address)+36+1;
		addr_buf = (char*)malloc(addr_buf_sz);
		if ( addr_buf == NULL )
			break;
		sprintf (addr_buf, "tcp://%s:%d", 
				 server_data_->index_server_address, 
				 server_data_->index_server_port);
		
		// create a socket and attempt a connect
		client = zmq_socket (server_data_->context, ZMQ_REQ);
		if ( client == NULL )
			break;
		zmq_setsockopt (client, ZMQ_LINGER, &linger, sizeof (linger));
		rc = zmq_connect (client, addr_buf);
		if ( rc != 0 ) {
			err_message( "Failed to connect: %s", 
						 zmq_strerror(zmq_errno()));
			break;
		}
		
		int retries_left = AISERVER_SERVER_RETRIES;		
		while (retries_left && keep_running) {
		
			// We send a request, then we work to get a reply
			rc = zmq_msg_send (&msg_send, client, 0); 
			if (rc != (int)sz) {
				err_message( "Failed to send message: %s", 
				    zmq_strerror(zmq_errno()));
				break;
			}
			
			for (;;) {
			
				// Poll socket for a reply, with timeout
				zmq_pollitem_t items [] = { { client, 0, ZMQ_POLLIN, 0 } };
				rc = zmq_poll (items, 1, AISERVER_SERVER_TIMEOUT * ZMQ_POLL_MSEC);
				if (rc == -1) {
					retries_left = 0;
					break; // Interrupted
				}
				
				if (items [0].revents & ZMQ_POLLIN) {
					// get the reply
					zmq_msg_t msg_recv;
					rc = zmq_msg_init (&msg_recv);
					AISERVER_ASSERT (rc == 0);
					rc = zmq_msg_recv (&msg_recv, client, 0);
					if ( rc < 0 ) {
						rc = zmq_errno();
						if ( rc == 4 ) {
							log_message ("Exit on user request.");
							keep_running = 0;
						} else {
							err_message ( "code %d while receiving message: %s", 
										  zmq_errno(), zmq_strerror(rc));
						}
						retries_left = 0;
						zmq_msg_close (&msg_recv);
						break;
					}
					
					// We got a reply from the server
					void *data = zmq_msg_data (&msg_recv);
					size_t data_sz = zmq_msg_size (&msg_recv);
					if ( data_sz == 0 ) {
						err_message ("Empty response");
						retries_left = 0;
						zmq_msg_close (&msg_recv);
						break;
					}
					AISERVER_ASSERT (data != NULL);
					
					// decode the message
					AiTownIndexReply * incoming = AiTownIndexReply__unpack (data_sz, data);
					if ( incoming == NULL ) {
						err_message( "Error decoding incoming message from index server");
						zmq_msg_close (&msg_recv);
						retries_left = 0;
						break;
					}
					
					// check the version - the only one that we know of is 1
					if ( incoming->version != 1 ) {
						err_message ("Unsupported protocol version: %d", incoming->version);
						AiTownIndexReply__free_unpacked (incoming);
						zmq_msg_close (&msg_recv);
						retries_left = 0;
						break;
					}
					
					// type based processing
					switch ( incoming->type ) {
					case   AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_OK: {
						log_message ("Index server ack");
						err_code = FUNC_OK;
						break; }
					case   AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ERROR: {
						err_message ("Index server replied: %s", incoming->error_message);
						break; }
					default:
						err_message( "Unknown reply message from index server: %d", incoming->type );
					}
					
					retries_left = 0;
					zmq_msg_close (&msg_recv);
					break;
				
				// timeout expired and we had no reply
				} else {
					
					// should we give it another chance
					if (--retries_left == 0) {
						err_message( "index server seems to be offline, aborting" );
						break;
					}
					
					// Old socket is confused; close it and open a new one
					err_message( "no response from index server, retrying (%d) ...", retries_left );
					zmq_close(client);
					
					// Send request again, on new socket
					log_message ("reconnecting to index server…");
					client = zmq_socket (server_data_->context, ZMQ_REQ);
					if ( client == NULL ) {
						err_message ("Unable to connect: %s", zmq_strerror(zmq_errno()));
						retries_left = 0;
						break;
					}
					zmq_setsockopt (client, ZMQ_LINGER, &linger, sizeof (linger));
					rc = zmq_connect (client, addr_buf);
					zmq_msg_close (&msg_send); /** @todo is this required? link */
					zmq_msg_init (&msg_send);
					if ( rc != 0 ) {
						err_message ("Unable to connect: %s", zmq_strerror(zmq_errno()));
						retries_left = 0;
						break;
					}
					
					// We send a request, then we work to get a reply
					rc = zmq_msg_init_data (&msg_send, content_buff, sz, NULL, NULL);
					AISERVER_ASSERT (rc == 0);
					break;
				}
			} // for(;;)
		} // while (retries_left && !zctx_interrupted)
		break;
	} // for (;;)
	
	// free things
	zmq_msg_close (&msg_send);
	if ( addr_buf != NULL )
		free (addr_buf);
	if ( content_buff != NULL )
		free (content_buff);
	if ( client != NULL )
		zmq_close(client);
	return err_code;
}

//! inform the index server about our existance (if one is defined)
static void inform_index_server_start (aiserver_data_t *server_data_)
{
	// if not required, exit
	if ( (server_data_->index_server_address == NULL) || 
	      (server_data_->index_server_port <= 0)) {
		return;
	}
	
	AiTownIndex input_msg = AI_TOWN_INDEX__INIT;
	input_msg.type = AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ADD;
	input_msg.version = AISERVER_CURRENT_PROTOCOL_VERSION;
	AiTownIndexAdd add_data = AI_TOWN_INDEX_ADD__INIT;
	input_msg.add = &add_data;
	add_data.name = (char*)server_data_->name;
	add_data.address = (char*)server_data_->address;
	add_data.has_port = 1;
	add_data.port = server_data_->port;
	if ( inform_index_server(server_data_, &input_msg) == FUNC_OK )
		server_data_->was_registered = 1;
	
}

//! inform the index server that we're being closed (if one is defined)
static void inform_index_server_end (aiserver_data_t *server_data_)
{
	// if not required, exit
	if ( (server_data_->index_server_address == NULL) || 
	      (server_data_->index_server_port <= 0) || 
	      (server_data_->was_registered == 0) ) {
		return;
	}
	
	AiTownIndex input_msg = AI_TOWN_INDEX__INIT;
	input_msg.type = AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_REM;
	input_msg.version = AISERVER_CURRENT_PROTOCOL_VERSION;
	AiTownIndexRem rem_data = AI_TOWN_INDEX_REM__INIT;
	input_msg.rem = &rem_data;
	rem_data.name = (char*)server_data_->name;
	inform_index_server(server_data_, &input_msg);
}

/* ------------------------------------------------------------------------- */
//! application entry point
int			main			( int argc, char *argv[] )
{
	// we will catch CTRL+C
	signal(SIGINT, intHandler);
	
	aiserver_data_t server_data;
	func_error_t exitcode;
	for (;;) {
	
		// initialize aiserver structure for this program
		exitcode = aiserver_data_init (&server_data);
		if ( exitcode != FUNC_OK )
			break;

		// get values from configuration file and command line
		exitcode = load_config (&server_data);
		if ( exitcode != FUNC_OK )
			break;
		exitcode = parse_command_line (&server_data, argc, argv);
		if ( exitcode != FUNC_OK )
			break;
			
		// set a default name
		if ( server_data.name == NULL ) {
			size_t sz = strlen(server_data.address) + 40;
			server_data.name = (const char*)malloc(sz);
			sprintf( (char*)server_data.name, "%s:%d", 
			    server_data.address, server_data.port );
		}
		log_message ("Running on address %s:%d under name %s", 
		    server_data.address,
		    server_data.port,
		    server_data.name );

		// prepare zmq
		/** @todo */
		
		inform_index_server_start (&server_data);
		
		// sleep (4);
		
		inform_index_server_end (&server_data);
		
		// terminate zmq
		/** @todo */
		break;
	}
	log_message (APP_NAME " is being closed.");
	
	// terminate the structure
	aiserver_data_end (&server_data);

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
