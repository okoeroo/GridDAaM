#include "common.h"
#include "cache.h"


static cache_list_t * clist = NULL;


int cache_add (const char * url, buffer_t * buffer)
{
    cache_list_t * tmp_list = NULL;

    if (!buffer)
    {
        return 1;
    }

    if (!clist)
    {
        clist = malloc (sizeof (cache_list_t));
        clist -> cached = NULL;
        clist -> next = NULL;
    }

    tmp_list = clist;
    while (tmp_list -> next)
        tmp_list = tmp_list -> next;

    tmp_list -> cached = malloc (sizeof (cache_block_t));
    tmp_list -> cached -> cached_url = strdup (url);
    tmp_list -> cached -> buffer = malloc (sizeof(buffer_t));
    /* Haxx*/
    tmp_list -> cached -> buffer -> data   = strdup (buffer -> data);
    tmp_list -> cached -> buffer -> size   = buffer -> size;
    tmp_list -> cached -> buffer -> offset = buffer -> offset;

    tmp_list -> cached -> ttl_end_time = time(NULL) + CACHE_TTL;

    return 0;
}


buffer_t * cache_fetch (const char * url)
{
    cache_list_t * tmp_list = NULL;
    buffer_t * buf = NULL;

    if (!clist || !url)
    {
        return NULL;
    }

    tmp_list = clist;
    while (tmp_list)
    {
        if (tmp_list -> cached)
        {
            if (strcmp(tmp_list -> cached -> cached_url, url) == 0)
            {
                buf = malloc (sizeof(buffer_t));
                buf -> data   = strdup (tmp_list -> cached -> buffer -> data);
                buf -> size   = tmp_list -> cached -> buffer -> size;
                buf -> offset = tmp_list -> cached -> buffer -> offset;
                
                return buf;
            }
        }

        tmp_list = tmp_list -> next;
    }

    return NULL;
}
