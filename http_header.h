#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H


typedef struct {
    char *key;
    char *value;
} HTTPHeader;


HTTPHeader *http_header_new();
void http_header_reset(HTTPHeader *header);
void http_header_free(HTTPHeader *header);

#ifdef HTTP_HEADER_ALIAS
    HTTPHeader *header_new();
    void header_reset(HTTPHeader *header);
    void header_free(HTTPHeader *header);
#endif  // !HTTP_HEADER_ALIAS

#endif  // !HTTP_HEADER_H
