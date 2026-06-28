#include "def.h"
#include <bits/sockaddr_storage.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
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
  int status = getaddrinfo(NULL, "8080", &hint, &servaddr);
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
  bool keepalive = false;
  char recvbuff[RECVSIZE] = {0};
  int bytes = 0;
  char *body = NULL;
  long int bodylen = 0;
  char *http = "HTTP/1.1 200 OK\r\nContent-length: ";
  long int httplen = 0, numlen = 0;
  char num[20];
  char *sendbuff;
  while (1) {
    if (!keepalive) {
      if ((thierfd = accept(sockfd, (struct sockaddr *)&thieraddr,
                            &thieraddrlen)) < 0) {
        errorhandle(thierfd);
      }
    }
    if ((bytes = recv(thierfd, recvbuff, RECVSIZE, 0)) < 0) {
      errorhandle(bytes);
    }
    if (strncmp(recvbuff, "GET", 3) != 0) {
      printf("this is not a get request!\n");
      return 1;
    }
    keepalive = true;
    body = httphandler(recvbuff, strlen(recvbuff));
    bodylen = strlen(body);
    snprintf(num, 20, "%li", bodylen + 1);
    numlen = strlen(num);
    httplen = strlen(http);
    sendbuff = calloc(httplen + bodylen + numlen + 5, 1);
    strcpy(sendbuff, http);
    strcpy(sendbuff + httplen, num);
    strcpy(sendbuff + httplen + numlen, "\r\n\r\n");
    strcpy(sendbuff + httplen + numlen + 4, body);
    if ((bytes = send(thierfd, sendbuff, httplen + bodylen + numlen + 5, 0)) <=
        0) {
      errorhandle(bytes);
    }
    printf("%s\n%i\n", sendbuff, bytes);
    if (!keep_alive(recvbuff, strlen(recvbuff))) {
      memset(recvbuff, 0, RECVSIZE);
      close(thierfd);
      keepalive = false;
    }
    free(sendbuff);
    free(body);
    body = NULL;
  }

  freeaddrinfo(servaddr);
  return 0;
}
