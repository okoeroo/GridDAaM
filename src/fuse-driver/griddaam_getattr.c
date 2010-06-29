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

#include "common.h"


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

    printf ("%s: DEBUG: path %s\n", __FILE__, path);
    printf ("%s: DEBUG: basename %s\n", __FILE__, basename(path));

    if (strncmp(basename(path), "._", 2) == 0)
    {
        return NULL;
    }
    else if (strncmp(basename(path), "/._", 3) == 0)
    {
        return NULL;
    }

    printf ("creating stat object\n");
    mystat = malloc (sizeof (struct stat));
    if (mystat)
    {
        printf ("Got it!\n");
    }

    myurl = getGridFSURL();

    printf ("bar %d\n", strlen(getGridFSURL()));

    printf ("URL: getGridFSURL() %s (%d)\n", getGridFSURL(), strlen(getGridFSURL()));
    searchpath = malloc (sizeof (char) * (strlen(path) + strlen(getGridFSURL()) + 2));
    strcpy (searchpath, getGridFSURL());
    strcat (searchpath, path);


    printf ("------------------- Fetching: %s\n", searchpath);
    mem = download ((char *) searchpath);
    printf ("------------------- Fetched : %s\n", searchpath);

    /* printf ("%s\n", mem -> data); */

    root = json_loads (mem -> data, &json_error);
    free (mem -> data);
    free (mem);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", json_error.line, json_error.text);
        return 1;
    }

    if( !json_is_object( root ) )
    {
        json_decref( root );
        fprintf(stderr, "error: root is not an object\n");
        return 1;
    }
    else
    {
        /* obj is a JSON object */
        char * dir_entry_name = NULL;
        json_t *value;
        void *iter = json_object_iter(root);
        while(iter)
        {
            json_t *value;

            dir_entry_name = json_object_iter_key(iter);
            value = json_object_iter_value(iter);

            printf ("Key is: %s\n", dir_entry_name);
           
            if (json_is_object (value))
            {
                void *iter2;

                printf ("object\n");

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
                            printf ("Yes, value 2 is a string\n");
                            myname = json_string_value(value2);

                            if (strcmp (myname, "dir") == 0)
                            {
                                mystat->st_mode = S_IFDIR | 0755;
                                mystat->st_nlink = 2;
                                break;
                            }
                            else if (strcmp(myname, "file") == 0)
                            {
                                mystat->st_mode = S_IFREG | 0444;
                                mystat->st_nlink = 1;
                                mystat->st_uid = getuid();
                                mystat->st_gid = getgid();
                                break;
                            }
                        }
                    }

                    /* break; */

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
#if 0
    {
        res = -ENOENT;
    }    
#else
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
        }
        else
        {
            res = -ENOENT;
        }
    }
#endif

    return res;
}

