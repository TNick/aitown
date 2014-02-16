/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_manager_inf.c
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#define PLUGIN_MANAGER_INF_C
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "plugin_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aitown/sglib.h>
#include <aitown/dir_iterator.h>
#include <aitown/dir_utils.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_version.h>
#include <aitown/ini.h>
#include <aitown/utils.h>
#include <aitown/stack_buff.h>
#include <aitown/utils_unused.h>

#ifdef AITOWN_WIN32
#	include "plugin_win.h"
#else
#	include "plugin_posix.h"
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef PM_INF_DEBUG
#define inf_dbg_message dbg_message
#else
static inline void
inf_dbg_message (const char *format, ...) {
	do { (void)(format); } while (0);
}
#endif


#   define SET_SIGN_STRING(loc) \
	in_buff = accumulator_add_string(  \
	    &data->plugin_manager->acm_sign, value_, 0 );\
	if ( in_buff == ACCUMULATOR_BAD_OFFSET ) {\
	    inf_dbg_message ("Failed to allocate memory");\
		ret_val = 0;\
	} else {\
		the_sign = (plugin_sign_t*)accumulator_to_ptr(\
		    &data->plugin_manager->acm_sign, data->sign_data);\
		the_sign->loc = in_buff;\
	}

#   define SET_SIGN_VERSION(loc) \
	if ( sscanf (value_,"%d.%d.%d", \
          &ver_ints[0], &ver_ints[1], &ver_ints[2]) != 3 ) {\
	    ret_val = 0;\
	    inf_dbg_message ("Bad version format in %s=%s", name_, value_);\
    } else {\
		the_sign = (plugin_sign_t*)accumulator_to_ptr(\
		    &data->plugin_manager->acm_sign, data->sign_data);\
		the_sign->loc = version_from_int (ver_ints[0], ver_ints[1], ver_ints[2]);\
	}

#   define SET_SIGN_FLAG(flg) \
	if (is_yes_no(value_, &yes_no ) != FUNC_OK) {\
	    ret_val = 0;\
	    inf_dbg_message ("%s expects yes/no value; provided: =%s", name_, value_);\
    } else {\
		the_sign = (plugin_sign_t*)accumulator_to_ptr(\
		    &data->plugin_manager->acm_sign, data->sign_data);\
		if ( yes_no ) {\
			the_sign->flags = \
					the_sign->flags | flg;\
		} else {\
			the_sign->flags = \
					the_sign->flags & (~flg);\
		}\
    }
    

//! states for dependencies parsing
typedef enum {
	DEPENDPARSE_WAIT_NAME = 0, /**< after a comma, maybe in white space */
	DEPENDPARSE_IN_NAME,
	DEPENDPARSE_WAIT_VER_1,
	DEPENDPARSE_WAIT_VER_2,
	DEPENDPARSE_WAIT_VER_3
} dep_parser_status_t;

#define VER_CONV_BUFFER_SIZE 16

//! structure that passes information while parsing
typedef struct {
	plugin_manager_t *plugin_manager;
	offset_t sign_data;
	const char * file_path;
	
	// dependency processing 
	
	const char *value;
	dep_parser_status_t status;
	offset_t char_off;
	char crt_char;
	int last_was_comma;
	
	offset_t name_start;
	offset_t name_end;
	offset_t last_non_space;
	size_t name_sz;
	
	version_t version;
	long ver_parts[3];
	char ver_conv[VER_CONV_BUFFER_SIZE];
	offset_t ver_conv_loc;
	
	// matching plug-ins with signatured
	
	plugin_sign_t *crt_sign;
	plugin_data_t *crt_plugin;
	const char *crt_name;
	const char *crt_path;
	
} plugin_manager_inf_t;

//! types of standard paths
typedef enum {
	PMSTD_PROGPATH,
	PMSTD_HOME,
	PMSTD_UNIXROOT
} standard_path_t;

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

//! tell if the argument is yes (1), no (0) or something else
static inline func_error_t is_yes_no ( const char * value, int * out ) {
	if ( strcmp (value, "yes") == 0 ) {
		*out = 1;
	} else if ( strcmp (value, "no") == 0 ) {
		*out = 0;
	} else {
		return FUNC_GENERIC_ERROR;
	}
	return FUNC_OK;
}

