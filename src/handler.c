#include "def.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool keep_alive(char *http, size_t size) {
  if (strstr(http, "Connection: keep-alive") != NULL ||
      (strstr(http, "HTTP/1.1") != NULL &&
       strstr(http, "Connection: closed") == NULL)) {
    return true;
  }
  return false;
}

char *filepath(char *http, size_t size) {
  char *buffer;
  if ((buffer = strstr(http, "/")) == NULL) {
    printf("couldn't find requested file path ");
    errorhandle(1);
  }
  bool found = false;
  size_t i = 0;
  while (i < size) {
    if (buffer[i] == ' ') {
      found = true;
      break;
    }
    i++;
  }
  if (!found) {
    printf("couldn't find the end of the path ");
    errorhandle(1);
  }
  char *result = malloc(i);
  strncpy(result, buffer, i);
  result[i] = '\0';

  return result;
}

char *httphandler(char *http, size_t size, size_t *retsize) {
  char *path = filepath(http, size);
  if (strcmp(path, "/") != 0) {
    char *result = NULL;
    char relative_path[strlen(path) + 2];
    strcpy(relative_path, ".");
    strcpy(relative_path + 1, path);
    free(path);
    FILE *pfile = fopen(relative_path, "rb");
    if (!pfile) {
      errorhandle(1);
    }
    fseek(pfile, 0, SEEK_END);
    size_t fsize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);

    if (!(result = malloc(fsize + 1))) {
      printf("faild to allocate storage for the string ");
      errorhandle(1);
    }
    size_t bytes = fread(result, 1, fsize, pfile);
    if (bytes != fsize) {
      printf("failed reading the file");
      errorhandle(1);
    }
    fwrite(result, 1, bytes, stdout);
    result[bytes] = '\0';
    *retsize = bytes;
    printf("the file has: %s\n", result);

    fclose(pfile);
    return result;
  } else {
    free(path);
    char *indexpath = "./html/index.html";
    char *result = NULL;

    FILE *pfile = fopen(indexpath, "r");
    if (!pfile) {
      perror("null file");
      exit(1);
    }
    fseek(pfile, 0, SEEK_END);
    size_t fsize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);

    if (!(result = malloc(fsize + 1))) {
      printf("faild to allocate storage for the string ");
      errorhandle(1);
    }
    long int bytes = fread(result, 1, fsize, pfile);
    if (bytes != fsize) {
      printf("failed reading the file");
      errorhandle(1);
    }
    result[bytes] = '\0';
    *retsize = bytes;

    fclose(pfile);
    return result;
  }
}
