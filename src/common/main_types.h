#ifndef MAIN_TYPES_H
    #define MAIN_TYPES_H


#ifdef HAVE_CONFIG
    #include <config.h>
#else
    #define PACKAGE_NAME    "griddaamfs"
    #define PACKAGE_VERSION "0.1"
#endif


#define USERAGENT           PACKAGE_NAME"/"PACKAGE_VERSION
/* #define MIMETYPE_JSON "application/json" */
#define MIMETYPE_JSON       "application/GridDAam+json"
#define X_STORAGE_URI       "X-Storage_URI: "


#define CACHE_TTL           60
#define DEFAULT_DIR_SIZE    4096

typedef struct buffer_s
{
    char *data;
    size_t size;
    size_t offset;
} buffer_t;

#endif /* MAIN_TYPES_H */
