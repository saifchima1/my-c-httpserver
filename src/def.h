#ifndef DEF_H_
#define DEF_H_
#include <stdbool.h>
#include <stddef.h>

#define RECVSIZE 5000

void errorhandle(int errcode);
bool keep_alive(char *http, size_t size);
char *filepath(char *http, size_t size);
char *httphandler(char *http, size_t size);

enum request { GET, POST, PUT, HEAD, DELETE, PATCH, OPTIONS, CONNECT, TRACE };

#endif
