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
    json_t * commits = NULL;
    int i = 0;
    char * myurl = NULL;

    char * searchpath = NULL;
    char * dir_entry_name = NULL;
    
    searchpath = dirname(path);
    /* searchpath = path; */
    dir_entry_name = basename(path);

    mystat = malloc (sizeof (struct stat));
    if (!mystat)
    {
        fprintf (stderr, "Error: couldn't allocate state object\n");
        return NULL;
    }

    
    printf ("%s ------------------- Fetching: %s%s on entry %s\n", __func__, getGridFSURL(), searchpath, dir_entry_name);
    /* Query root */
    if ((strlen(searchpath) == 1) && (searchpath[0] == '/'))
    {    
        mem = download (getGridFSURL(), NULL, 0);
    }
    else
    {
        mem = download (getGridFSURL(), searchpath, 1);
    }
    printf ("%s ------------------- Fetched:  %s%s on entry %s  path = %s\n", __func__, getGridFSURL(), searchpath, dir_entry_name, path);
    printf ("%s\n", path);
    
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
        char * dir_entry_name = NULL;
        json_t *value;
        void *iter = json_object_iter(root);
        while(iter)
        {
            dir_entry_name = json_object_iter_key(iter);
            value = json_object_iter_value(iter);

            printf ("Key is: %s\n", dir_entry_name);
           
            if (json_is_object (value))
            {
                void *iter2;

                iter2 = json_object_iter(value);
                while (iter2)
                {
                    const char *key2;
                    json_t *value2;
                    char * myname = NULL;

                    key2 = json_object_iter_key(iter2);
                    value2 = json_object_iter_value(iter2);

                    printf ("Key2 is: %s\n", key2);
                    if (strcmp(key2, "type") == 0)
                    {
                        if(json_is_string(value2))
                        {
                            myname = json_string_value(value2);

                            if (strcmp (myname, "dir") == 0)
                            {
                                mystat->st_mode = S_IFDIR | 0755;
                                mystat->st_nlink = 2;

                                printf ("%s(%s) is a dir, with 0755\n", path, dir_entry_name);

                                return mystat;
                            }
                            else if (strcmp(myname, "file") == 0)
                            {
                                mystat->st_mode = S_IFREG | 0444;
                                mystat->st_nlink = 1;
                                mystat->st_uid = getuid();
                                mystat->st_gid = getgid();

                                printf ("%s(%s) is a file, with 0444\n", path, dir_entry_name);

                                return mystat;
                            }
                        }
                    }

                    iter2 = json_object_iter_next(value, iter2);
                }
            }
            iter = json_object_iter_next(root, iter);
        }
    }
    /* Ref-counter lowering */
    json_decref(root);

    return mystat;
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
    else if(strncmp(basename(path), "/._", 3) == 0) {
        printf ("Source fork file\n");
        res = -ENOENT;
    }
    else if(strncmp(basename(path), "._", 2) == 0) {
        printf ("Source fork file\n");
        res = -ENOENT;
    }
    else
    {
        /* Query for information */
        printf ("Querying info for: %s\n", path);

        mystat = GDDI_getattr (path);
        if (mystat)
        {
            stbuf->st_mode  = mystat->st_mode;
            stbuf->st_nlink = mystat->st_nlink;
            stbuf->st_uid   = getuid();
            stbuf->st_gid   = getgid();

            free(mystat);
        }
        else
        {
            res = -ENOENT;
        }
    }

    return res;
}

