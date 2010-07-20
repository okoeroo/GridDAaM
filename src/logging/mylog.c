#include <stdio.h>
#include <stdlib.h>

#include "mylog.h"


FILE * griddaam_logfile = NULL;

void setGriddaam_logfile (FILE * f);
FILE * getGriddaam_logfile (void);


FILE * getGriddaam_logfile (void)
{
    return griddaam_logfile;
}

void setGriddaam_logfile (FILE * f)
{
    griddaam_logfile = f;
}
