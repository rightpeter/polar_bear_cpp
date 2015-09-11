#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
// #include <curl/types.h>
#include <curl/easy.h>

// Define our struct for accepting LCs output
struct BufferStruct
{
    char* buffer;
    size_t size;
};

// This is the fuction we pass to LC, which writes the output to a BufferStruct
static size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size* nmemb;

    struct BufferStruct* mem = (struct BufferStruct *) data;
    mem->buffer = realloc(mem->buffer, mem->size + realsize + 1);

    if ( mem->buffer )
    {
        memcpy( &( mem->buffer[ mem->size ] ), ptr, realsize );
        mem->size += realsize;
        mem->buffer[ mem->size ] = 0;
    }
    return realsize;
}

int main()
{
    curl_global_init( CURL_GLOBAL_ALL );
    CURL* myHandler;
    CURLcode result; // We'll store the result of CURL's webpage retrieval, for simple error checking.
    struct BufferStruct output;
    output.buffer = NULL;
    output.size = 0;
    myHandler = curl_easy_init();

    curl_easy_setopt(myHandler, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(myHandler, CURLOPT_WRITEDATA, (void *)&output);
    curl_easy_setopt(myHandler, CURLOPT_URL, "http://api.douyutv.com/api/client/room/rightpeter");
    result = curl_easy_perform(myHandler);
    curl_easy_cleanup(myHandler);

    FILE* fp;
    fp = fopen("rightpeter.json", "w");
    if ( !fp )
        return 1;
    fprintf( fp, "%s", output.buffer );
    fclose( fp );

    if ( output.buffer )
    {
        free ( output.buffer );
        output.buffer = 0;
        output.size = 0;
    }

    printf("LibCurl rules!\n");
    return 0;

    return 0;
}
