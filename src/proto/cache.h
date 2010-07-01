#ifndef CACHE_H
    #define CACHE_H

#include <time.h>

#include "common.h"



typedef struct cache_block_s {
    char * cached_url;
    buffer_t * buffer;
    time_t ttl_end_time;
} cache_block_t;

typedef struct cache_list_s {
    cache_block_t * cached;
    struct cache_list_s * next;
} cache_list_t;


int cache_add (const char * url, buffer_t * buffer);
buffer_t * cache_fetch (const char * url);

#endif /* CACHE_H */
