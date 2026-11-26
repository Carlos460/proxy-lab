#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void get_filetype(char *filename, char *filetype) {
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

int parse_uri(char *uri, char *filename, char *cgiargs) {
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri) - 1] == '/')
            strcat(filename, "home.html");
        return 1;
    } else { /* Dynamic content */
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        } else
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

int buf_append(char *buf, int bufsize, char *value) {
    if (g_is_verbose) {
        printf("\nadding vlaue: %s\n", value);
        printf("to buffer   : %s\n", buf);
    }

    if (strlen(buf) + strlen(value) > bufsize)
        return -1;

    snprintf(buf + strlen(buf), bufsize, "%s", value);

    if (g_is_verbose) {
        printf("result buff : %s\n", buf);
    }
    return 0;
};

void logger(char *msg) {
    if (g_is_verbose)
        return;
    printf("%s", msg);
}