//! we are ready to add a new dependency
static int plugin_manager_dep_add (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	DBG_ASSERT (data_->name_end >= data_->name_start);
	DBG_ASSERT (data_->name_end >= data_->last_non_space);
	DBG_ASSERT (data_->last_non_space >= data_->name_start);
	
	// make sure it's not empty
	data_->name_sz = data_->last_non_space - data_->name_start;
	if ( data_->name_sz == 0 ) {
		inf_dbg_message (
					"dependency entry: no plug-in name "
					"in value %s", data_->value);
		ret_code = 0;
	} else {
		
		// compute the space that we need and allocate
		size_t alloc_sz = sizeof(plugin_dep_t) + data_->name_sz + 1;
		offset_t off = accumulator_alloc (
		    &data_->plugin_manager->acm_sign, alloc_sz);
		if ( off == ACCUMULATOR_BAD_OFFSET ) {
			return FUNC_MEMORY_ERROR;
		}
		plugin_dep_t *new_dep = (plugin_dep_t*)accumulator_to_ptr(
		    &data_->plugin_manager->acm_sign, off);
		plugin_sign_t *new_sign = (plugin_sign_t*)accumulator_to_ptr(
		    &data_->plugin_manager->acm_sign, data_->sign_data);
		
		// fill in the structure
		new_dep->my_ver = data_->version;
		new_dep->next = new_sign->first_dep;
		new_sign->first_dep = off; // this becomes new first
		new_dep->str_sz = data_->name_sz;
		
		// copy the name
		char * name_copy = plugin_dep_name (new_dep);
		memcpy (name_copy, data_->value+data_->name_start, data_->name_sz);
		name_copy[data_->name_sz] = 0;
	}
	
	return ret_code;
}

//! waiting for the name to start
/// @param data_ all the variables regarding parsing
/// @return 0 to continue, anything else to break
static int plugin_manager_dep_wait_name (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	if (data_->crt_char == ',') {
		// ignore it (empty entry)
	} else if (data_->crt_char == ':') {
		inf_dbg_message (
					"dependency entry: waiting plug-in name, "
					"got : in value %s", data_->value);
		ret_code = 0;
	} else if (data_->crt_char > ' ') {
		
		// changing status; a new name is in da house
		data_->status = DEPENDPARSE_IN_NAME;
		data_->name_start = data_->char_off;
		data_->last_non_space = data_->char_off;
		data_->version = VERSION_DEFAULT;
		data_->ver_parts[0] = 0;
		data_->ver_parts[1] = 0;
		data_->ver_parts[2] = 0;
		data_->ver_conv[0] = 0;
		data_->ver_conv_loc = 0;
		
	}
	return ret_code;
}

//! in name, looking for an end or a version
/// @param data_ all the variables regarding parsing
/// @return 0 to continue, anything else to break
static int plugin_manager_dep_in_name (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	
	if (data_->crt_char == ',') {
		
		// this entry ends
		data_->name_end = data_->char_off;
		plugin_manager_dep_add (data_);
		
		// changing status
		data_->status = DEPENDPARSE_WAIT_NAME;
		
	} else if (data_->crt_char == ':') {
		
		// the version starts here
		data_->name_end = data_->char_off;
		data_->status = DEPENDPARSE_WAIT_VER_1;
		
	} else {
		
		// we want to trim trailing spaces from the name
		if ( data_->crt_char > ' ' ) {
			data_->last_non_space = data_->char_off+1;
		}
		
	}
	
	return ret_code;
}


//! at the end of a version component
/// @param data_ all the variables regarding parsing
/// @return 0 to continue, anything else to break
static int plugin_manager_dep_compute_version (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	int idx = data_->status - DEPENDPARSE_WAIT_VER_1;
	DBG_ASSERT( idx <= 2 );
	DBG_ASSERT( data_->ver_conv_loc < VER_CONV_BUFFER_SIZE );
	
	// convert to integer
	data_->ver_conv[data_->ver_conv_loc] = 0;
	char * out_buff;
	data_->ver_parts[idx] = strtol (data_->ver_conv, &out_buff, 10);
	if ( *out_buff != 0 ) {
		inf_dbg_message (
					"dependency entry: plug-in version "
					"expects x.y.z format in value %s", data_->value);
		ret_code = 0;
	}
	data_->ver_conv_loc = 0;
	data_->ver_conv[0] = 0;
	return ret_code;
}

