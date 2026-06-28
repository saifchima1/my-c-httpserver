#include <bits/sockaddr_storage.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define RECVSIZE 5000

void errorhandle(int errcode) {
  fprintf(stderr, "error code:\n");
  exit(errcode);
}

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
  while (1) {
    if (thierfd == -67) {
      if ((thierfd = accept(sockfd, (struct sockaddr *)&thieraddr,
                            &thieraddrlen)) < 0) {
        errorhandle(thierfd);
      }
    }
    if ((bytes = recv(thierfd, recvbuff, RECVSIZE, 0)) < 0) {
      errorhandle(bytes);
    }
  }

  freeaddrinfo(servaddr);
  return 0;
}
