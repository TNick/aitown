/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aitown-dstorage.c
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

#include "aitown-dstorage.h"

#include <stdlib.h>
#include <string.h>
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/utils_unused.h>
#include <aitown/cfgpath.h>
#include <aitown/ini.h>
#include <aitown/pivotal_gmtime.h>

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

static void dstorage_init_default (dstorage_t *dstorage, const char *cfg_file) {

    // get a database name to use
    char dbf_file[MAX_PATH];
    int len_name = strlen ("dstorage.kch"); // too specific for kyoto db

    get_user_data_folder(dbf_file, MAX_PATH-len_name-2, dstorage->p_name);
    if (dbf_file[0]==0) {
        dbg_message ("dstorage default database name error %s", dstorage->p_name);
    } else {
        strcat (dbf_file, "/dstorage.kch");
        dstorage->p_db_file = strdup (dbf_file);
    }

    // create a new file
    FILE* f_cfg_file = fopen (cfg_file,"w");
    if (f_cfg_file != NULL) {

        fprintf (f_cfg_file, "[database]\n");
        fprintf (f_cfg_file, "file=%s\n", dbf_file);
        fprintf (f_cfg_file, "[chunks]\n"); // 67108864 = 1024*1024*64
        fprintf (f_cfg_file, "limit=67108864\n");

        // close the file
        fclose (f_cfg_file);
    } else {
        dbg_message ("dstorage config file can't be created at %s", cfg_file);
    }
}

static void dstorage_read_config (dstorage_t *dstorage, const char *cfg_file)
{
    dbg_message ("Reading dstorage config file %s", cfg_file);
    FILE* f_cfg_file = fopen (cfg_file,"r");
    if (f_cfg_file) {
        int i_parse = struct_ini_parse_file (&dstorage->sini, f_cfg_file);
        switch (i_parse) {
        case INIH_ERR_SUCCESS: {
            dbg_message ("dstorage config file %s ok", cfg_file);

            dstorage->p_db_file = strdup (struct_ini_get_value(&dstorage->sini,"database", "file"));
            dbg_message ("Database file is %s", dstorage->p_db_file);

            break; }
        case INIH_ERR_FILE_IO: {
            err_message ("File error while reading dstorage config file for %s", cfg_file);
            break; }
        case INIH_ERR_MALLOC: {
            err_message ("Memory allocation error while reading dstorage config file for %s", cfg_file);
            break; }
        default: {
            err_message ("Error parsing dstorage config file %s[%d]", cfg_file, i_parse);
            break; }
        } /* switch (i_parse) */

        fclose (f_cfg_file);
        f_cfg_file = NULL;
    } else {
        dbg_message ("dstorage config file %s does not exist", cfg_file);
        dstorage_init_default (dstorage, cfg_file);
    }
}

void dstorage_init (
        dstorage_t *dstorage, const char *p_name, const char *p_ini_file)
{
    // prepare the list of callbacks for controllers (ok more than once)
    dstorage_controllers_init();

    // clear the structure
    memset (dstorage, 0, sizeof(dstorage_t));

    // prepare settings structure
    struct_ini_init (&dstorage->sini);

    if (p_name == NULL) {
        log_message ("dstorage_init without a name; probably an error.");
    } else {
        // make a copy of the name and store it for our reference
        dstorage->p_name = strdup (p_name);
    }/* if ( p_name == NULL )  */

    // location of the configuration file
    char cfg_file[MAX_PATH];
    if ((p_ini_file == NULL) && (p_name != NULL)) {
        get_user_config_file(cfg_file,MAX_PATH-1,p_name);
        if (cfg_file[0] == 0) {
            err_message ("Failed to get dstorage config file for %s", p_name);
        }
        p_ini_file = cfg_file;
    }

    // read config file
    if (p_ini_file != NULL) {
        dstorage_read_config (dstorage, p_ini_file);
    }

    // prepare the database for ids
    if (dstorage->p_db_file != NULL) {
        dstorage_lookup_init_open (&dstorage->lku, dstorage->p_db_file);
    } else {
        dstorage_lookup_init (&dstorage->lku);
    }

    // start-up handle manager
    dstorage_handle_mng_init (&dstorage->hmng);

    // prepare memory allocator for chunks
    dstorage_chunk_mng_init (&dstorage->ckmng);
    long int mem_limit;
    struct_ini_status_t mem_limit_sts;
    mem_limit_sts = struct_ini_get_int (&dstorage->sini, "chunks", "limit", &mem_limit, 1024*1024*16);
    if ((mem_limit_sts == STRUCT_INI_CANT_CONVERT) || (mem_limit < 0)) {
        err_message ("The value for chunk memory limit must be a positive integer");
    }
    dbg_message ("chunk memory limit set to %u", (unsigned)mem_limit);
    dstorage_chunk_mng_set_limit (&dstorage->ckmng, (size_t)mem_limit);

    // start-up the list of controllers
    dstorage_clist_init (dstorage);
}