//! in version, looking for an end or a version
/// @param data_ all the variables regarding parsing
/// @return 0 to continue, anything else to break
static int plugin_manager_dep_in_version (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	
	if (data_->crt_char == ',') {
		// save last part of the version
		ret_code = plugin_manager_dep_compute_version (data_);
		if ( ret_code == 0 ) {
			data_->version = version_from_int(
			(int)data_->ver_parts[0],
			(int)data_->ver_parts[1],
			(int)data_->ver_parts[2]);
		
			// this entry ends
			ret_code = plugin_manager_dep_add (data_);
			
			// changing status
			data_->status = DEPENDPARSE_WAIT_NAME;
		}
	} else if (data_->crt_char == '.') {
		
		// get component index; make sure we're good
		int idx = data_->status - DEPENDPARSE_WAIT_VER_1;
		if ( idx > 1 ) {
			inf_dbg_message (
						"dependency entry: plug-in version "
						"expects x.y.z format in value %s", data_->value);
			ret_code = 0;
		} else {
			// save this portion
			ret_code = plugin_manager_dep_compute_version (data_);
			data_->status = (dep_parser_status_t)(data_->status + 1);
		}
	} else {
		
		// save this character in the buffer to be converted latter
		if ( data_->ver_conv_loc >= VER_CONV_BUFFER_SIZE - 1 ) {
			inf_dbg_message (
						"dependency entry: plug-in version "
						"expects x.y.z format in value %s", data_->value);
			ret_code = 0;
		} else if ( data_->crt_char > ' ' ) {
			data_->ver_conv[data_->ver_conv_loc] = data_->crt_char;
			data_->ver_conv_loc++;
		}
		
	}
	return ret_code;
}

//! decide what to do next based on the current character
/// @param data_ all the variables regarding parsing
/// @return 0 to continue, anything else to break
static int plugin_manager_char_dep (plugin_manager_inf_t * data_)
{
	int ret_code = 0;
	switch (data_->status) {
	case DEPENDPARSE_WAIT_NAME: {
		ret_code = plugin_manager_dep_wait_name (data_);
		break; }
	case DEPENDPARSE_IN_NAME: {
		ret_code = plugin_manager_dep_in_name (data_);
		break; }
	case DEPENDPARSE_WAIT_VER_1:
	case DEPENDPARSE_WAIT_VER_2:
	case DEPENDPARSE_WAIT_VER_3: {
		ret_code = plugin_manager_dep_in_version (data_);
		break; }
	}
	return ret_code;
}

//! parse the value of a "dependency" key
/// @param data_ all the variables regarding parsing
/// @param value_ the string to parse
/// @return 0 to continue, anything else to break
static int plugin_manager_parse_dep ( 
    plugin_manager_inf_t * data_, const char* value_)
{

	int ret_code = 0;
	data_->value = value_;
	
	// initialisation
	data_->status = DEPENDPARSE_WAIT_NAME;
	data_->char_off = 0;
	data_->last_was_comma = 0;
	
	// loop in each character in value_
	for ( ;; ) {
		// read the next character; break on ending NULL
		data_->crt_char = value_[data_->char_off];
		if ( data_->crt_char == 0 )
			break;
		
		// process this character
		ret_code = plugin_manager_char_dep (data_);
		if ( ret_code != 0 )
			break;
		
		// advance
		data_->char_off++;
		data_->last_was_comma = (data_->crt_char == ',');
	}
	
	// a fake , to force processing last entry, if any
	if ( !data_->last_was_comma && (ret_code == 0 ) ) {
		data_->crt_char = ',';
		ret_code = plugin_manager_char_dep (data_);
	}
	
	return ret_code;
}

