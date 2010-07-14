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

/* #include "net.h" */
#include "curl-getinmemory.h"


#ifndef GRIDDAAM_DRIVER_FUNC_H
    #define GRIDDAAM_DRIVER_FUNC_H

struct stat * GDDI_getattr (const char * path);
int grid_getattr(const char *path, struct stat *stbuf);
int grid_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi);
int grid_mkdir(const char *path, mode_t mode);
int grid_create(const char *path, mode_t mode, struct fuse_file_info *fi);


#endif /* GRIDDAAM_DRIVER_FUNC_H */
