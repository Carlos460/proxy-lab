/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;
memset(content,0,MAXLINE);
    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
	    p = strchr(buf, '&');
	    *p = '\0';
	    strcpy(arg1, buf);
	    strcpy(arg2, p+1);
	    n1 = atoi(arg1);
	    n2 = atoi(arg2);
    }

    /* Make the response body */
    sprintf(content, "<h1>Welcome to add.com: ");
    sprintf(content+strlen(content), "THE Internet addition portal.</h1>\r\n");
    sprintf(content+strlen(content), "<p>The answer is: %d + %d = %d</p>\r\n", 
	    n1, n2, n1 + n2);
    sprintf(content+strlen(content), "<p>Thanks for visiting!</p>\r\n");
  
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
/* $end adder */
