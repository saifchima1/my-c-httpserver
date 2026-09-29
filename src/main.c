#define _GNU_SOURCE
#include "def.h"
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

enum options{
  i,
  p
};

int main(int count, char **argv) {
  char *index = "index.html";
  char *port = "8080";
  static struct option options[] = {
    {"index",required_argument,0,'i'},
    {"port",required_argument,0,'p'}
  };
  if (count <= 1){
    index = "index.html";
  }else if(count == 2)
  {
    index = argv[1];
  }else if(count == 3){
    if (strcmp(argv[1],"-i") == 0 ||strcmp(argv[1],"--index") == 0){
      index = argv[1];
    } else if(strcmp(argv[1],"-p") == 0 ||strcmp(argv[1],"--port") == 0){
      port = argv[2];
    } else{
      index = argv[1];
      port = argv[2];
    }
  }else{
    bool worsecase = true;
    int opt = 0;
    while ((opt = getopt_long(count,argv,"i:p:",options,NULL)) != -1){
      switch (opt){
        case 'i':
          worsecase = false;
          index = optarg;
          break;
        case 'p':
          worsecase = false;
          port = optarg;
          break;
      }
    }
    if (worsecase){
      printf("this program is ccurrently very primitave it's only \"./server [index] [port]\" nothing else\n");
      return 1;
    }
  }
  struct addrinfo *servaddr, hint;
  int sockfd, thierfd;
  thierfd = -67;
  struct sockaddr_storage thieraddr;
  socklen_t thieraddrlen = sizeof thieraddr;
  // initialisation:
  memset(&hint, 0, sizeof hint);
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;
  int status = getaddrinfo(NULL, port, &hint, &servaddr);
  if (status != 0) {
    errorhandle(status);
  }
  if ((sockfd = socket(servaddr->ai_family, servaddr->ai_socktype,
                       servaddr->ai_protocol)) < 0) {
    errorhandle(sockfd);
  }
  if ((status = bind(sockfd, servaddr->ai_addr, servaddr->ai_addrlen)) < 0) {
    errorhandle(status);
  }
  if ((status = listen(sockfd, 5)) < 0) {
    errorhandle(status);
  }
  // end of initialisation:
  char recvbuff[RECVSIZE] = {0};
  int bytes = 0;
  char *body = NULL;
  size_t bodylen = 0;
  char *http = "HTTP/1.1 200 OK\r\nContent-length: ";
  long int httplen = 0, numlen = 0;
  char num[20];
  char *sendbuff;
  char ext[26] = {0};
  char *extlist[26] = {".js"};
  char *extheader[50] = {"\r\nContent-Type: application/javascript"};
  size_t extlen = 0;
  while (1) {
    if ((thierfd = accept(sockfd, (struct sockaddr *)&thieraddr,
                          &thieraddrlen)) < 0) {
      errorhandle(thierfd);
    }
    if ((bytes = recv(thierfd, recvbuff, RECVSIZE, 0)) < 0) {
      errorhandle(bytes);
    }
    printf("%s\n", recvbuff);
    if (strncmp(recvbuff, "GET", 3) != 0) {
      printf("this is not a get request!\n");
      return 1;
    }
    body = httphandler(recvbuff, strlen(recvbuff), index, &bodylen, ext);
    if (!body) {
      continue;
    }
    printf("\n");
    snprintf(num, 20, "%li", bodylen);
    numlen = strlen(num);
    extlen = strlen(extheader[js]);
    httplen = strlen(http);
    if (strcmp(extlist[js], ext) == 0) {
      sendbuff = calloc(httplen + extlen + bodylen + numlen + 5, 1);

    } else {
      sendbuff = calloc(httplen + bodylen + numlen + 5, 1);
    }
    strcpy(sendbuff, http);
    strcpy(sendbuff + httplen, num);
    if (strcmp(extlist[js], ext) == 0) {
      strcpy(sendbuff + httplen + numlen, extheader[js]);
      strcpy(sendbuff + httplen + numlen + extlen, "\r\n\r\n");
      memcpy(sendbuff + httplen + numlen + extlen + 4, body, bodylen);
      if ((bytes = send(thierfd, sendbuff,
                        httplen + extlen + bodylen + numlen + 5, 0)) <= 0) {
        errorhandle(bytes);
      }
      printf("buffer size: %zu\n", httplen + extlen + bodylen + numlen + 5);

    } else {
      strcpy(sendbuff + httplen + numlen, "\r\n\r\n");
      memcpy(sendbuff + httplen + numlen + 4, body, bodylen);
      if ((bytes = send(thierfd, sendbuff, httplen + bodylen + numlen + 5,
                        0)) <= 0) {
        errorhandle(bytes);
      }
      printf("buffer size: %zu\n", httplen + bodylen + numlen + 5);
    }
    printf("bytes sent: %i\n", bytes);
    printf(" %s\n ext: %s\n", sendbuff, ext);
    memset(recvbuff, 0, RECVSIZE);
    close(thierfd);
    free(sendbuff);
    free(body);
    body = NULL;
    memset(ext, 0, 26);
  }

  freeaddrinfo(servaddr);
  return 0;
}
