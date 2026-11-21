#include "csapp.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_response(char buf[MAXLINE]) {

}

void forward_response(int fd, char buf[MAXLINE]) {
    memset(buf, 0, MAXLINE);

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf+strlen(buf), "Server: Tiny Web Server\r\n");
    sprintf(buf+strlen(buf), "Connection: close\r\n");
    sprintf(buf+strlen(buf), "Content-length: %d\r\n", 5);
    sprintf(buf+strlen(buf), "Content-type: %s\r\n\r\n", "text/plain");

    Rio_writen(fd, buf, strlen(buf));
    printf("Response headers:\n");
    printf("%s", buf);

    /* send response body to client */
}
