/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-cfg.c
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

#include "aitown-cfg.h"

#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/pointer_aritmetic.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

aitown_cfg_node_t * get_node_by_bounded_name(
        aitown_cfg_node_t * first, const char * str, const char * str_end);


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

func_error_t aitown_cfg_init (
        aitown_cfg_t **cfg, const char * file_path)
{
    aitown_cfg_t *out;

    // check input
    DBG_ASSERT (cfg != NULL);
    *cfg = NULL;
    if ((file_path == NULL) || (file_path[0] == 0)) {

        // create this thingy
        out = (aitown_cfg_t*)malloc (sizeof(aitown_cfg_t));
        if (out == NULL) {
            return FUNC_MEMORY_ERROR;
        }

        // clear it
        memset (out, 0, sizeof(aitown_cfg_t));
        out->root.node.root = out;

        // set reference count to 1
        out->ref_count = 1;

        *cfg = out;
        return FUNC_BAD_INPUT;
    }

    // see the lenght of the chunk
    size_t sz_path = strlen (file_path);
    size_t sz_tot = sizeof(aitown_cfg_t) + sz_path + 1;

    // create this thingy
    out = (aitown_cfg_t*)malloc (sz_tot);
    if (out == NULL) {
        return FUNC_MEMORY_ERROR;
    }

    // clear it
    memset (out, 0, sizeof(aitown_cfg_t));
    out->root.node.root = out;


    // copy the path inside
    out->file_path = PTR_ADD (out, sizeof(aitown_cfg_t));
    memcpy ((void*)out->file_path, file_path, sz_path+1); // out->file_path[sz_path] = 0;

    // set reference count to 1
    out->ref_count = 1;

    func_error_t ret = aitown_cfg_parse_file (out, file_path);
    if (ret != FUNC_OK) {
        free (out);
        return ret;
    }

    // and that's that
    *cfg = out;
    return ret;
}


void aitown_cfg_incref (aitown_cfg_t *cfg, void * owner)
{
    cfg->ref_count++;
    // we may decide later to add (maybe for debug) a list of owners
    VAR_UNUSED (owner);
}

void aitown_cfg_decref (aitown_cfg_t *cfg, void * owner)
{
    DBG_ASSERT (cfg != NULL);

    if (cfg == NULL)
        return;

    if (--cfg->ref_count <= 0 ) {
        // check for a changed name
        char * name_loc = (char*)PTR_ADD (cfg, sizeof(aitown_cfg_t));
        if (name_loc != cfg->file_path) {
            free ((void*)cfg->file_path);
        }

        // clear it and free it
        memset (cfg, 0, sizeof(aitown_cfg_t));
        free (cfg);
    }

    // we may decide later to add (maybe for debug) a list of owners
    VAR_UNUSED (owner);
}

func_error_t save_a_leaf_chain (FILE * f, aitown_cfg_leaf_t  * leaf)
{
    DBG_ASSERT (f != NULL);
    if (leaf == NULL) {
        return FUNC_OK;
    }

    func_error_t ret = FUNC_OK;

    // save in reverse order
    if (leaf->node.next != NULL) {
        ret = save_a_leaf_chain (f, (aitown_cfg_leaf_t*)leaf->node.next);
    }

    // save us (!)
    if (leaf->value == NULL) {
        fprintf (f, "%s = \n", leaf->node.name);
    } else {
        fprintf (f, "%s = \"%s\"\n", leaf->node.name, leaf->value);
    }
    return ret;
}

typedef struct _text_chain_t {
    const char * p_text;
    struct _text_chain_t * p_next;
} text_chain_t;

typedef struct _save_a_sect_data {
    FILE * f;
    text_chain_t * p_first;
} save_a_sect_data;

