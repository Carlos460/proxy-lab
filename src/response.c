#include "csapp.h"
#include "transaction.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void get_header(char *buf, char *name, char *value);

void handle_response(Transaction *t) {
    char buf[MAXLINE];
    rio_t *rio = &(t->res_rio);
    int content_length;

    // store response
    Rio_readlineb(&(t->res_rio), buf, MAXLINE);

    printf("t->res_buf: %s \n", t->res_buf);
    buf_append(t->res_buf, t->res_bufsize, buf);
    printf("t->res_buf: %s \n", t->res_buf);

    // handle headers
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rio, buf, MAXLINE);
        buf_append(t->res_buf, t->res_bufsize, buf);

        // get header name
        printf("%s", buf);

        char header_name[strlen(buf)];
        char header_value[strlen(buf)];
        memset(header_name, 0, strlen(buf));
        memset(header_value, 0, strlen(buf));

        get_header(buf, header_name, header_value);

        if (strcmp(header_name, "Content-length") == 0) {
            printf("name: %s\nvalue: %s\n", header_name, header_value);
            content_length = atoi(header_value);
        }
    }

    // handle body
    t->res_buf_body_size = content_length;
    t->res_buf_body =
        (unsigned char *)malloc(content_length * sizeof(unsigned char));

    printf("\nprinting...content-length: %d\n", content_length);
    Rio_readnb(rio, t->res_buf_body, content_length);
}

void get_header(char *buf, char *name, char *value) {
    bool is_header_name_read = false;

    int offset = 0;
    // header name stops at the first ":"
    for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == '\r') {
            value[i - offset - 1] = '\0';
            return;
        }

        if (buf[i] == ':') {
            is_header_name_read = true;
            i++; // accounts for the space padding after the ":"
            offset = i;
            continue;
        }

        if (!is_header_name_read) {
            name[i] = buf[i];
        } else {
            value[i - offset - 1] = buf[i];
        }
    }
}

void forward_response(Transaction *t) {
    char *buf = t->res_buf;

    // memset(buf, 0, MAXLINE);

    /*
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf + strlen(buf), "Server: Tiny Web Server\r\n");
    sprintf(buf + strlen(buf), "Connection: close\r\n");
    sprintf(buf + strlen(buf), "Content-length: %d\r\n", 5);
    sprintf(buf + strlen(buf), "Content-type: %s\r\n\r\n", "text/plain");
    */

    Rio_writen(t->client_fd, t->res_buf, strlen(t->res_buf));
    Rio_writen(t->client_fd, t->res_buf_body, t->res_buf_body_size);
    printf("Response headers:\n");
    printf("%s", buf);

    /* send response body to client */
}
