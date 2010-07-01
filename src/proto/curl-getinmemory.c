#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "main_types.h"


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

    static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
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

buffer_t * download (char * baseurl, char * urlpath, short trailling_slash)
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



    chunk = malloc (sizeof(buffer_t));

    chunk -> data = NULL; /* we expect realloc(NULL, size) to work */ 
    chunk -> size = 0;    /* no data at this point */ 

    curl_global_init(CURL_GLOBAL_ALL);


    printf ("Creating headers Accept and Content-Type\n");
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

    printf ("foo!\n%s\n", chunk -> data);


    /*
    if(chunk -> memory)
        free(chunk -> memory);
    */


    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();

    return chunk;
}

