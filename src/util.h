#ifndef UTIL_H
#define UTIL_H

void get_filetype(char *filename, char *filetype);
int parse_uri(char *uri, char *filename, char *cgiargs);
void logger(char *msg);
int buf_append(char *buf, int bufsize, char *value);

#endif // !UTIL_H
