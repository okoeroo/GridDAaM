#include "common.h"


static char * gridFSURL = NULL;


void setGridFSURL (char * url)
{
    if (url)
    {
        free(gridFSURL);
        gridFSURL = NULL;

        gridFSURL = malloc (sizeof(char) * (strlen(url) + 1));
        bzero(gridFSURL, (strlen(url) + 1));
        memcpy (gridFSURL, url, strlen(url));
    }
}

char * getGridFSURL (void)
{
    return gridFSURL;
}