func_error_t save_a_sect_chain (
        save_a_sect_data * d, aitown_cfg_sect_t  * sect, text_chain_t * prev_s)
{
    DBG_ASSERT (d->f != NULL);
    DBG_ASSERT (sect != NULL);

    func_error_t ret = FUNC_OK;
    text_chain_t * iter;

    // save siblings in reverse order
    if (sect->node.next != NULL) {
        ret = save_a_sect_chain (d, (aitown_cfg_sect_t*)sect->node.next, prev_s);
        if (ret != FUNC_OK) {
            return ret;
        }
    }

    // trim out empty sections
    // if ((sect->first_leaf == NULL) && (sect->first_sect == NULL))
    // {
    //     return ret;
    // }

    // save our section name
    if (sect->node.parent != NULL) {
        fprintf (d->f, "[");
        iter = d->p_first;
        while (iter != NULL) {
            fprintf (d->f, "%s/", iter->p_text);
            iter = iter->p_next;
        }
        fprintf (d->f, "%s]\n", sect->node.name);
    }

    // save leafs
    if (sect->first_leaf != NULL) {
        ret = save_a_leaf_chain (d->f, sect->first_leaf);
        if (ret != FUNC_OK) {
            return ret;
        }
    }

    // save child sections
    text_chain_t me = {sect->node.name, NULL};
    if (sect->first_sect != NULL) {
        if (sect->node.parent == NULL) {
            DBG_ASSERT (d->p_first == NULL);
            DBG_ASSERT (prev_s == NULL);

            ret = save_a_sect_chain (d, sect->first_sect, prev_s);
        }
        else if (prev_s == NULL) {
            DBG_ASSERT (d->p_first == NULL);

            d->p_first = &me;
            ret = save_a_sect_chain (d, sect->first_sect, &me);
            d->p_first = NULL;
        } else {
            DBG_ASSERT (d->p_first != NULL);
            DBG_ASSERT (prev_s->p_next == NULL);

            prev_s->p_next = &me;
            ret = save_a_sect_chain (d, sect->first_sect, &me);
            prev_s->p_next = NULL;
        }
    }

    return ret;
}

func_error_t aitown_cfg_save ( aitown_cfg_t *cfg, const char *file_path)
{
    func_error_t ret = FUNC_OK;
    FILE * f = NULL;

    // check input
    DBG_ASSERT (cfg != NULL);
    if ((file_path == NULL) || (file_path[0] == 0)) {
        file_path = cfg->file_path;
        if ((file_path == NULL) || (file_path[0] == 0)) {
            return FUNC_BAD_INPUT;
        }
    }

    for (;;) {
        // open the file
        f = fopen (file_path, "w");
        if (f == NULL) {
            ret = FUNC_BAD_INPUT;
            break;
        }

        // save top level values
        ret = save_a_leaf_chain (f, cfg->root.first_leaf);
        if (ret != FUNC_OK) {
            break;
        }

        // save top level sections
        save_a_sect_data sd = {f, NULL};
        ret = save_a_sect_chain (&sd, cfg->root.first_sect, NULL);

        break;
    }

    if (f != NULL) {
        fclose (f);
    }

    return ret;
}


