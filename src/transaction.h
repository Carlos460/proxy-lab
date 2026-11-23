#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "csapp.h"

typedef struct {
    // incoming client
    int client_fd;
    rio_t req_rio;
    char req_buf[MAXLINE];

    // client to server from proxy
    rio_t res_rio;
    char res_buf[MAXLINE];

    // other
    char hostname[MAXLINE];
    char port[MAXLINE];
    char target_hostname[MAXLINE];
    char target_port[MAXLINE];
} Transaction;

Transaction create_transaction(int fd);

#endif
