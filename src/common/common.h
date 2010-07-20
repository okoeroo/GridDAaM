#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "main_types.h"


#ifndef COMMON_H
    #define COMMON_H

#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif

#define DEFAULT_SCRATCH_DIR "/tmp/griddaam_scratch"


void setGridFSURL (const char * url);
char * getGridFSURL (void);
void setScratchDir (char * path);
char * getScratchDir (void);

extern struct curl_slist *slist_append( struct curl_slist * list, const char * format, ... );
time_t iso8601_decode( const char *isoformat );


#endif /* COMMON_H */
