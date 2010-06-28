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
#include "common.h"
#include "griddaam_driver_func.h"


static const char *grid_str = "Hello World!\n";
static const char *grid_path = "/grid";
const char *url = "http://asen.nikhef.nl:8000";


const char * debug_file = "debug";


static int grid_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    buffer_t * mem = NULL;
    json_t * root = NULL;
    json_error_t json_error;
    json_t * commits = NULL;
    int i = 0;
    
    (void) offset;
    (void) fi;


    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, debug_file, NULL, 0);

    mem = download (getGridFSURL());

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
                        return 1;
                    }
                    myname = json_string_value(file);
                    filler(buf, myname, NULL, 0);
                }

            }
            else
                printf ("Something completely different\n");
            
            iter = json_object_iter_next(root, iter);
        }
    }
    /* Ref-counter lowering */
    json_decref(root);

    return 0;
}

static int grid_open(const char *path, struct fuse_file_info *fi)
{
    if(strcmp(path, grid_path) != 0)
        return -ENOENT;

    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int grid_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;

#if 0
    if (strcmp(basename(path), debug_file))
    {
        len = strlen(debug_output);
        if (offset < len)
        {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, grid_str + offset, size);
        }
        else
            size = 0;

        return 0
    }
#endif

    if(strcmp(path, grid_path) != 0)
        return -ENOENT;

    len = strlen(grid_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, grid_str + offset, size);
    } else
        size = 0;

    return size;
}

/* const struct fuse_operations  *      op */
static const struct fuse_operations grid_oper = {
    .getattr    = grid_getattr,
    .readdir    = grid_readdir,
    .open       = grid_open,
    .read       = grid_read
};


int main(int argc, char *argv[])
{
    setGridFSURL(url);

    /* return fuse_main(argc, argv, &grid_oper); */
    return fuse_main(argc, argv, &grid_oper, NULL);
}

