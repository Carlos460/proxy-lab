#include "globals.h"
#include "csapp.h"
#include <stdbool.h>
#include <sys/socket.h>

int g_listenfd, g_connfd;
struct sockaddr_storage g_clientaddr;
socklen_t g_clientlen = sizeof(g_clientaddr);
bool g_is_verbose = false;
SA *g_clientp = (SA *)&g_clientaddr;
