#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H


typedef struct {
    char *key;
    char *value;
} HTTPHeader;


HTTPHeader *http_header_new(void);
void http_header_reset(HTTPHeader *header);
void http_header_free(HTTPHeader *header);

#endif  // !HTTP_HEADER_H
