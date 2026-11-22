#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "csapp.h"

typedef struct {
    // client to server
    int client_fd;

    // incoming client
    char hostname[MAXLINE];
    char port[MAXLINE];

    // other
    char buf[MAXLINE];
    int data;
} Transaction;

#endif
