#include "csapp.h"
#include "tiny/csapp.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void handle_request(int fd);
void forward_request(char req_buf[MAXLINE]);
void handle_response(char buf[MAXLINE]);
void forward_response(int fd, char buf[MAXLINE]);

void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void get_filetype(char *filename, char *filetype);
void clienterror(int fd, char *cause, char *errnum, 
                 char *shortmsg, char *longmsg);

int listenfd, connfd;
char hostname[MAXLINE], port[MAXLINE];

socklen_t clientlen;
struct sockaddr_storage clientaddr;
SA *clientp = (SA *)&clientaddr;


int main(int argc, char **argv)
{
    char *program_name = argv[0];

    /* verify program usage */
    if (argc != 2) {
        fprintf(stderr, "usuage: %s <port>\n", program_name);
        exit(1);
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

    printf("%s", user_agent_hdr);
    return 0;
}


/* serve functions */

/* forward_request: forwards client request to target host
 */
void handle_request(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE/3-1], uri[MAXLINE/3-1], version[MAXLINE/3-1];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;


    printf("handling request...\n");
    /* Read request line and headers */
    Rio_readinitb(&rio, fd);

    if (!Rio_readlineb(&rio, buf, MAXLINE)) return;
    printf("buffer:\n");
    printf("%s", buf);
    printf("\n");

    sscanf(buf, "%s %s %s", method, uri, version);
    read_requesthdrs(&rio);
    // check for http/version and convert to 1.0
    printf("method: %s\nuri: %s\nversion: %s\n", method, uri, version);

    if (strcmp(version, "HTTP/1.1") == 0) {
        strcpy(version, "HTTP/1.0");
        printf("version updated to: %s\n", version);
        snprintf(buf, sizeof(buf), "%s %s %s", method, uri, version);
        printf("updated buffer:\n%s\n\n", buf);
    }



    printf("forwarding request...\n");
    forward_request(buf);
    printf("response:\n%s\n", buf);

    handle_response(buf);

    forward_response(fd, buf);
    // create client
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


void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body+strlen(body), "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body+strlen(body), "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body+strlen(body), "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body+strlen(body), "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

/* helper function */
void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
    strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
    strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
    strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
    strcpy(filetype, "image/jpeg");
    else
    strcpy(filetype, "text/plain");
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {
	strcpy(cgiargs, "");
	strcpy(filename, ".");
	strcat(filename, uri);
	if (uri[strlen(uri)-1] == '/')
	    strcat(filename, "home.html");
	return 1;
    }
    else {  /* Dynamic content */
	ptr = index(uri, '?');
	if (ptr) {
	    strcpy(cgiargs, ptr+1);
	    *ptr = '\0';
	}
	else
	    strcpy(cgiargs, "");
	strcpy(filename, ".");
	strcat(filename, uri);
	return 0;
    }
}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}