func_error_t aitown_cfg_parse_file ( aitown_cfg_t *cfg, const char *file_path )
{
    // check input
    DBG_ASSERT (cfg != NULL);
    if ((file_path == NULL) || (file_path[0] == 0)) {
        return FUNC_BAD_INPUT;
    }

    // keep a reference
    ++cfg->ref_count;

    FILE * f = NULL;
    size_t fsz;
    func_error_t ret = FUNC_OK;
    char * file_content = NULL;
    unsigned err_line;
    const char * err_string;
    for (;;) {

        // open the file, get its size
        f = fopen (file_path, "r");
        if (f == NULL) {
            fprintf (stderr, "Error opening file %s\n", file_path );
            ret = FUNC_BAD_INPUT;
            break;
        }
        fseek (f, 0, SEEK_END);
        fsz = ftell (f);
        fseek (f, 0, SEEK_SET);

        // allocate a buffer large enough
        file_content = (char*) malloc (fsz);
        if (file_content == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        // and read
        if (fsz != fread (file_content, 1, fsz, f)) {
            fprintf (stderr, "Error reading file %s\n", file_path );
            ret = FUNC_GENERIC_ERROR;
            break;
        }

        // now parse this
        ret = aitown_cfg_parse_buffer (
                    cfg, file_content, &err_line, &err_string);
        if (ret != FUNC_OK) {
            fprintf (stderr, "Error [line %u]: %s\n in %s",
                     err_line, err_string, file_path );
            break;
        }

        break;
    }

    if (f != NULL) {
        fclose (f);
    }
    if (file_content != NULL) {
        free (file_content);
    }

    // release the reference
    --cfg->ref_count;
    return ret;

}

func_error_t enter_subsection (
        aitown_cfg_sect_t * parent_section, const char * name, unsigned name_len,
        aitown_cfg_sect_t ** out)
{
    aitown_cfg_sect_t * s;
    func_error_t ret = FUNC_OK;
    for (;;) {

        // find existing section with this name
        s = (aitown_cfg_sect_t *)get_node_by_bounded_name(
                    (aitown_cfg_node_t*)parent_section->first_sect,
                    name, name+name_len);
        if (s == NULL) {
            ret = aitown_cfg_sect_init (
                        parent_section->node.root, parent_section,
                        name, name_len, &s);
        }
        break;
    }
    *out = s;
    return ret;
}

func_error_t add_empty_leaf (
        aitown_cfg_sect_t * parent_section, const char * name, unsigned name_len,
        aitown_cfg_leaf_t ** out)
{

    aitown_cfg_leaf_t * l;
    func_error_t ret = FUNC_OK;
    for (;;) {

        // find existing section with this name
        l = (aitown_cfg_leaf_t *)get_node_by_bounded_name(
                    (aitown_cfg_node_t*)parent_section->first_leaf,
                    name, name+name_len);
        if (l == NULL) {
            ret = aitown_cfg_leaf_init (
                        parent_section->node.root, parent_section,
                        name, name_len, &l);
        }

        break;
    }
    *out = l;
    return ret;
}

func_error_t add_value_to_leaf (
        aitown_cfg_leaf_t * leaf, const char * name, unsigned name_len)
{

    func_error_t ret = FUNC_OK;
    for (;;) {
        if (leaf->value != NULL) {
            free ((void*)leaf->value);
        }

        char * v = (char*)malloc (name_len+1);
        if (v == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        memcpy (v, name, name_len);
        v[name_len] = 0;

        leaf->value = (const char*)v;

        break;
    }
    return ret;
}

func_error_t append_value_to_leaf (
        aitown_cfg_leaf_t * leaf, const char * name, unsigned name_len)
{
    func_error_t ret = FUNC_OK;
    for (;;) {

        if (leaf->value == NULL) {
            return add_value_to_leaf (leaf, name, name_len);
        }

        size_t crt_l = strlen (leaf->value);
        char * v = (char*)realloc ((void*)leaf->value, crt_l + name_len+1);
        if (v == NULL) {
            ret = FUNC_MEMORY_ERROR;
            break;
        }

        memcpy (v+crt_l, name, name_len);
        v[crt_l+name_len] = 0;

        leaf->value = (const char*)v;

        break;
    }
    return ret;
}


typedef enum _aitown_cfg_parse_status_t {
    CFG_PSTS_IN_LINE_START,
    CFG_PSTS_IN_COMMENT,
    CFG_PSTS_WAIT_SECTION,
    CFG_PSTS_IN_SECTION,
    CFG_PSTS_IN_KEY,
    CFG_PSTS_WAIT_VALUE,
    CFG_PSTS_IN_VALUE,
    CFG_PSTS_IN_LINE_CONTIN
} aitown_cfg_parse_status_t;

func_error_t aitown_cfg_parse_buffer (
        aitown_cfg_t *cfg, const char *file_content,
        unsigned * err_line, const char ** err_string )
{
    // check input
    DBG_ASSERT (cfg != NULL);
    if ((file_content == NULL) || (file_content[0] == 0)) {
        return FUNC_OK;
    }

    // keep a reference
    ++cfg->ref_count;

    // we may want to factor this out as an argument, thus allowing loading
    // a file under a given section
    aitown_cfg_sect_t * root_sect = &cfg->root;


    func_error_t ret = FUNC_OK;
    aitown_cfg_sect_t * crt_sect = root_sect;
    aitown_cfg_leaf_t * crt_leaf = NULL;
    aitown_cfg_parse_status_t sts = CFG_PSTS_IN_LINE_START;
    aitown_cfg_parse_status_t old_sts = CFG_PSTS_IN_LINE_START;
    unsigned line_number = 1;
    char crlf_trick = 0;
    char c;
    char quote_char;
    const char * first_non_white = NULL;
    const char * first_white = NULL;
    int end_loop = 0;
    int continuation_was_last = 0;
    for (;end_loop == 0;) {

        // get next character and advance the buffer
        c = *file_content;
        ++file_content;

        // avoid having a separate switch after this loop by replacing the
        // ending \0 with a new line; the loop will end after this pass
        if (c == 0) {
            end_loop = 1;
            c = '\n';
            crlf_trick = c;
        } else if ((c == '\n') || (c == '\r')) {// count lines

            if ((crlf_trick == 0) || (crlf_trick == c)) {
                line_number++;
            }
            crlf_trick = c;
            //continue;
        } else {
            crlf_trick = 0;
        }

        switch (sts) {
        case CFG_PSTS_IN_LINE_START: {

            // skip white space at the start of the line
            if (c <= ' ')
                continue;

            // detect comments
            if (c == AITOWN_CFG_COMMENT) {
                sts = CFG_PSTS_IN_COMMENT;
                break;
            }

            // detect section start
            if (c == AITOWN_CFG_SECT_S) {
                sts = CFG_PSTS_WAIT_SECTION;
                crt_sect = root_sect;
                break;
            }

            // a value starts here
            sts = CFG_PSTS_IN_KEY;
            first_non_white = file_content - 1;
            break; }
        case CFG_PSTS_IN_COMMENT: {
            // wait for end of line and restart the search
            if (crlf_trick != 0) {
                sts = CFG_PSTS_IN_LINE_START;
            }
            break; }
        case CFG_PSTS_WAIT_SECTION: {
            // a new line at this time is an error
            if (crlf_trick != 0) {
                *err_string = "New line while waiting for section name";
                ret = FUNC_GENERIC_ERROR;
                break;
            }

            // skip white spaces
            if (c <= ' ')
                continue;

            // section end
            if (c == AITOWN_CFG_SECT_E) {
                sts = CFG_PSTS_IN_LINE_START;
                break;
            }

            // we have found the start of the section
            sts = CFG_PSTS_IN_SECTION;
            first_non_white = file_content - 1;
            break; }
        case CFG_PSTS_IN_SECTION: {
            // a new line at this time is an error
            if (crlf_trick != 0) {
                *err_string = "New line while waiting for section name";
                ret = FUNC_GENERIC_ERROR;
                break;
            } else if (c == AITOWN_CFG_SEPARATOR) {// section separator
                sts = CFG_PSTS_WAIT_SECTION;
            } else if (c == AITOWN_CFG_SECT_E) {// section end
                sts = CFG_PSTS_IN_LINE_START;
            } else {
                // character or whitespace in section
                break;
            }

            // make this section current
            if (first_white == NULL)
                first_white = file_content - 1;
            ret = enter_subsection (
                        crt_sect, first_non_white,
                        first_white - first_non_white,
                        &crt_sect);
            first_non_white = NULL;

            break; }
        case CFG_PSTS_IN_KEY: {
            // end of line => empty value
            if (crlf_trick != 0) {
                if (first_white == NULL)
                    first_white = file_content - 1;
                ret = add_empty_leaf (
                            crt_sect, first_non_white,
                            first_white - first_non_white,
                            &crt_leaf);
                sts = CFG_PSTS_IN_LINE_START;
                break;
            }

            // key end and value start
            if ((c == AITOWN_CFG_ASSIGN1) || (c == AITOWN_CFG_ASSIGN2)) {
                if (first_white == NULL)
                    first_white = file_content - 1;
                ret = add_empty_leaf (
                            crt_sect, first_non_white,
                            first_white - first_non_white,
                            &crt_leaf);
                sts = CFG_PSTS_WAIT_VALUE;
                break;
            }

            // wait for the end of the key
            break; }
        case CFG_PSTS_WAIT_VALUE: {
            // end of line => empty value
            if (crlf_trick != 0) {
                if (continuation_was_last) {
                    continuation_was_last = 0;
                    continue;
                }
                crt_leaf = NULL;
                sts = CFG_PSTS_IN_LINE_START;
                break;
            }

            // found value start?
            if (c <= ' ')
                continue;

            if (c == AITOWN_CFG_CONCAT) {
                continue;
            }

            if ((c == '"') || (c == '\'')) {
                first_non_white = file_content;
                quote_char = c;
            } else {
                first_non_white = file_content - 1;
            }
            sts = CFG_PSTS_IN_VALUE;
            break; }
        case CFG_PSTS_IN_LINE_CONTIN:
        case CFG_PSTS_IN_VALUE: {
            old_sts = sts;
            // end of line => end of value
            if (crlf_trick != 0) {
                if (first_white == NULL)
                    first_white = file_content - 1;
                if (continuation_was_last) {
                    sts = CFG_PSTS_IN_LINE_CONTIN;
                    continuation_was_last = 0;
                } else {
                    sts = CFG_PSTS_IN_LINE_START;
                    quote_char = 0;
                }
            } else if (c == quote_char) {
                first_white = file_content - 1;
                sts = CFG_PSTS_IN_LINE_START;
                quote_char = 0;
            } else {
                break;
            }

            // found the end
            if (first_white == NULL)
                first_white = file_content - 1;
            if (old_sts == CFG_PSTS_IN_LINE_CONTIN) {
                append_value_to_leaf (
                            crt_leaf, first_non_white,
                            first_white - first_non_white);
            } else {
                add_value_to_leaf (
                            crt_leaf, first_non_white,
                            first_white - first_non_white);
            }
            break; }
        }

        // did we encountered an error durring last pass?
        if (ret != FUNC_OK) {
            *err_line = line_number;
            break;
        }

        // track firs white space in a sequence, while in that sequence
        if (first_white == NULL) {
            if (c <= ' ') {
                first_white = file_content - 1;
            }
        } else {
            if ((c > ' ') && (c != AITOWN_CFG_CONCAT)) {
                first_white = NULL;
            }
        }

        if (c == AITOWN_CFG_CONCAT) {
            continuation_was_last = 1;
        } else if (c > ' ') {
            continuation_was_last = 0;
        }
    }

    // release the reference
    --cfg->ref_count;
    return ret;

}

unsigned get_string_hash (const char * str)
{
    if (str == NULL)
        return 0;

    // copy the name and compute hash
    unsigned hash_comp[8] = {1,1,1,1,1,1,1,1};
    int i = 0;
    int j;
    char c;
    for (;;) {
        c = *str; str++;
        if (c == 0) break;
        j = i % 8;
        hash_comp[j] += c;
        i++;
    }
    return
            hash_comp[0] * hash_comp[1] * hash_comp[2] * hash_comp[3] *
            hash_comp[4] * hash_comp[5] * hash_comp[6] * hash_comp[7];

}

unsigned get_string_hash_bounded (const char * str, const char * str_end)
{
    if (str == NULL)
        return 0;

    // copy the name and compute hash
    unsigned hash_comp[8] = {1,1,1,1,1,1,1,1};
    int i = 0;
    int j;
    char c;
    for (;str != str_end;) {
        c = *str; str++;
        j = i % 8;
        hash_comp[j] += c;
        i++;
    }
    return
            hash_comp[0] * hash_comp[1] * hash_comp[2] * hash_comp[3] *
            hash_comp[4] * hash_comp[5] * hash_comp[6] * hash_comp[7];

}

aitown_cfg_node_t * get_node_by_bounded_name(
        aitown_cfg_node_t * first, const char * str, const char * str_end)
{

    aitown_cfg_node_t * ret = first;
    unsigned hash = get_string_hash_bounded (str, str_end);
    while (ret != NULL) {
        if (ret->name_hash == hash) {
            int i;
            int i_lim = str_end - str;
            const char *sname = ret->name;
            for(i = 0;i<i_lim;i++) {
                if (str[i] != sname[i]) {
                    break;
                }
            }
            if (i == i_lim) {
                if (sname[i] == 0) {
                    return ret;
                }
            }
        }
        ret = ret->next;
    }
    return NULL;
}

aitown_cfg_leaf_t * aitown_cfg_get (
        aitown_cfg_t *cfg, const char * name)
{
    if (cfg == NULL)
        return NULL;
    if (name == NULL)
        return NULL;
    return aitown_cfg_get_rel (&cfg->root, name);
}


typedef struct _aitown_cfg_get_rel_data {
    const char * iter;
    const char * start;
    const char * end;

} aitown_cfg_get_rel_data;

typedef enum _aitown_cfg_get_rel_stat_t {
    CFGRELSTS_WAIT_NAME,
    CFGRELSTS_IN_NAME


} aitown_cfg_get_rel_stat_t;

aitown_cfg_leaf_t * aitown_cfg_get_rel (
        aitown_cfg_sect_t *sect, const char *name)
{
    if (sect == NULL)
        return NULL;
    if (name == NULL)
        return NULL;

    aitown_cfg_leaf_t * ret = NULL;

    char c;
    const char * first_white = name;
    const char * first_non_white;
    aitown_cfg_sect_t * crt_sect = sect;
    aitown_cfg_get_rel_stat_t sts = CFGRELSTS_WAIT_NAME;
    for (;;) {
        // get next character
        c = *name; name++;

        switch (sts) {
        case CFGRELSTS_WAIT_NAME: {
            if (c == 0) {
                return NULL;
            }
            if (c > ' ') {
                if (c != AITOWN_CFG_SEPARATOR) {
                    first_non_white = name-1;
                    sts = CFGRELSTS_IN_NAME;
                }
            }

            break; }
        case CFGRELSTS_IN_NAME: {
            if (c == 0) {
                if (first_white == NULL) first_white = name-1;
                return (aitown_cfg_leaf_t *)get_node_by_bounded_name(
                            (aitown_cfg_node_t*)crt_sect->first_leaf,
                            first_non_white,
                            first_white
                            );
            } else if (c == AITOWN_CFG_SEPARATOR) {
                if (first_white == NULL) first_white = name-1;
                crt_sect = (aitown_cfg_sect_t *)get_node_by_bounded_name(
                            (aitown_cfg_node_t*)crt_sect->first_sect,
                            first_non_white,
                            first_white
                            );
                if (crt_sect == NULL) {
                    return NULL;
                }
                sts = CFGRELSTS_WAIT_NAME;
            }

            break; }
        }

        if (c <= ' ') {
            if (first_white == NULL) {
                first_white = name-1;
            }
        } else {
            first_white = NULL;
        }
    }

    return ret;
}

aitown_cfg_sect_t * aitown_cfg_get_sect (
        aitown_cfg_sect_t * section, const char * name )
{
    if (section == NULL)
        return NULL;
    if (name == NULL)
        return section;

    aitown_cfg_sect_t * ret = section->first_sect;
    unsigned hash = get_string_hash (name);
    while (ret != NULL) {
        if (ret->node.name_hash == hash) {
            if (strcmp (name, ret->node.name) == 0) {
                return ret;
            }
        }
        ret = (aitown_cfg_sect_t *)ret->node.next;
    }

    return ret;
}

aitown_cfg_leaf_t * aitown_cfg_get_leaf (
        aitown_cfg_sect_t * section, const char * name)
{
    if (section == NULL)
        return NULL;
    if (name == NULL)
        return NULL;

    aitown_cfg_leaf_t * ret = section->first_leaf;
    unsigned hash = get_string_hash (name);
    while (ret != NULL) {
        if (ret->node.name_hash == hash) {
            if (strcmp (name, ret->node.name) == 0) {
                return ret;
            }
        }
        ret = (aitown_cfg_leaf_t *)ret->node.next;
    }

    return NULL;
}

aitown_cfg_sect_t * aitown_cfg_get_or_create_sect (
        aitown_cfg_sect_t * section, const char * name )
{
    aitown_cfg_sect_t * ret = aitown_cfg_get_sect (section, name);
    if (ret == NULL) {
        if (FUNC_OK != aitown_cfg_sect_init (
                    section->node.root, section,
                    name, strlen (name), &ret)) {
            ret = NULL;
        }
    }
    return ret;
}

aitown_cfg_leaf_t * aitown_cfg_get_or_create_leaf (
        aitown_cfg_sect_t * sect, const char * name )
{
    aitown_cfg_leaf_t * ret = aitown_cfg_get_leaf (sect, name);
    if (ret == NULL) {
        if (FUNC_OK != aitown_cfg_leaf_init (
                    sect->node.root, sect,
                    name, strlen (name), &ret)) {
            ret = NULL;
        }
    }
    return ret;
}

func_error_t aitown_cfg_set_leaf (
        aitown_cfg_leaf_t * leaf, const char * value)
{
    if (leaf == NULL) {
        return FUNC_BAD_INPUT;
    }
    if (leaf->value != NULL) {
        free ((void*)leaf->value);
    }
    if (value == NULL) {
        leaf->value = NULL;
    } else {
        leaf->value = strdup (value);
        if (leaf->value == NULL) {
            return FUNC_MEMORY_ERROR;
        }
    }
    return FUNC_OK;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


