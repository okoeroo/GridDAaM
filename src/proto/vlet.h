#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include <sys/stat.h>


#ifndef VLET_H
    #define VLET_H

int vlet_get_srm (const char * srmuri, char * localuri);
int vlet_put_srm (const char * localuri, const char * srmuri);

#endif /* VLET_H */
