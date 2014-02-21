/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         aitown-client/main.c
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

#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <aitown/ini.h>
#include <aitown/cfgpath.h>
#include <aitown/utils.h>
#include <aitown/protobuf_wrapper.h>
#include <aitown/utils_unused.h>
#include <aitown/dbg_assert.h>
#include <aitown/error_codes.h>
#include <aitown/zmq_wrapper.h>

#include "globals.h"
#include "aiclient_data.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/// name of this program
#define APP_NAME "aitown-client"
#define APP_INI_FILE APP_NAME ".ini"

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
    aiclient_data_t* client_data_, int argc, char *argv[] )
{
	// initialise the table of arguments
	struct arg_lit  *help    = arg_lit0 (NULL,"help",
	    "print this help and exit");
    struct arg_lit  *version = arg_lit0 (NULL,"version",
        "print version information and exit");
    struct arg_end  *end     = arg_end (20);

    void* argtable[] = {help,version,end};
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
		
		// Parse the command line as defined by argtable[]
		nerrors = arg_parse (argc,argv,argtable);
		
		// special case: '--help' takes precedence over error reporting
		if (help->count > 0) {
			printf ("Usage: %s", progname);
			arg_print_syntax (stdout,argtable,"\n");
			printf ("This program runs a client for aitown\n");
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
	aiclient_data_t* client_data = (aiclient_data_t*)user_;
    VAR_UNUSED(client_data);
	if ( strcmp (section_, "general") ) {
        /** @todo */
	} else {
		return 0;
	}
	return 1;
}

//! locate config file and load it
static func_error_t load_config ( aiclient_data_t* client_data_ )
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
		if ( ini_parse (cfg_file, handler, client_data_) < 0 ) {
			fprintf (stderr, "Can't load configuration file %s\n", cfg_file);
			return FUNC_GENERIC_ERROR;
		}
	} else {
		dbg_message ("no config file to load" );
	}
	
	return FUNC_OK;
}



/* ------------------------------------------------------------------------- */
//! application entry point
int			main			( int argc, char *argv[] )
{
	// we will catch CTRL+C
	signal(SIGINT, intHandler);
	
	aiclient_data_t client_data;
	func_error_t exitcode;
	for (;;) {
	
		// initialize aiclient structure for this program
		exitcode = aiclient_data_init (&client_data);
		if ( exitcode != FUNC_OK )
			break;

		// get values from configuration file and command line
		exitcode = load_config (&client_data);
		if ( exitcode != FUNC_OK )
			break;
		exitcode = parse_command_line (&client_data, argc, argv);
		if ( exitcode != FUNC_OK )
			break;
			
//		// set a default name
//		if ( client_data.name == NULL ) {
//			size_t sz = strlen(client_data.address) + 40;
//			client_data.name = (const char*)malloc(sz);
//			sprintf( (char*)client_data.name, "%s:%d", 
//			    client_data.address, client_data.port );
//		}
//		log_message ("Running on address %s:%d under name %s", 
//		    client_data.address,
//		    client_data.port,
//		    client_data.name );
		
		// prepare zmq
		/** @todo */
		

		
		// terminate zmq
		/** @todo */
		break;
	}
	log_message (APP_NAME " is being closed.");
	
	// terminate the structure
	aiclient_data_end (&client_data);

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
