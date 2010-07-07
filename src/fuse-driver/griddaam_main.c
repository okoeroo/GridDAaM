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


/* static const char *grid_str = "Hello World!\n"; */
/* static const char *grid_path = "/grid"; */
/* const char * debug_file = "debug"; */
const char *url = "http://asen.nikhef.nl:8000";
static const char *grid_str = "Hello World!\n";


static int grid_opt_proc( void *data, const char *arg, int key, struct fuse_args *outargs );


static int grid_opendir(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

static int grid_open(const char *path, struct fuse_file_info *fi)
{
#if 0
    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;
#endif

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
    .mkdir      = grid_mkdir,
    .getattr    = grid_getattr,
    .readdir    = grid_readdir,
    .open       = grid_open,
    .opendir    = grid_opendir,
    .read       = grid_read
};


static int grid_opt_proc( void *data, const char *arg, int key, struct fuse_args *outargs )
{
    (void)data;
    (void)outargs;
    switch( key )
    {
        case FUSE_OPT_KEY_NONOPT:
            if(getGridFSURL() == NULL )
            {
                setGridFSURL(arg);
                return 0;
            }
            return 1;
        default:
            return 1;
    }
}



int main(int argc, char *argv[])
{
    int ret = 0;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    if( fuse_opt_parse(&args, NULL, NULL, grid_opt_proc) == -1 )
        return -1;

    if(getGridFSURL() == NULL )
    {
        fprintf (stderr, "No Grid URI to mount\n");
        return 1;
    }

    printf ("Contact URL is: %s\n", getGridFSURL());
    ret = fuse_main(args.argc, args.argv, &grid_oper, NULL);

    if (ret)
        printf("\n");

    fuse_opt_free_args(&args);

    return ret;
}

