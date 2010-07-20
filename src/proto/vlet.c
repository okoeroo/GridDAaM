#include "vlet.h"



int vlet_get_srm (const char * srmuri, char * localuri)
{
    int ret = 0;
    char * buf = NULL;

    if ((!srmuri) || (!localuri))
        return -1;

    /* Perform a system call to fetch the data to the local scratch space */
    buf = malloc (sizeof (char) * PATH_MAX * 2);
    ret = snprintf (buf, (PATH_MAX * 2) - 1, "%s %s %s %s", "uricopy.sh", "-mkdirs", srmuri, localuri);
    if (ret >= ((PATH_MAX * 2) - 1))
    {
        return -1;
    }

    /* Execute the command */
    ret = system (buf);

    free(buf);
    return ret;
}



int vlet_put_srm (const char * localuri, const char * srmuri)
{
    int ret = 0;
    char * buf = NULL;

    if ((!srmuri) || (!localuri))
        return -1;


    /* Perform a system call to put the data to the srm space from the scratch disk */
    buf = malloc (sizeof (char) * PATH_MAX * 2);
    if (!buf)
        return -1;

    ret = snprintf (buf, (2 * PATH_MAX) - 1, "%s %s %s %s", "uricopy.sh", "-force -mkdirs", localuri, srmuri);
    if (ret >= ((2 * PATH_MAX) - 1))
        return -1;

    /* Execute the command */
    ret = system (buf);

    free(buf);
    return ret;
}