//! process an inf name_=value_ entry in given section_
/// @param user_ an plugin_manager_inf_t instance
/// @param section_ the section in inf or "" if before previous section
/// @param name_ the name part of the pair
/// @param value_ the value part of the pair, trimmed
/// @return nonzero on success, zero on error.
static int plugin_manager_inf(void* user_, const char* section_, const char* name_,
				   const char* value_)
{

	int ret_val = 1;
	// remember that memory is inside an accumulator, so do NOT free previous values
	plugin_manager_inf_t* data = (plugin_manager_inf_t*)user_;
	plugin_sign_t * the_sign;
	offset_t in_buff;
	int ver_ints[3];
	int yes_no;
	
	if ( strcmp (section_, "general") == 0 ) {
		if ( strcmp (name_, "name") == 0 ) {
			SET_SIGN_STRING(pretty_name);
		} else if ( strcmp (name_, "description") == 0 ) {
			SET_SIGN_STRING(description);
		} else if ( strcmp (name_, "version") == 0 ) {
			SET_SIGN_VERSION(my_ver);
		} else if ( strcmp (name_, "manager") == 0 ) {
			SET_SIGN_VERSION(mng_ver);
		} else if ( strcmp (name_, "dependencies") == 0 ) {
			ret_val = (plugin_manager_parse_dep (data, value_) == FUNC_OK);
		} else {
			inf_dbg_message (
						"Unknown entry %s in section %s",
						value_, section_ );
			ret_val = 0;
		}
	} else if ( strcmp (section_, "aitown") == 0 ) {
		if ( strcmp (name_, "server") == 0 ) {
			SET_SIGN_FLAG(PLUGIN_SIGN_FOR_SERVER);
		} else if ( strcmp (name_, "client") == 0 ) {
			SET_SIGN_FLAG(PLUGIN_SIGN_FOR_CLIENT);
		} else {
			inf_dbg_message (
						"Unknown entry %s in section %s",
						value_, section_ );
			ret_val = 0;
		}
	} else {
		inf_dbg_message (
					"Unknown section %s",
					section_ );
		ret_val = 0;
	}
	return ret_val;
}

#define PLUGIN_MATCH_NOT_FOUND FUNC_OK
#define PLUGIN_MATCH_FOUND FUNC_GENERIC_ERROR

//! compare the names to see if signature and plugin are the same
/// @return PLUGIN_MATCH_NOT_FOUND to keep searching, PLUGIN_MATCH_FOUND otherwise
static func_error_t plugin_manager_set_assoc_plugin ( 
    plugin_manager_t *plugin_manager_, plugin_data_t *plugin, void *user_data)
{
	VAR_UNUSED (plugin_manager_);
	plugin_manager_inf_t *data = (plugin_manager_inf_t*)user_data;
	if ( strcasecmp (plugin->name, data->crt_name) == 0 ) {
		plugin->sign = data->sign_data;
		data->crt_sign->loaded_plugin = plugin;
		data->crt_plugin = plugin;
		return PLUGIN_MATCH_FOUND;
	}
	return PLUGIN_MATCH_NOT_FOUND;
}

