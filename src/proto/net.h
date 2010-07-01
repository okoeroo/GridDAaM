/* cdmifs, a filesystem interface based on the CDMI standard.
 * Koen Bollen <meneer koenbollen nl>
 * 2010 GPL
 */
#ifndef NET_H
#define NET_H 1

#ifdef HAVE_CONFIG
# include <config.h>
#endif

#define APP_NAME "griddaamfs"
#define APP_VERSION "0.1"

#include <curl/curl.h>

#define USERAGENT APP_NAME"/"APP_VERSION


typedef struct buffer_s
{
	char *data;
	size_t size;
	size_t pos;
} buffer_t;

extern CURLcode curl_defaults( CURL *curl, int flags );
extern char *download( CURL *curl );
extern CURLcode upload( CURL *curl, const char *data, size_t size );
extern char *simple( const char *url );

#endif /* !NET_H */

