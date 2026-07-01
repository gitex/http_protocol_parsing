#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "ptr_vector.h"
#include "http_header.h"

#define INITIAL_HEADERS_SIZE 4

typedef struct {
    char method[8];
    char path[64];
    char version[10];
    PtrVector *headers;
} HTTPRequest;

HTTPRequest *http_request_new(void);
void http_request_add_header(HTTPRequest *req, HTTPHeader *header);
void http_request_free(HTTPRequest *req);


#endif  // !HTTP_REQUEST_H