void dstorage_end (dstorage_t *dstorage)
{

    // terminate components
    dstorage_lookup_end (&dstorage->lku);
    dstorage_clist_end (&dstorage->clist);
    struct_ini_end (&dstorage->sini);
    dstorage_handle_mng_end (&dstorage->hmng);
    dstorage_chunk_mng_end (&dstorage->ckmng);

    // free our copy of the name
    if ( dstorage->p_name != NULL ) {
        free (dstorage->p_name);
    }

    // free our copy of the db file
    if ( dstorage->p_db_file != NULL ) {
        free (dstorage->p_db_file);
    }

    // clear the structure
    memset (dstorage, 0, sizeof(dstorage_t));

}

dstorage_handle_t * dstorage_handle (dstorage_t *dstorage, dstorage_id_t id, void *owner)
{
    // make sure is not the reserved value
    if (id == 0) {
        err_message ("dstorage_handle: 0 is not a valid ID");
        return NULL;
    }

    // check to see if we already have this id loaded
    dstorage_handle_t * ret = dstorage_lookup_handle (&dstorage->lku, id);

    // create a new handle for it if we don't
    if (ret == NULL) {
        ret = dstorage_handle_init (&dstorage->hmng, id);
        DBG_ASSERT (dstorage_handle_ref_count (ret) == 0);
        dstorage_lookup_add (&dstorage->lku, ret);
    }
    dstorage_handle_inc_ref (ret, owner);
    ret->tstamp = z_clock();

    // and that's it
    return ret;
}

static void ctrl_kb_free_common(dstorage_t *dstorage, dstorage_handle_t *handle)
{

    /** @todo add it to a list of free handles */
    /** @todo take memory scarcity into account */

    // free memory chunk
    if (handle->p_data != NULL) {
        dstorage_chunk_mng_free (&dstorage->ckmng, &handle->p_data);
    }

    // remove from btree and free memory
    dstorage_lookup_rem (&dstorage->lku, handle);
    dstorage_handle_end (&dstorage->hmng, &handle);
}

static void ctrl_kb_free (
        dstorage_ctrl_sts_t sts, struct _dstorage_ctrl_param_t* req)
{
    if (sts == DSTORAGE_CTRL_OK) {
        ctrl_kb_free_common (req->ctrl->dstorage, req->handle);
    } else {
        /** @todo attempt to save in a different controller; if succesfull
            update the id in the database */

    }
}


void dstorage_handle_save (
        dstorage_t *dstorage, dstorage_handle_t *handle,
        dstorage_ctrl_response kb)
{
    dstorage_ctrl_param_t   param;
    param.handle = handle;
    param.kb = kb;

    // querry the database for controller specific data and controller's index
    param.ctrl_data = dstorage_lookup_cdata (&dstorage->lku, handle);
    if (param.ctrl_data == NULL) {
        // the index
        /** @todo register this id */
        dbg_message ("resolve failed to find id %u in database", (unsigned)handle->id);
        return;
    }

    // get the controller;
    unsigned ctrl_index = dstorage_cdata_ctrl (param.ctrl_data);
    param.ctrl = dstorage_clist_get (
                &dstorage->clist,
                ctrl_index);
    if (param.ctrl == NULL) {
        /** @todo try other controllers? */

        dbg_message ("controller %u for id %u is missing", ctrl_index, (unsigned)handle->id);
        return;
    }

    // update time stamp
    handle->tstamp = z_clock();

    // finaly, ask the controller to do hard part
    param.ctrl->write (&param);
}

