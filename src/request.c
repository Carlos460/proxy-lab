#include "csapp.h"
#include "globals.h"
#include "transaction.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/* forward_request: forwards client request to target host
 */
void handle_request(Transaction *t) {
    char buf[MAXLINE], method[MAXLINE / 3 - 1], uri[MAXLINE / 3 - 1],
        version[MAXLINE / 3 - 1];
    rio_t rio;

    if (is_verbose) {
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
        if (is_verbose) {
            printf("version updated to: %s\n", version);
            printf("updated buffer:\n%s\n\n", buf);
        }
    }

    verify_requesthdrs(&rio, buf, MAXLINE);
    printf("rio.rio_buf: \n%s\nn", rio.rio_buf);

    if (is_verbose)
        printf("forwarding request...\nbuffer:%s\n", buf);

    printf("response:\n%s\n", buf);
}

void forward_request(Transaction *t) {
    int clientfd;
    rio_t rio;
    char *host = "localhost";
    char *port = "4500";

    printf("opening client fd...\n");
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    printf("preparing to read reponse...\n");

    printf("reading response...\n");
    Rio_writen(clientfd, t->buf, strlen(t->buf));

    printf("closing client fd\n");
    Close(clientfd);
}

/* helper functions */

void verify_requesthdrs(rio_t *rp, char buf[MAXLINE], size_t maxlen) {
    char header_name[] = {"Connection", "Proxy-Connection"};
    char header_value[] = {"close", "close"};

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}
