/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_clist.h
  \date			September 2013
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef struct_ini_h_INCLUDE
#define struct_ini_h_INCLUDE
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

struct _struct_ini_value_t;

//! a section structure
typedef struct _struct_ini_sect_t {
    char* name;
    struct _struct_ini_value_t* p_first;
    struct _struct_ini_sect_t* p_next;
} struct_ini_sect_t;

//! a value structure
typedef struct _struct_ini_value_t {
    char* name;
    char* value;
    struct _struct_ini_value_t* p_next;
} struct_ini_value_t;

//! describes a controller list structure
///
/// The implementation is not a fast one; we're not using hashes or a B-tree,
/// for example
typedef struct _struct_ini_t {
    struct _struct_ini_sect_t* p_first;
} struct_ini_t;

//! status for some functions
typedef enum {
    STRUCT_INI_OK,
    STRUCT_INI_NOT_FOUND,
    STRUCT_INI_CANT_CONVERT
} struct_ini_status_t;

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

//! initialize a structure;
void
struct_ini_init (struct_ini_t *sini);

//! terminate a structure and free all memory;
void
struct_ini_end (struct_ini_t *sini);

//! parse a file
int
struct_ini_parse (struct_ini_t *sini, const char *p_file);

//! parse a file
int
struct_ini_parse_file (struct_ini_t *sini, FILE *f);

//! initialise and parse a file
int
struct_ini_init_and_parse (struct_ini_t *sini, const char *p_file);

//! create a structure using malloc(), initialise and parse a file
///
/// the pointer is valid only if result is INIH_ERR_SUCCESS
/// and must be freed by the caller.
int
struct_ini_get (struct_ini_t **sini, const char *p_file);

//! create a structure using malloc(), initialise and parse a file
///
/// the pointer is valid only if result is INIH_ERR_SUCCESS
/// and must be freed by the caller.
int
struct_ini_get_file (struct_ini_t **sini, FILE *f);


//! find a section (case insensitive)
struct_ini_sect_t *
struct_ini_find_section (struct_ini_t *sini, const char * p_name);

//! find a value in a section (case insensitive)
struct_ini_value_t *
struct_ini_find_value (struct_ini_sect_t *sect, const char * p_name );

//! find a section (case insensitive)
/// @return NULL if not found
struct_ini_sect_t *
struct_ini_find_section_0 (struct_ini_t *sini, const char * p_name);

//! find a value in a section (case insensitive)
/// @return NULL if not found
struct_ini_value_t *
struct_ini_find_value_0 (struct_ini_sect_t *sect, const char * p_name );


//! find a value in a section (case insensitive); if not found returns NULL
/// @return a pointer to internally stored buffer
const char *
struct_ini_get_value (struct_ini_t *sini, const char * p_sect, const char * p_entry );


//! find an integer in a section (case insensitive)
///
/// if not found returns default value;
/// if it can't be converted an error message is presented; default value
/// still returned.
/// @return the value, converted from internal string, or default value
struct_ini_status_t
struct_ini_get_int (
        struct_ini_t *sini,
        const char * p_sect,
        const char * p_entry,
        long int *ret_val,
        long int default_val);


//! find an double precision floating point in a section (case insensitive)
///
/// if not found returns default value;
/// if it can't be converted an error message is presented; default value
/// still returned.
/// @return the value, converted from internal string, or default value
struct_ini_status_t
struct_ini_get_double (
        struct_ini_t *sini,
        const char * p_sect,
        const char * p_entry,
        double *ret_val,
        double default_val);



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
#endif // struct_ini_h_INCLUDE
