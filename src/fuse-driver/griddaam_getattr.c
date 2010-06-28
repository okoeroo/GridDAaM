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

#include "net.h"
/* #include "curl-getinmemory.h" */

#include "griddaam_driver_func.h"



struct stat * GDDI_getattr (const char * path)
{
    struct stat * mystat = NULL;
    buffer_t * mem = NULL;
    json_t * root = NULL;
    json_error_t json_error;
    json_t * commits = NULL;
    int i = 0;
    
    char * searchpath = NULL;

    searchpath = malloc (sizeof (char) * (strlen(path) + strlen(getGridFSURL()) + 2));
    strcpy (searchpath, getGridFSURL());
    strcat (searchpath, path);

    mem = download ((char *) searchpath);
    if (mem)
    {
        /* Create stat struct */
        mystat = calloc (1, sizeof (struct stat));
        if (mystat)
        {
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
                json_t * dir;


                /* obj is a JSON object */
                const char *key;
                json_t *value;
                void *iter = json_object_iter(root);
                while(iter)
                {
                    json_t * list = NULL;

                    key = json_object_iter_key(iter);
                    value = json_object_iter_value(iter);

                    /* use key and value ... */
                    if (json_is_array (value))
                    {
                        for(i = 0; i < json_array_size(value); i++)
                        {
                            json_t * file = NULL;
                            json_t * myname = NULL;
                            const char *message_text;

                            file = json_array_get(value, i);
                            if(!json_is_string(file))
                            {
                                fprintf(stderr, "error: file %d: message is not a string\n", i + 1);
                                return NULL;
                            }
                            myname = json_string_value(file);

                            if (strcmp (myname, basename (path)) == 0)
                            {
                                printf ("Found thingy!\n");

                                if (strcmp(key, "dir") == 0)
                                {
                                    mystat->st_mode = S_IFDIR | 0755;
                                    mystat->st_nlink = 2;
                                    break;
                                }
                                else if (strcmp(key, "file") == 0)
                                {
                                    mystat->st_mode = S_IFREG | 0444;
                                    mystat->st_nlink = 1;
                                    mystat->st_uid = getuid();
                                    mystat->st_gid = getgid();
                                    break;
                                }
                            }
                            else
                                printf ("Not found thingy!\n");
                        }

                    }
                    else
                        printf ("Something completely different\n");
                    
                    iter = json_object_iter_next(root, iter);
                }
            }
            /* Ref-counter lowering */
            json_decref(root);
        }
    }

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
    else if(strncmp(basename(path), "._", 2) == 0) {
        res = -ENOENT;
    }
    else if(strcmp(path, basename(debug_file)) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
    }
    else
    {
        /* Query for information */
        mystat = GDDI_getattr (path);
        if (mystat)
        {
            stbuf->st_mode  = mystat->st_mode;
            stbuf->st_nlink = mystat->st_nlink;
            stbuf->st_uid   = getuid();
            stbuf->st_gid   = getgid();
        }
        else
        {
            res = -ENOENT;
        }
    }
        /* res = -ENOENT; */

    return res;
}

