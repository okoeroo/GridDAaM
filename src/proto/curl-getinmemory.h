#ifndef FOO_H
    #define FOO_H

#include "common.h"
#include "cache.h"


buffer_t * download (const char * baseurl, const char * urlpath, const short trailling_slash);
int upload(const char * baseurl, const char * urlpath, const char * storage_uri, const short trailling_slash);


#endif /* FOO_H */
