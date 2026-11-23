#include "csapp.h"
#include "globals.h"
#include "transaction.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void buf_append(char *buf, char *value);
void verify_requesthdrs(rio_t *rp, char buf[MAXLINE], size_t maxlen);

void handle_request(Transaction *t) {
    char buf[MAXLINE], method[MAXLINE / 3 - 1], uri[MAXLINE / 3 - 1],
        version[MAXLINE / 3 - 1];
    rio_t rio;

    if (g_is_verbose) {
        printf("handling request...\n");
        printf("request buffer:\n%s\n\n", buf);
    }

    /* Read request line and headers */
    Rio_readinitb(&rio, t->client_fd);

    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;

    sscanf(buf, "%s %s %s", method, uri, version);

    if (strcmp(version, "HTTP/1.1") == 0) {
        strcpy(version, "HTTP/1.0");
        snprintf(buf, sizeof(buf), "%s %s %s", method, uri, version);
        if (g_is_verbose) {
            printf("version updated to: %s\n", version);
            printf("updated buffer:\n%s\n\n", buf);
        }
    }

    verify_requesthdrs(&rio, buf, MAXLINE);
    printf("rio.rio_buf: \n%s\nn", rio.rio_buf);

    if (g_is_verbose)
        printf("forwarding request...\nbuffer:%s\n", buf);

    printf("response:\n%s\n", buf);
}

void buf_append(char *buf, char *value) {

};

/* helper functions */
void verify_requesthdrs(rio_t *rp, char buf[MAXLINE], size_t maxlen) {
    char *header_name[] = {"Connection", "Proxy-Connection"};
    char *header_value[] = {"close", "close"};

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);

        // get header name
        printf("%s", buf);
    }
    return;
}

/* forward_request: forwards client request to target host
 */
void forward_request(Transaction *t) {
    char *host = "localhost";
    char *port = "4500";
    int fd = Open_clientfd(host, port);
    char temp_buf[MAXLINE];

    rio_t *rio = &(t->res_rio);

    Rio_readinitb(rio, fd);

    // forwarding request
    Rio_writen(fd, t->req_rio.rio_buf, strlen(t->req_rio.rio_buf));

    // store response
    Rio_readlineb(rio, temp_buf, strlen(temp_buf));

    Close(fd);
}
