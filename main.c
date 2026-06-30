#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ptr_vector.h"
#include "http_request.h"
#include "http_header.h"

#define CRLF_LENGTH 2

typedef enum {
    STATE_START = 0,
    STATE_REQ_METHOD,
    STATE_REQ_PATH,
    STATE_REQ_VERSION,
    STATE_HEADER_KEY,
    STATE_HEADER_VALUE,
    STATE_HEADERS_DONE,
    STATE_CRLF,
    STATE_BODY,
    STATE_DONE,
    STATE_ERROR,
} State;


char *http_request_text = "GET /index.html HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "User-Agent: curl/8.0\r\n"
                    "Accept: */*\r\n"
                    "Connection: close\r\n"
                    "\r\n";


static void *find(char *buf, const char sub) {
   return strstr(buf, &sub);
}

static char *next_char_pos(const char *buf, const char ch) {
    if (!buf) {
        fprintf(stderr, "Buffer has nullptr\n");
        return NULL;
    }

    char *position = strchr(buf, ch);
    if (!position) {
        fprintf(stderr, "Cannot find symbol %c\n", ch);
        return NULL;
    }

    return position;
}


static void write_until_ptr(char *buf, const char *start, const char *end) {
    if (!buf || !start || !end) return;

    int len = end - start + 1;
    snprintf(buf, len, "%.*s", len, start);
}

static void *write_until_char(char *buf, char *start, const char ch) {
    char *end = next_char_pos(start, ch);
    int len = end - start + 1;
    snprintf(buf, len, "%.*s", len, start);
    return end;
}

int main(void) {
    State state = STATE_START;
    char error_msg[64] = {0};
    HTTPRequest *req = http_request_new();

    char *start = http_request_text;
    char *end = NULL;
    HTTPHeader *curr_header = http_header_new();
    PtrVector *headers = ptr_vector_new(4);

    while(state != STATE_DONE) {
        switch (state) {
            case STATE_START: state = STATE_REQ_METHOD; break;
            case STATE_REQ_METHOD:
                end = write_until_char(req->method, start, ' ');
                start = end + 1;
                state = STATE_REQ_PATH;
                break;

            case STATE_REQ_PATH:
                end = write_until_char(req->path, start, ' ');
                start = end + 1;
                state = STATE_REQ_VERSION;
                break;

            case STATE_REQ_VERSION:
                end = write_until_char(req->version, start, '\r');
                start = end + CRLF_LENGTH;
                state = STATE_HEADER_KEY;
                break;

            case STATE_HEADER_KEY:
                // if (curr_header) {
                //     http_header_reset(curr_header);
                // }

                end = next_char_pos(start, ':');
                if (!end) {
                    // error_msg = "Cannot find symbol ':'";
                    state = STATE_ERROR;
                    break;
                }

                curr_header->key = calloc(end - start + 1, sizeof(char));
                write_until_ptr(curr_header->key, start, end);

                start = end + 2;  // : + space
                state = STATE_HEADER_VALUE;
                break;

            case STATE_HEADER_VALUE:
                if (!curr_header || !curr_header->key) {
                    fprintf(stderr, "Cannot find key for value\n");
                    state = STATE_ERROR;
                    break;
                }

                while (*start == ' ') {
                    start += 1;
                }

                end = next_char_pos(start, '\r');
                if (!end) {
                    // error_msg = "Cannot find symbol ':'";
                    state = STATE_DONE;
                    break;
                }

                curr_header->value = calloc(end - start + 1, sizeof(char));
                write_until_ptr(curr_header->value, start, end);
                ptr_vector_push(headers, curr_header, sizeof(HTTPHeader));

                start = end + CRLF_LENGTH;
                state = STATE_HEADER_KEY;
                break;

            case STATE_HEADERS_DONE:
                break;

            case STATE_ERROR:
                state = STATE_DONE;
                break;

            default: state = STATE_DONE; break;
        }
    }

    printf("Method = %s\n", req->method);
    printf("Path = %s\n", req->path);
    printf("Version = %s\n", req->version);

    for (int i = 0; i < headers->length; i++) {
        HTTPHeader *h = ptr_vector_at(headers, i);
        printf("Header = %s -> %s\n", h->key, h->value);
    }

    free(curr_header);
    http_request_free(req);

    return 0;
}
