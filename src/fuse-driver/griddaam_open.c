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
#include <string.h>

#include <jansson.h>
#include <libgen.h>

#include "common.h"
#include "griddaam_driver_func.h"
#include "vlet.h"


int grid_open(const char *path, struct fuse_file_info *fi)
{
    int res = 0;
    char * localuri = NULL;
    char * tmpfilename = NULL;
    FILE * f = NULL;

    /* Construct mkstemp() input */
    tmpfilename = malloc(sizeof(char) * PATH_MAX);
    if (!tmpfilename)
        return -EIO;

    strcpy (tmpfilename, getScratchDir());
    if (tmpfilename[strlen(tmpfilename) - 1] != '/')
        strcat (tmpfilename, "/");
    strcat (tmpfilename, "griddaam.XXXXXXXX");

    tmpfilename = mkstemp(tmpfilename);


    /* Construct Local URI */
    localuri = malloc (sizeof(char) * PATH_MAX);
    strcpy (localuri, "file://");
    strcat (localuri, tmpfilename);

    /* Fetch file to scratch file */
    if (vlet_get_srm ("srm://tbn18.nikhef.nl:8446/dpm/nikhef.nl/home/dteam/okoeroo/fuse/testfile", "file:///tmp/bar/foo"))
    {
        /* File transfer to local scratch space failed */
        return -EIO; 
    }
    else
    {
        f = fopen (tmpfilename, "r");
        if (f)
            printf ("I haz file\n");
    }

    free(tmpfilename);
    return 0;
}
