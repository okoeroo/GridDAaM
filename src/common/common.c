#include "common.h"


static char * gridFSURL = NULL;


void setGridFSURL (char * url)
{
    gridFSURL = url;
}

char * getGridFSURL (void)
{
    return gridFSURL;
}

