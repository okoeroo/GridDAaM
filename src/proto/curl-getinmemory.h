#ifndef FOO_H
    #define FOO_H

#include "common.h"
#include "cache.h"


buffer_t * download (char * baseurl, char * urlpath, short trailling_slash);
int upload(const char * baseurl, const char * urlpath);


#endif /* FOO_H */