//! scan one .inf file indicated by path_
/// This method conforms to the dir_iterator_foreach_t type.
/// @param path_ full file path
/// @param name_ points inside same buffer as path_
/// @param user_data_ pointer to a plugin_manager_t structure
/// @param is_file_ is set if the entry is a file (should always be)
/// @return FUNC_OK on success, other func_error_t on error
static func_error_t plugin_manager_scan_an_inf ( const char * path_, const char * name_,
    void * user_data_, int is_file_)
{
	inf_dbg_message ("    file %s", path_);
	
	DBG_ASSERT (is_file_);
	plugin_manager_t *plugin_manager = (plugin_manager_t *)user_data_;
	
	// allocate space for path and name; copy them
	size_t name_sz = strlen(name_);
	size_t path_sz = PTR_OFF(path_,name_) + name_sz;
	offset_t path_lib = accumulator_add_string (
	    &plugin_manager->acm_sign, path_, path_sz);
	if ( path_lib == ACCUMULATOR_BAD_OFFSET )
		return FUNC_MEMORY_ERROR;
	
	// allocate space for name; copy it
	size_t name_base_sz = name_sz - 4; // we know is ".inf"
	offset_t plugin_name = accumulator_add_string (
	    &plugin_manager->acm_sign, name_, name_base_sz );
	if ( plugin_name == ACCUMULATOR_BAD_OFFSET )
		return FUNC_MEMORY_ERROR;
	
	// allocate a new structure for this file and initialise it
	offset_t new_sign_off = accumulator_alloc(
	    &plugin_manager->acm_sign, sizeof(plugin_sign_t) );
	if ( new_sign_off == 0 )
		return FUNC_MEMORY_ERROR;
	{
		plugin_sign_t * new_sign = (plugin_sign_t*)accumulator_to_ptr(
					&plugin_manager->acm_sign, new_sign_off);
		func_error_t err_code = plugin_sign_init (new_sign);
		DBG_ASSERT (err_code==FUNC_OK);
        VAR_UNUSED (err_code);
		// fill in the structure
		new_sign->path_lib = path_lib;
		new_sign->plugin_name = plugin_name;
	}

	// read the file and interpret its content
	plugin_manager_inf_t inf_data = {plugin_manager, new_sign_off, path_};
	if ( ini_parse (path_, plugin_manager_inf, &inf_data) < 0 ) {
		plugin_sign_t * new_sign = (plugin_sign_t*)accumulator_to_ptr(
					&plugin_manager->acm_sign, new_sign_off);
		new_sign->flags = PLUGIN_SIGN_FAIL_TO_LOAD;
	} else {
		// insert the new structure into the chain
		plugin_sign_t * new_sign = (plugin_sign_t*)accumulator_to_ptr(
					&plugin_manager->acm_sign, new_sign_off);
		new_sign->next = plugin_manager->first_sign;
		plugin_manager->first_sign = new_sign_off;
		
		// change the name to point to the binary
		inf_data.crt_name = accumulator_to_char(
		    &plugin_manager->acm_sign, plugin_name);
		inf_data.crt_path = accumulator_to_char(
		    &plugin_manager->acm_sign, path_lib);
		char * p_ext = (char*)inf_data.crt_path + path_sz - 3; // inf is 3 characters long
		plugin_manager_shared_binary_extension (p_ext);
		
		// see if this plug-in is loaded; save the association
		inf_data.crt_sign = new_sign;
		inf_data.crt_plugin = NULL;
		plugin_manager_foreach_plugin( 
		    plugin_manager, 
		    plugin_manager_set_assoc_plugin,
		    &inf_data);
		
		plugin_manager->sign_count++;
	}

	return FUNC_OK;
}
	
//! scan one directory indicated by path_
/// This method conforms to the linked_list_str_foreach_t type.
/// @param path_ the directory to scan
/// @param manager_ a plugin_manager_t instance
/// @return FUNC_OK or the error returned by last plugin_manager_scan_an_inf()
static func_error_t plugin_manager_scan_a_path(const char * path_, void * manager_)
{
	func_error_t err_code = FUNC_OK;
	inf_dbg_message ("  path %s", path_);
	
	access_rights_t ar = du_existsdir (path_);
	if ( ( ar & DU_READABLE ) && ( ar & DU_EXECUTABLE ) ) {
		err_code = dir_iterator (
					path_,
					"*.inf",
					DIR_ITERATOR_EXCLUDE_DIRECTORIES, 
					plugin_manager_scan_an_inf,
					manager_);
	} else {
		inf_dbg_message ("    path %s not used; exists() returns %d", path_, (int)ar);
	}
	return err_code;
}

