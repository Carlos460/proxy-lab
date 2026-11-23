#include "csapp.h"
#include "globals.h"
#include "request.h"
#include "response.h"
#include "transaction.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void service_client(int fd);

int main(int argc, char **argv) {
    char *program_name = argv[0];

    /* verify program usage */
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s [-v] <port>\n", program_name);
        exit(1);
    }

    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        g_is_verbose = true;
    }

    char *PORT = argv[1];

    g_listenfd = Open_listenfd(PORT);

    while (true)
        service_client(Accept(g_listenfd, g_clientp, &g_clientlen));

    return 0;
}

void service_client(int fd) {
    Transaction t = create_transaction(fd);
    printf("Accepted connection from (%s, %s)\n", t.hostname, t.port);

    handle_request(&t);
    forward_request(&t);
    handle_response(&t);
    forward_response(&t);

    Close(t.client_fd);
}