void dstorage_handle_done (dstorage_t *dstorage, dstorage_handle_t **handle, void *owner)
{
    // make sure this is valid input
    dstorage_handle_t *h = *handle;
    if (h == NULL ) {
        return;
    }

    // release the reference
    DBG_ASSERT (dstorage_lookup_id (&dstorage->lku, h));
    dstorage_handle_dec_ref (h, owner);

    // see if it is not used anymore
    if (dstorage_handle_ref_count (h) == 0) {

        // check if is dirty; ask the controller to write it
        if (dstorage_handle_is_dirty (h)) {
            dstorage_handle_set_status(h,DSTORAGE_H_WITH_DATA);
            dstorage_handle_save (dstorage, h, ctrl_kb_free);
        } else {
            ctrl_kb_free_common (dstorage, h);
        }
    }
    *handle = NULL;
}

void dstorage_handle_resolve (
        dstorage_t *dstorage, dstorage_handle_t *handle,
        void * user, dstorage_ctrl_response kb)
{
    dstorage_ctrl_param_t   param;
    param.handle = handle;
    param.kb = kb;

    // check the status
    dstorage_sts_t sts = (dstorage_sts_t)dstorage_handle_gen_status(handle);
    if (!((sts == DSTORAGE_H_NO_DATA) ||
         (sts == DSTORAGE_H_TEMP_FAILURE) ||
         (sts == DSTORAGE_H_LOST))) {

        /* should we call the callback, anyway? */
        dbg_message ("handle state not proper for resolving");
        return;
    }

    // querry the database for controller specific data and controller's index
    param.ctrl_data = dstorage_lookup_cdata (&dstorage->lku, handle);
    if (param.ctrl_data == NULL) {
        dbg_message ("resolve failed to find id %u in database", (unsigned)handle->id);
        return;
    }

    // get the controller; again, if no controller, nothing to do
    unsigned ctrl_index = dstorage_cdata_ctrl (param.ctrl_data);
    param.ctrl = dstorage_clist_get (
                &dstorage->clist,
                ctrl_index);
    if (param.ctrl == NULL) {
        dstorage_handle_mark_temp_errors (handle);
        dbg_message ("controller %u for id %u is missing", ctrl_index, (unsigned)handle->id);
        return;
    }

    // update time stamp
    handle->tstamp = z_clock();

    // finaly, ask the controller to do hard part
    param.ctrl->read (&param);
}

dstorage_handle_t * dstorage_new (dstorage_t *dstorage, size_t sz)
{

    // get the controller to use
    dstorage_ctrl_t * ctrl = dstorage_clist_get_best (&dstorage->clist);

    // allocate a new id; should not have been used before
    dstorage_id_t id = dstorage_lookup_new_id (&dstorage->lku, ctrl);
    if (id == 0) {
        return NULL;
    }
    DBG_ASSERT (dstorage_lookup_handle (&dstorage->lku, id) == NULL);

    // create a new handle
    dstorage_handle_t *ret = dstorage_handle_init (&dstorage->hmng, id);
    if (ret == NULL) {
        /** @todo leaking an ID, here; maybe undo previous op */
        return NULL;
    }
    DBG_ASSERT (dstorage_handle_ref_count (ret) == 0);

    // allocate a chunk and associate it with the handle
    ret->p_data = dstorage_alloc_chunk (dstorage, sz);
    if (ret->p_data == NULL) {
        /** @todo leaking an ID, here; maybe undo previous op */
        dstorage_handle_end (&dstorage->hmng, &ret);
        return NULL;
    }
    dstorage_handle_mark_resolved (ret);

    // add the handle in the chain and take a reference
    dstorage_lookup_add (&dstorage->lku, ret);
    dstorage_handle_inc_ref (ret, ret);

    // mark it as dirty so that it gets written when released
    dstorage_handle_mark_dirty (ret);

    // update time stamp
    ret->tstamp = z_clock();

    return ret;
}

dstorage_chunk_t* dstorage_alloc_chunk (dstorage_t *dstorage, size_t sz)
{
    dstorage_chunk_t* ret;

    // give it a try
    ret = dstorage_chunk_mng_alloc (&dstorage->ckmng, sz);
    if (ret != NULL) {
        return ret;
    }

    /// @todo try harder; look into freed handles; mark the system as being
    /// stressed from lack of memory (handles are freed directly)

    return NULL;
}


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