static func_error_t plugin_manager_scan_standard_path (
    plugin_manager_t *plugin_manager_, standard_path_t type, 
    const char * path_, size_t path_sz_ )
{
	func_error_t err_code;
	char * src = NULL;
	int b_free_buffer = 0;

	if ( path_sz_ == 0 ) {
		path_sz_ = strlen (path_);
	}
	
	switch (type) {
	case PMSTD_PROGPATH: {
		// get program path (includes the name of the executable)
		src = plugin_manager_program_path();
		if (src == NULL) {
			return FUNC_MEMORY_ERROR;
		}
		// locate last part (exe name) and trim it
		int i; 
		int last_separator = 0;
		for ( i = 0; ;i++ ) {
			char c = src[i];
			if ( c == 0 ) {
				break;
			} else if ( ( c == '/' ) || ( c == '\\' ) ) {
				last_separator = i;
			}
		}
		if ( last_separator == 0 ) {
			free (src);
			src = NULL;
			
		} else {
			src[last_separator] = 0;
			b_free_buffer = 1;
		}
		break; }
	case PMSTD_HOME: { src = getenv ("HOME"); break; }
	case PMSTD_UNIXROOT: { src = getenv ("UNIXROOT"); break; }
	}
	if ( src == NULL )
		return FUNC_OK;
	
	// allocate buffer
	size_t src_sz = strlen (src);
	STACKBUFF_INIT (char, buff, 256, src_sz+path_sz_+1);
	if ( buff_ptr == NULL ) {
		if (b_free_buffer) {
			free (src);
		}
		return FUNC_MEMORY_ERROR;
	}
	
	// glue parts together

	memcpy (buff_ptr, src, src_sz);
	memcpy (buff_ptr+src_sz, path_, path_sz_);
	buff_ptr[buff_actual_sz-1] = 0;
	
	// actual scan
	err_code = plugin_manager_scan_a_path (buff_ptr, plugin_manager_ );
	
	if (b_free_buffer) {
		free (src);
	}
		
	// free resources
	STACKBUFF_END(buff);
	return err_code;
}

static func_error_t plugin_manager_rem_assoc_plugin ( 
    plugin_manager_t *plugin_manager_, plugin_data_t *plugin, void *user_data)
{
	VAR_UNUSED (plugin_manager_);
	VAR_UNUSED (user_data);
	plugin->sign = ACCUMULATOR_BAD_OFFSET;
	return FUNC_OK;
}

func_error_t plugin_manager_rescan (plugin_manager_t *plugin_manager_)
{
	func_error_t err_code;
	inf_dbg_message ("Scanning all paths for plugins");
	
	// clear previous buffer, if any; reset the states
	size_t prev_alloc = plugin_manager_->acm_sign.allocated / 2;
	size_t prev_step = prev_alloc / 2;
	if ( prev_alloc < 1024 )
		prev_alloc = 1024;
	if ( prev_step < 1024*16 )
		prev_step = 1024*16;
	accumulator_end (&plugin_manager_->acm_sign);
	accumulator_init (&plugin_manager_->acm_sign, prev_alloc );
	plugin_manager_->acm_sign.step = prev_step;
	plugin_manager_->first_sign = ACCUMULATOR_BAD_OFFSET;
	plugin_manager_->sign_count = 0;
	
	// remove association for loaded plug-ins
	plugin_manager_foreach_plugin( 
	    plugin_manager_, 
		plugin_manager_rem_assoc_plugin,
		NULL);
	
	// scan standard paths
	err_code = plugin_manager_scan_standard_path (
	    plugin_manager_, PMSTD_PROGPATH, 
	    "/plugins", sizeof("/plugins") );
	char * env_path = getenv ("AITOWN_PLUGIN_DIR");
	if ( env_path != NULL ) {
		err_code = plugin_manager_scan_a_path (
	        env_path, plugin_manager_);
	}
	err_code = plugin_manager_scan_standard_path (
	    plugin_manager_, PMSTD_HOME, 
	    "/.aitown/plugins", sizeof("/.aitown/plugins") );
	err_code = plugin_manager_scan_standard_path (
	    plugin_manager_, PMSTD_UNIXROOT, 
	    "/usr/lib/aitown/plugins", sizeof("/usr/lib/aitown/plugins") );
	err_code = plugin_manager_scan_a_path (
	    "/usr/lib/aitown/plugins", plugin_manager_);
	err_code = plugin_manager_scan_standard_path (
	    plugin_manager_, PMSTD_UNIXROOT, 
	    "/lib/aitown/plugins", sizeof("/lib/aitown/plugins") );
	err_code = plugin_manager_scan_a_path (
	    "/lib/aitown/plugins", plugin_manager_);
	
	// scan custom paths
	err_code = linked_list_str_foreach (
	    plugin_manager_->user_paths_first,
	    plugin_manager_scan_a_path,
	    plugin_manager_);
	
	return err_code;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
