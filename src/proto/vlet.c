#include "vlet.h"


#define DEFAULT_SCRATCH_DIR "/tmp/griddaam_scratch"


int vlet_get_srm (const char * srmuri, char ** localuri)
{
    int ret = 0;
    char * scratchdir = getScratchDir();
    char * buf = NULL;

    if ((!srmuri) || (!localuri))
        return -1;

    /* Scratch Directory set? If not, set one now! */
    if (!scratchdir)
    {
        ret = mkdir(DEFAULT_SCRATCH_DIR, 0755);
        if (!ret)
            setScratchDir (DEFAULT_SCRATCH_DIR);
        else
            return ret;
    }

    /* Perform a system call to fetch the data to the local scratch space */
    buf = malloc (sizeof (char) * PATH_MAX);
    ret = snprintf (buf, PATH_MAX - 1, "%s %s %s %s", "uricopy.sh", "-force -mkdirs", srmuri, *localuri);
    if (ret >= (PATH_MAX - 1))
    {
        return -1;
    }

    /* Execute the command */
    ret = system (buf);
    return ret;
}



int vlet_put_srm (const char * localuri, const char * srmuri)
{
    int ret = 0;
    char * scratchdir = getScratchDir();
    char * buf = NULL;

    if ((!srmuri) || (!localuri))
        return -1;

    /* Scratch Directory set? If not, set one now! */
    if (!scratchdir)
        return -1;
    {
        ret = mkdir(DEFAULT_SCRATCH_DIR, 0755);
        if (!ret)
            setScratchDir (DEFAULT_SCRATCH_DIR);
        else
            return ret;
    }

    /* Perform a system call to put the data to the srm space from the scratch disk */
    buf = malloc (sizeof (char) * PATH_MAX);
    if (!buf)
        return -1;

    ret = snprintf (buf, PATH_MAX - 1, "%s %s %s %s", "uricopy.sh", "-force -mkdirs", localuri, srmuri);
    if (ret >= (PATH_MAX - 1))
        return -1;

    /* Execute the command */
    ret = system (buf);
    return ret;
}
