/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dstorage_handle.c
  \date			September 2013
  \author		TNick

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

#include "ini.h"
#include "struct_ini.h"
#include <string.h>
#include <stdlib.h>

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

void struct_ini_init (struct_ini_t *sini)
{
    memset (sini, 0, sizeof(struct_ini_t));
}

void struct_ini_end (struct_ini_t *sini)
{
    // iterate in sections
    struct_ini_sect_t * iter = sini->p_first;
    struct_ini_sect_t * iter_next;
    while (iter != NULL) {
        iter_next = iter->p_next;

        // iterate in values
        struct_ini_value_t * iterval = iter->p_first;
        struct_ini_value_t * itervalnext;
        while (iterval != NULL) {
            itervalnext = iterval->p_next;

            // free the name, value, itself and get to next
            if (iterval->name != NULL) free (iterval->name);
            if (iterval->value != NULL) free (iterval->value);
            free (iterval);
            iterval = itervalnext;
        }

        // free the name, itself and get to next
        if (iter->name != NULL) free (iter->name);
        free (iter);

        iter = iter_next;
    }

    memset (sini, 0, sizeof(struct_ini_t));
}


struct_ini_sect_t * struct_ini_find_section_0 (struct_ini_t *sini, const char * p_name )
{
    // find the section
    struct_ini_sect_t * iter = sini->p_first;
    while (iter != NULL) {
        if (!strcasecmp(p_name, iter->name)) {
            return iter;
        }
        iter = iter->p_next;
    }
    return NULL;
}

struct_ini_sect_t * struct_ini_find_section (struct_ini_t *sini, const char * p_name )
{
    // find the section
    struct_ini_sect_t * iter = sini->p_first;
    struct_ini_sect_t * iter_prev = NULL;
    while (iter != NULL) {
        if (!strcasecmp(p_name, iter->name)) {
            return iter;
        }
        iter_prev = iter;
        iter = iter->p_next;
    }

    // allocate a new section
    iter = malloc (sizeof(struct_ini_sect_t));
    memset(iter, 0, sizeof(struct_ini_sect_t));
    iter->name = strdup(p_name);

    // add the section in chain
    if ( iter_prev ) {
        iter_prev->p_next = iter;
    } else {
        sini->p_first = iter;
    }

    return iter;
}

struct_ini_value_t * struct_ini_find_value_0 (struct_ini_sect_t *sect, const char * p_name )
{
    // find the value
    struct_ini_value_t * iter = sect->p_first;
    while (iter != NULL) {
        if (!strcasecmp(p_name, iter->name)) {
            return iter;
        }
        iter = iter->p_next;
    }

    return NULL;
}

struct_ini_value_t * struct_ini_find_value (struct_ini_sect_t *sect, const char * p_name )
{
    // find the value
    struct_ini_value_t * iter = sect->p_first;
    struct_ini_value_t * iter_prev = NULL;
    while (iter != NULL) {
        if (!strcasecmp(p_name, iter->name)) {
            return iter;
        }
        iter_prev = iter;
        iter = iter->p_next;
    }

    // allocate a new value
    iter = malloc (sizeof(struct_ini_value_t));
    memset(iter, 0, sizeof(struct_ini_value_t));
    iter->name = strdup(p_name);

    // add the section in chain
    if ( iter_prev ) {
        iter_prev->p_next = iter;
    } else {
        sect->p_first = iter;
    }

    return iter;
}

static int struct_handler(void* user_, const char* section_, const char* name_,
                   const char* value_)
{
    struct_ini_t * sini = (struct_ini_t*)user_;
    struct_ini_sect_t * sect = struct_ini_find_section(sini,section_);
    if (sect == NULL)
        return INIH_KB_ERROR;
    struct_ini_value_t * entry = struct_ini_find_value (sect, name_);
    if (entry == NULL)
        return INIH_KB_ERROR;
    entry->value = strdup(value_);
    return INIH_KB_SUCCESS;
}

int struct_ini_parse (struct_ini_t *sini, const char *p_file)
{
    int i_parse = ini_parse (p_file, struct_handler, sini);
    return i_parse;
}
int struct_ini_parse_file (struct_ini_t *sini, FILE *f)
{
    int i_parse = ini_parse_file (f, struct_handler, sini);
    return i_parse;
}

int struct_ini_init_and_parse (struct_ini_t *sini, const char *p_file)
{
    struct_ini_init (sini);
    int i_parse = ini_parse (p_file, struct_handler, sini);
    return i_parse;
}

int struct_ini_get (struct_ini_t **sini, const char *p_file)
{
    // allocate memory
    struct_ini_t *tr = malloc(sizeof(struct_ini_t));
    *sini = NULL;
    if (tr == NULL)
        return INIH_ERR_MALLOC;
    struct_ini_init (tr);

    // parse the file
    int i_parse = ini_parse (p_file, struct_handler, tr);
    if (INIH_IS_OK(i_parse)) {
        *sini = tr;
    } else {
        struct_ini_end (tr);
        free (tr);
    }
    return i_parse;
}

int struct_ini_get_file (struct_ini_t **sini, FILE *f)
{
    // allocate memory
    struct_ini_t *tr = malloc(sizeof(struct_ini_t));
    *sini = NULL;
    if (tr == NULL)
        return INIH_ERR_MALLOC;
    struct_ini_init (tr);

    // parse the file
    int i_parse = ini_parse_file (f, struct_handler, tr);
    if (INIH_IS_OK(i_parse)) {
        *sini = tr;
    } else {
        struct_ini_end (tr);
        free (tr);
    }
    return i_parse;
}

const char * struct_ini_get_value (
        struct_ini_t *sini, const char * p_sect, const char * p_entry )
{
    struct_ini_sect_t * sect = struct_ini_find_section_0(sini, p_sect);
    if (sect == NULL)
        return NULL;
    struct_ini_value_t * entry = struct_ini_find_value_0(sect, p_entry);
    if (entry == NULL)
        return NULL;
    return entry->value;
}

struct_ini_status_t struct_ini_get_int (
        struct_ini_t *sini, const char * p_sect, const char * p_entry,
        long int *ret_val, long int default_val )
{
    const char * v = struct_ini_get_value (sini, p_sect, p_entry);
    if (v == NULL) {
        *ret_val = default_val;
        return STRUCT_INI_NOT_FOUND;
    }

    // convert
    char * p_end;
    long int ret = strtol (v, &p_end, 0);

    // check that entire string was resolved (allow white spaces afterwards)
    while (*p_end != 0) {
        if ((*p_end != ' ') && (*p_end != 9)) {
            *ret_val = default_val;
            return STRUCT_INI_CANT_CONVERT;
        }
    }

    // that's your answer
    *ret_val = ret;
    return STRUCT_INI_OK;
}

struct_ini_status_t struct_ini_get_double (
        struct_ini_t *sini, const char * p_sect, const char * p_entry,
        double *ret_val, double default_val )
{
    const char * v = struct_ini_get_value (sini, p_sect, p_entry);
    if (v == NULL) {
        *ret_val = default_val;
        return STRUCT_INI_NOT_FOUND;
    }

    // convert
    char * p_end;
    double ret = strtod (v, &p_end);

    // check that entire string was resolved (allow white spaces afterwards)
    while (*p_end != 0) {
        if ((*p_end != ' ') && (*p_end != 9)) {
            *ret_val = default_val;
            return STRUCT_INI_CANT_CONVERT;
        }
    }

    // that's your answer
    *ret_val = ret;
    return STRUCT_INI_OK;
}




/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
