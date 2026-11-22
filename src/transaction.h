#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "csapp.h"

typedef struct {
    int client_fd;
    char buf[MAXLINE];
    int data;
} Transaction;

#endif
