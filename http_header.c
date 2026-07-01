#include <stdlib.h>
#include "http_header.h"

HTTPHeader *http_header_new() {
    HTTPHeader *header = malloc(sizeof(HTTPHeader));
    return header;
}

void http_header_reset(HTTPHeader *header) {
    free(header->key);
    free(header->value);
}

void http_header_free(HTTPHeader *header) {
    free(header);
}

