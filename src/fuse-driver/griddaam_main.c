#include <fuse.h>
#include <fuse_opt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

#include <jansson.h>
#include <libgen.h>

#include "common.h"
#include "griddaam_driver_func.h"

#include "mylog.h"




struct helper_opts
{
    int foreground;
    int singlethread;
    char * mountpoint;
} helper_opts;

#define FUSE_HELPER_OPT(t, p) { t, offsetof(struct helper_opts, p), 1 }

static const struct fuse_opt fuse_helper_opts[] = {
    FUSE_HELPER_OPT("-f",          foreground),
    FUSE_HELPER_OPT("-s",          singlethread),
    FUSE_OPT_END
};


static int grid_opt_proc( void *data, const char *arg, int key, struct fuse_args *outargs );


static int grid_opendir(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

static int grid_open(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

static int grid_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    return 0;
}


/* const struct fuse_operations  *      op */
static const struct fuse_operations grid_oper = {
    .getattr    = grid_getattr,
    .mkdir      = grid_mkdir,
    .create     = grid_create,
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
            /* Search for the URI to mount */
            if (strncmp(arg, "http", 4) == 0)
            {
                if(getGridFSURL() == NULL )
                {
                    setGridFSURL(arg);
                    return 0;
                }
            }
            return 1;
        default:
            return 1;
    }
}



int main(int argc, char *argv[])
{
    int i = 0, ret = 0;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    char * mount_point = "test/";

    mkdir("/tmp/griddaam_scratch", 0755);
    setScratchDir ("/tmp/griddaam_scratch");

    chdir ("/tmp/griddaam_scratch");



    ret = system ("uricopy.sh -force -mkdirs srm://tbn18.nikhef.nl:8446/dpm/nikhef.nl/home/dteam/okoeroo/fuse/testfile file:///tmp/griddaam_scratch/foobar");
    printf ("Return value is: %d\n", ret);

    return 0;

#if 0
    for (i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "http", 4) == 0)
        {
            if(getGridFSURL() == NULL )
            {
                setGridFSURL(argv[i]);
            }
        }
    }
#endif

    if( fuse_opt_parse(&args, &helper_opts, fuse_helper_opts, grid_opt_proc) == -1 )
        return -1;


    if(getGridFSURL() == NULL )
    {
        printf ("No Grid URI to mount\n");
        /* return 1; */
    }

    ret = fuse_daemonize(1);
    printf ("Ret was: %d\n", ret);

    printf ("Contact URL is: %s\n", getGridFSURL());
    ret = fuse_main(args.argc, args.argv, &grid_oper, NULL);
    /* ret = fuse_main(argc - 2, argv + 2, &grid_oper, NULL); */

    if (ret)
        printf("\n");

    fuse_opt_free_args(&args);


    return ret;
}

