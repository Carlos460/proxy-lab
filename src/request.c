#include "csapp.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/* forward_request: forwards client request to target host
 */
void handle_request(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE/3-1], uri[MAXLINE/3-1], version[MAXLINE/3-1];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    char header_list[10][MAXLINE] = {0};

    if (is_verbose) {
        printf("handling request...\n");
        printf("request buffer:\n%s\n\n", buf);
    }

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);

    if (!Rio_readlineb(&rio, buf, MAXLINE)) return;

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

    if (is_verbose) printf("forwarding request...\nbuffer:%s\n", buf);

    forward_request(buf);
    printf("response:\n%s\n", buf);

    handle_response(buf);

    forward_response(fd, buf);
}

void forward_request(char buf[MAXLINE]) {
    int clientfd;
    rio_t rio;
    char *host = "localhost";
    char *port = "4500";

    printf("opening client fd...\n");
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio,clientfd);

    printf("preparing to read reponse...\n");

    printf("reading response...\n");
    Rio_writen(clientfd, buf, strlen(buf));

    printf("closing client fd\n");
    Close(clientfd);
}


