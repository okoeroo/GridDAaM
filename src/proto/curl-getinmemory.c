#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "main_types.h"
#include "curl-getinmemory.h"



static void *myrealloc(void *ptr, size_t size);

static void *myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocing
       NULL pointers, so we take care of it here */ 
    if(ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}


static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    buffer_t *mem = (buffer_t*)data;

    mem->data= myrealloc(mem->data, mem->size + realsize + 1);
    if (mem->data) {
        memcpy(&(mem->data[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->data[mem->size] = 0;
    }
    return realsize;
}


static size_t read_callback( void *ptr, size_t size, size_t nmemb, void *data )
{
    buffer_t * buf = (buffer_t *) data;

    int length = size * nmemb;
    int left = buf->size - buf->offset;

    if( left < length )
        length = left;

    if( length <= 0 )
        return 0;

    memcpy( ptr, buf->data, length );
    buf->offset += length;

    return length;
}

buffer_t * download (const char * baseurl, const char * urlpath, const short trailling_slash)
{
    CURL *curl_handle;
    static struct curl_slist *headers = NULL;
    char * mainurl = NULL;
    int len = 0;

    buffer_t * chunk = NULL;

    if (!baseurl)
    {
        fprintf (stderr, "Error: no URL specified\n");
        return NULL;
    }

    /* Construct URL */
    len += strlen(baseurl);
    if (urlpath)
        len += strlen(urlpath);

    if (trailling_slash)
        len++;

    mainurl = malloc (sizeof (char) * (len + 1));
    strcpy (mainurl, baseurl);
    if (urlpath)
        strcat (mainurl, urlpath);

    if (trailling_slash)
        strcat (mainurl, "/");

    printf ("======================================================== Curl Fetching : %s\n", mainurl);

    /* Check cache'd data */
    if ((chunk = cache_fetch (mainurl)))
    {
        return chunk;
    }
    


    chunk = malloc (sizeof(buffer_t));

    chunk -> data = NULL; /* we expect realloc(NULL, size) to work */ 
    chunk -> size = 0;    /* no data at this point */ 

    curl_global_init(CURL_GLOBAL_ALL);


    headers = curl_slist_append ( headers, "Accept: "MIMETYPE_JSON);
    headers = curl_slist_append ( headers, "Content-Type: "MIMETYPE_JSON);


    /* init the curl session */ 
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    /* specify URL to get */ 
    curl_easy_setopt(curl_handle, CURLOPT_URL, mainurl);

    /* send all data to this function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */ 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USERAGENT);

    /* get it! */ 
    curl_easy_perform(curl_handle);

    /* cleanup curl stuff */ 
    curl_easy_cleanup(curl_handle);

    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     *
     * You should be aware of the fact that at this point we might have an
     * allocated data block, and nothing has yet deallocated that data. So when
     * you're done with it, you should free() it as a nice application.
     */ 

    cache_add (mainurl, chunk);
    /*
    if(chunk -> memory)
        free(chunk -> memory);
    */


    /* we're done with libcurl, so clean it up */ 
    free(mainurl);
    curl_global_cleanup();

    return chunk;
}


int upload(const char * baseurl, const char * urlpath, const char * storage_uri, const short trailling_slash)
{
    CURL *curl;
    CURLcode res;
    int rc = 0;
    char * mainurl = NULL;
    int len = 0;
    static struct curl_slist *headers = NULL;
    char * storage_uri_header = NULL;

    if (!baseurl)
    {
        fprintf (stderr, "Error: no URL specified\n");
        return 1;
    }

    /* Construct URL */
    len += strlen(baseurl);
    if (urlpath)
        len += strlen(urlpath);

    if (trailling_slash)
        len++;

    mainurl = malloc (sizeof (char) * (len + 1));
    strcpy (mainurl, baseurl);
    if (urlpath)
        strcat (mainurl, urlpath);

    if (trailling_slash)
        strcat (mainurl, "/");

    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) 
    {
        /* we want to use our own read function */ 
        /* curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback); */

        /* enable uploading */ 
        /* curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L); */

        /* HTTP PUT please */ 
        curl_easy_setopt(curl, CURLOPT_PUT, 1L);

        if (storage_uri)
        {
            storage_uri_header = malloc (sizeof(char) * (strlen(X_STORAGE_URI) + strlen(storage_uri) + 1));
            strcpy (storage_uri_header, X_STORAGE_URI);
            strcat (storage_uri_header, storage_uri);
            headers = curl_slist_append (headers, storage_uri_header);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            free(storage_uri_header);
        }

        /* specify target URL, and note that this URL should include a file
           name, not only a directory */ 
        curl_easy_setopt(curl, CURLOPT_URL, mainurl);

        /* now specify which file to upload */ 
        /* curl_easy_setopt(curl, CURLOPT_READDATA, hd_src); */

        /* provide the size of the upload, we specicially typecast the value
           to curl_off_t since we must be sure to use the correct data size */ 
        /* curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size); */

        /* Now run off and do what you've been told! */ 
        res = curl_easy_perform(curl);
        if( res != CURLE_OK )
        {
            rc = 1;
        }
        else
            rc = 0;
    }
    else
        return 1;

    /* always cleanup */ 
    free(mainurl);
    curl_global_cleanup();
    return rc;
}


