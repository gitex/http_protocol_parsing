#include "http_request.h"
#include "http_header.h"
#include "ptr_vector.h"
#include <stdlib.h>


HTTPRequest *http_request_new() {
    HTTPRequest *req = malloc(sizeof(HTTPRequest));
    req->headers = ptr_vector_new(INITIAL_HEADERS_SIZE);
    return req;
}

void http_request_add_header(HTTPRequest *req, HTTPHeader *header) {
    if (!req->headers) {
        req->headers = ptr_vector_new(INITIAL_HEADERS_SIZE);
    }

    ptr_vector_push_back(req->headers, header, sizeof(HTTPHeader));
}

void http_request_free(HTTPRequest *req) {
    if (!req) return;

    if (req->headers != NULL && req->headers->elements != NULL) {
        for (size_t i = 0; i < req->headers->length; i++) {
            free(req->headers->elements[i]);
        }

        free(req->headers->elements);
        free(req->headers);

    }

    free(req);
}


