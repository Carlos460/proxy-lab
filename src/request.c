#include "csapp.h"
#include "globals.h"
#include "transaction.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void verify_requesthdrs(rio_t *rp, char *buf, size_t maxlen, Transaction *t);

/* handle_request: handles incoming request
 * */
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
        snprintf(buf, sizeof(buf), "%s %s %s\r\n", method, uri, version);
        if (g_is_verbose) {
            printf("version updated to: %s\n", version);
            printf("updated buffer: %s\n", buf);
        }
        buf_append(t->req_buf, t->req_bufsize, buf);
    }

    verify_requesthdrs(&rio, buf, MAXLINE, t);
}

/* helper functions */
void verify_requesthdrs(rio_t *rp, char *buf, size_t maxlen, Transaction *t) {
    char *header_name[] = {"Connection", "Proxy-Connection"};
    char *header_value[] = {"close", "close"};

    Rio_readlineb(rp, buf, MAXLINE);

    if (strcmp(buf, "\r\n") != 0)
        buf_append(t->req_buf, t->req_bufsize, buf);

    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        if (strcmp(buf, "\r\n") == 0)
            printf("nr detected");
        buf_append(t->req_buf, t->req_bufsize, buf);

        // get header name
        printf("%s", buf);
    }

    buf_append(t->req_buf, t->req_bufsize, buf);
    return;
}

/* forward_request: forwards client request to target host
 */
void forward_request(Transaction *t) {
    char *host = "localhost";
    char *port = "4500";
    int fd = Open_clientfd(host, port);
    char buf[MAXLINE];

    rio_t *rio = &(t->res_rio);
    rio->rio_fd = fd;

    Rio_readinitb(rio, fd);

    if (g_is_verbose)
        printf("forwarding request...\nbuffer:%s\n", buf);

    // forwarding request
    Rio_writen(fd, t->req_buf, strlen(t->req_buf));
}
