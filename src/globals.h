#ifndef GLOBALS_H
#define GLOBALS_H

#include "csapp.h"
#include <stdbool.h>
#include <sys/socket.h>

/* You won't lose style points for including this long line in your code */
static const char *g_user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

extern bool g_is_verbose;
extern int g_listenfd, g_connfd;
extern socklen_t g_clientlen;
extern struct sockaddr_storage g_clientaddr;
extern SA *g_clientp;
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#endif
