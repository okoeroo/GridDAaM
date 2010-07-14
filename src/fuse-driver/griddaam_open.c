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


int grid_open(const char *path, struct fuse_file_info *fi)
{
    int res = 0;

    /* Touch the path */
    printf ("%s ------------------- Putting: %s%s/\n", __func__, getGridFSURL(), path);
    res = upload (getGridFSURL(), path, NULL, 1);
    printf ("%s ------------------- Fetching: %s%s/\n", __func__, getGridFSURL(), path);

    return res;
}
