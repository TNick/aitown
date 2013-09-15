/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin_win.h
  \date			September 2013
  \author		TNick
  
*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_plugin_win_h_INCLUDE
#define AITOWN_plugin_win_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif 

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

#ifdef PLUGIN_MANAGER_C
//! load a shared library
static inline func_error_t
plugin_manager_load_binary (plugin_data_t *ret_ptr_, const char * path_)
{
	/** @todo windows */
	return FUNC_OK;
}
#endif

#ifdef PLUGIN_DATA_C
//! unload the binary
static void
plugin_manager_unload_binary (plugin_data_t **plugin_)
{
	/** @todo windows */
}
#endif

//! call initialisation function
static inline func_error_t
plugin_manager_call_init (plugin_manager_t *plugin_manager_, plugin_data_t *ret_ptr_)
{
	func_error_t err_code = FUNC_OK;
	/** @todo windows */
	return err_code;
}

//! extension for plug-in binary (max 3 characters)
static inline void
plugin_manager_shared_binary_extension (char * out_buff)
{
	out_buff[0] = 'd'; out_buff[1] = 'l'; out_buff[2] = 'l';
}

//! get the path of the executable
static inline char *
plugin_manager_program_path ()
{
	int i;
	int buff_sz = 256;
	int result;
	char * buff;
	for ( i = 0; i < 10; i++ ) {
		char * buff = (char*)malloc(buff_sz);
		result = GetModuleFileName (NULL, buff, buff_sz);
		if ( result == 0 ) {
			dbg_message ("Failed to retrieve process directory; error code: %d", GetLastError());
			free (buff);
			return NULL;
		}
		if ( result != buff_sz ) {
			break;
		}
		free (buff);
		buff_sz += 256;
		buff = NULL;
	}
	// no need to shrink it, the buffer is short lived anyway
	return buff;
}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif 
#endif /* AITOWN_plugin_win_h_INCLUDE */
