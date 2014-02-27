/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg.h
  \date			September 2013
  \author		TNick

  This file defines aitown_cfg_t data structure that represents a visual
  sensor and related functions.



*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef AITOWN_cfg_h_INCLUDE
#define AITOWN_cfg_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <aitown/aitown_global.h>
#include <aitown/aitown-cfg-config.h>
#include <aitown/error_codes.h>

#include <aitown/aitown-cfg-sect.h>
#include <aitown/aitown-cfg-leaf.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _aitimage_t;
struct _aitown_cfg_sect_t;
struct _aitown_cfg_leaf_t;

//! describes a cfg instance
///
typedef struct _aitown_cfg_t {
    const char * file_path; /**< path to the file associated with with this structure */
    aitown_cfg_sect_t root; /**< root section */
    int ref_count;
} aitown_cfg_t;

//! the character used as comment marker
#define AITOWN_CFG_COMMENT ';'
#define AITOWN_CFG_COMMENT_STR ";"

//! the character that marks section name start
#define AITOWN_CFG_SECT_S '['
#define AITOWN_CFG_SECT_S_STR "["

//! the character used as path separator
#define AITOWN_CFG_SEPARATOR '/'
#define AITOWN_CFG_SEPARATOR_STR "/"

//! the character that marks section name end
#define AITOWN_CFG_SECT_E ']'
#define AITOWN_CFG_SECT_E_STR "]"

//! the character that separates key from value
#define AITOWN_CFG_ASSIGN1 '='
#define AITOWN_CFG_ASSIGN1_STR "="

//! the character that separates key from value
#define AITOWN_CFG_ASSIGN2 ':'
#define AITOWN_CFG_ASSIGN2_STR ":"

//! the character used as line continuation
#define AITOWN_CFG_CONCAT '\\'
#define AITOWN_CFG_CONCAT_STR "\\"



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


//! initialize a config head structure
///
/// @param cfg          address of the structure to initialise
/// @param file_path    the file to parse
///
AITOWN_EXPORT func_error_t
aitown_cfg_init (
        aitown_cfg_t **cfg,
        const char *file_path
        );


//! aquire a reference
///
/// @param cfg   address of the structure
/// @param owner address of the owner of returned reference
///
AITOWN_EXPORT void
aitown_cfg_incref (
        aitown_cfg_t *cfg,
        void * owner);


//! release a reference
///
/// @param cfg   address of the structure to release
/// @param owner address of the owner of returned reference
///
AITOWN_EXPORT void
aitown_cfg_decref (
        aitown_cfg_t *cfg,
        void * owner);


//! save the content to a file
///
/// @param cfg          address of the structure to save
/// @param file_path    destination path (may be NULL to use associated path)
///
AITOWN_EXPORT func_error_t
aitown_cfg_save (
        aitown_cfg_t *cfg,
        const char *file_path
        );


//! parse a file and load its content into this structure
///
/// @param cfg   address of the destination structure
/// @param file_path    the file to parse
///
/// @warning internal file pointer is not reset; the structure
/// remains associated with initial file set through aitown_cfg_init(); this
/// function may be used to load additional content.
///
AITOWN_EXPORT func_error_t
aitown_cfg_parse_file (
        aitown_cfg_t *cfg,
        const char *file_path
        );

//! parse a file and load its content into this structure
///
/// @param cfg   address of the destination structure
/// @param file_path    the file to parse
///
/// This function may be used to load additional content.
///
AITOWN_EXPORT func_error_t
aitown_cfg_parse_buffer (
        aitown_cfg_t *cfg,
        const char *file_content,
        unsigned * err_line,
        const char ** err_string
        );

//! interpret the path (sections and leaf) and return the leaf at the end of it
///
/// Example: a/b/c will look for section a in root section, then for
/// section b in section a and for the key c in section b. White space
/// before and after the separator and on the sides of the string are ignored
/// (ie. "   a  /  b  /  c  " is a legal string, and so is
/// "  a  1 / b 1 / c 1 ", with internal string being part of the name).
///
AITOWN_EXPORT aitown_cfg_leaf_t *
aitown_cfg_get (
        aitown_cfg_t *cfg,
        const char * name);

//! interpret the path (sections and leaf) and return the leaf at the end of it
///
/// Example: a/b/c will look for section a in provided section, then for
/// section b in section a and for the key c in section b. White space
/// before and after the separator and on the sides of the string are ignored
/// (ie. "   a  /  b  /  c  " is a legal string, and so is
/// "  a  1 / b 1 / c 1 ", with internal string being part of the name).
///
AITOWN_EXPORT aitown_cfg_leaf_t *
aitown_cfg_get_rel (
        aitown_cfg_sect_t *sect,
        const char * name);

//! locate a section inside a parent section
///
/// The string is searched exactly as it is, without any trimming or other
/// adjustments.
///
AITOWN_EXPORT aitown_cfg_sect_t *
aitown_cfg_get_sect (
        aitown_cfg_sect_t * section,
        const char * name
        );

//! locate a leaf inside a section
///
/// The string is searched exactly as it is, without any trimming or other
/// adjustments.
///
AITOWN_EXPORT aitown_cfg_leaf_t *
aitown_cfg_get_leaf (
        aitown_cfg_sect_t * section,
        const char * name);



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
#endif /* AITOWN_cfg_h_INCLUDE */
