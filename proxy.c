#include "csapp.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

int listenfd, connfd;
char hostname[MAXLINE], port[MAXLINE];

socklen_t clientlen;
struct sockaddr_storage clientaddr;
SA *clientp = (SA *)&clientaddr;
int is_verbose = 0;


int main(int argc, char **argv)
{
    char *program_name = argv[0];

    /* verify program usage */
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s [-v] <port>\n", program_name);
        exit(1);
    }

    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        is_verbose = 1;
    }

    char* PORT = argv[1];
    listenfd = Open_listenfd(PORT);

    while(1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, clientp, &clientlen);

        Getnameinfo(clientp, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);

        handle_request(connfd);

        Close(connfd);
    }

    return 0;
}
