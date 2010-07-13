#include "common.h"
#include <curl/curl.h>


static char * gridFSURL   = NULL;
static char * scratch_dir = NULL;



void setScratchDir (char * path)
{
    if (path)
    {
        free(scratch_dir);
        scratch_dir = NULL;

        scratch_dir = calloc (1, sizeof(char) * (strlen(path) + 1));
        memcpy (scratch_dir, path, strlen(path));
    }
}

char * getScratchDir (void)
{
    return scratch_dir;
}


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


struct curl_slist *slist_append( struct curl_slist * list, const char * format, ... )
{
    size_t size = 0;
    char * buf = NULL;
    struct curl_slist *res = NULL;
    va_list ap;

    /* Fix */
    size = strlen(format)+512;
    buf = malloc (sizeof(char) * size);

    va_start(ap, format);
    vsnprintf( buf, size, format, ap );
    va_end(ap);
    
    printf ("Buf is: %s\n", buf);

    res = curl_slist_append( list, buf );

    printf ("Have res: %s\n", res ? "yes" : "no");
    /* Fix */
    /* free(buf); */
    return res;
}


time_t iso8601_decode( const char *isoformat )
{
    struct tm td;
    memset( &td, 0, sizeof(struct tm) );
    strptime( isoformat, "%FT%T", &td );

    return mktime( &td );
}

