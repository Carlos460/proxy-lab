#include "csapp.h";

void handle_request(int fd);
void forward_request(char req_buf[MAXLINE]);
void handle_response(char buf[MAXLINE]);
void forward_response(int fd, char buf[MAXLINE]);
