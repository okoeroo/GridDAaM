#include <fuse.h>
#include <fuse/fuse.h>
#include <fuse_opt.h>
#include <fuse/fuse_opt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <jansson.h>
#include <libgen.h>

#include "common.h"
#include "griddaam_driver_func.h"



int grid_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    buffer_t * mem = NULL;
    json_t * root = NULL;
    json_error_t json_error;


    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    printf ("%s ------------------- Fetching: %s%s/\n", __func__, getGridFSURL(), path);
    if (path[strlen(path) - 1] == '/')
        mem = download (getGridFSURL(), path, 0);
    else
        mem = download (getGridFSURL(), path, 1);
    printf ("%s ------------------- Fetched:  %s%s/\n", __func__, getGridFSURL(), path);

    /* If no data is provided (yet) */
    if (!mem)
    {
        return 0;
    }

    root = json_loads (mem -> data, &json_error);
    free (mem -> data);
    free (mem);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", json_error.line, json_error.text);
        return 1;
    }

    if(!json_is_object(root))
    {
        json_decref( root );
        fprintf(stderr, "error: root is not an object\n");
        return 1;
    }
    else
    {
        /* obj is a JSON object */
        char * dir_entry_name = NULL;
        void *iter = json_object_iter(root);
        while(iter)
        {
            dir_entry_name = json_object_iter_key(iter);

            if( dir_entry_name[strlen(dir_entry_name) - 1] == '/' )
                dir_entry_name[strlen(dir_entry_name) - 1] = '\0';

            printf ("Key is: %s\n", dir_entry_name);
            filler(buf, dir_entry_name, NULL, 0);

            iter = json_object_iter_next(root, iter);
        }
    }
    /* Ref-counter lowering */
    json_decref(root);

    return 0;
}

