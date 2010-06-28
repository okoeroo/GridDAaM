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


#ifndef GRIDDAAM_DRIVER_FUNC_H
    #define GRIDDAAM_DRIVER_FUNC_H

const char *grid_str = "Hello World!\n";
const char *grid_path = "/grid";


const char * debug_file = "debug";



struct stat * GDDI_getattr (const char * path);
int grid_getattr(const char *path, struct stat *stbuf);


#endif /* GRIDDAAM_DRIVER_FUNC_H */
