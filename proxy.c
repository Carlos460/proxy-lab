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
void verify_arguments(int argc, char **argv);

int main(int argc, char **argv) {
    /* verify program usage */
    verify_arguments(argc, argv);

    char *PORT = argc == 2 ? argv[1] : argv[2];
    g_listenfd = Open_listenfd(PORT);

    while (true)
        service_client(Accept(g_listenfd, g_clientp, &g_clientlen));

    Close(g_listenfd);
    return 0;
}

void service_client(int fd) {
    Transaction t = create_transaction(fd);
    printf("Accepted connection from (%s, %s)\n", t.hostname, t.port);

    handle_request(&t);
    forward_request(&t);
    handle_response(&t);
    forward_response(&t);

    free(t.res_buf_body);
    Close(t.client_fd);
}

void verify_arguments(int argc, char **argv) {
    char *program_name = argv[0];

    if (!(argc == 2 || argc == 3)) {
        fprintf(stderr, "Usage: %s [-v verbose] <port>\n", program_name);
        exit(1);
    }

    if (argc == 3 && strcmp(argv[1], "-v") == 0) {
        g_is_verbose = true;
    }
}
