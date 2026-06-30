#include <stdlib.h>
#include "http_header.h"

HTTPHeader *http_header_new(void) {
    HTTPHeader *header = malloc(sizeof(HTTPHeader));
    return header;
}

void *http_header_set_key(HTTPHeader *header, char *key, int length) {
    header->key = malloc(sizeof(char) * length);
    return header->key;
}

void http_header_reset(HTTPHeader *header) {
    free(header->key);
    free(header->value);
}

void http_header_free(HTTPHeader *header) {
    free(header);
}

#ifdef HTTP_HEADER_ALIAS
    #define header_new http_header_new
    #define header_reset http_header_reset
    #define header_free http_header_free
#endif  // !HTTP_HEADER_ALIAS


