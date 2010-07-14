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



struct stat * GDDI_getattr (const char * path)
{
    struct stat * mystat = NULL;
    buffer_t * mem = NULL;
    json_t * root = NULL;
    json_error_t json_error;
    json_t *value = NULL;
    void *iter = NULL;
    void *iter2 = NULL;

    const char *key2 = NULL;
    json_t *value2 = NULL;
    const char * myname = NULL;
    const char * searchpath = NULL;
    const char * path_base = NULL;
    char * dir_entry_name = NULL;
    
    searchpath = dirname(path);
    /* searchpath = path; */
    path_base = basename(path);

    mystat = calloc (1, sizeof (struct stat));
    if (!mystat)
    {
        fprintf (stderr, "Error: couldn't allocate state object\n");
        return NULL;
    }

    
    printf ("%s ------------------- Fetching: %s%s on entry %s\n", __func__, getGridFSURL(), searchpath, path_base);
    /* Query root */
    if ((strlen(searchpath) == 1) && (searchpath[0] == '/'))
    {    
        mem = download (getGridFSURL(), NULL, 0);
    }
    else
    {
        mem = download (getGridFSURL(), searchpath, 1);
    }
    printf ("%s ------------------- Fetched:  %s%s on entry %s  path = %s\n", __func__, getGridFSURL(), searchpath, path_base, path);
    
    /* Check on Django error */
    if (strstr(mem -> data, "NameError at"))
    {
        fprintf(stderr, "error: path \"%s\" doesn't exist\n", path);
        return NULL;
    }


    root = json_loads (mem -> data, &json_error);
    free (mem -> data);
    free (mem);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", json_error.line, json_error.text);
        return NULL;
    }

    if( !json_is_object( root ) )
    {
        json_decref( root );
        fprintf(stderr, "error: root is not an object\n");
        return NULL;
    }
    else
    {
        /* obj is a JSON object */
        iter = json_object_iter(root);
        while(iter)
        {
            dir_entry_name = json_object_iter_key(iter);
            value = json_object_iter_value(iter);

#ifdef DEBUG
            printf ("Key is: %s\n", dir_entry_name);
#endif
            if (strcmp(dir_entry_name, path_base) == 0)
            {
                if (json_is_object (value))
                {
                    iter2 = json_object_iter(value);
                    while (iter2)
                    {

                        key2 = json_object_iter_key(iter2);
                        value2 = json_object_iter_value(iter2);

                        if (strcmp(key2, "type") == 0)
                        {
                            if(json_is_string(value2))
                            {
                                myname = json_string_value(value2);

                                if (strcmp (myname, "dir") == 0)
                                {
                                    mystat->st_mode = S_IFDIR | 0755;
                                    mystat->st_nlink = 2;
                                    mystat->st_uid = getuid();
                                    mystat->st_gid = getgid();
                                    mystat->st_size = DEFAULT_DIR_SIZE;
                                }
                                else if (strcmp(myname, "file") == 0)
                                {
                                    mystat->st_mode = S_IFREG | 0444;
                                    mystat->st_nlink = 1;
                                    mystat->st_uid = getuid();
                                    mystat->st_gid = getgid();
                                }
                            }
                        }
                        else if (strcmp(key2, "size") == 0)
                        {
                            if(json_is_integer(value2))
                            {
                                mystat->st_size = json_integer_value(value2);
                            }
                        }

                        iter2 = json_object_iter_next(value, iter2);
                    }
                    return mystat;
                }
            }
            iter = json_object_iter_next(root, iter);
        }
    }
    /* Ref-counter lowering */
    json_decref(root);

    return NULL;
}



int grid_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    struct stat * mystat = NULL;


    printf ("Current path for getattr is: %s\n", path);

    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(strncmp((char *)basename(path), "/._", 3) == 0) {
#ifdef DEBUG
        printf ("Source fork file\n");
#endif
        res = -ENOENT;
    }
    else if(strncmp((char *)basename(path), "._", 2) == 0) {
#ifdef DEBUG
        printf ("Source fork file\n");
#endif
        res = -ENOENT;
    }
    else
    {
        /* Query for information */
#ifdef DEBUG
        printf ("Querying info for: %s\n", path);
#endif

        mystat = GDDI_getattr (path);
        if (mystat)
        {
            stbuf->st_mode  = mystat->st_mode;
            stbuf->st_nlink = mystat->st_nlink;
            stbuf->st_uid   = getuid();
            stbuf->st_gid   = getgid();
            stbuf->st_size  = mystat->st_size;

            free(mystat);
        }
        else
        {
            res = -ENOENT;
        }
    }

    return res;
}

