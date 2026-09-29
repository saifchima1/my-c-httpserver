#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "def.h"

void errorhandle(int errcode) {
  fprintf(stderr, "error code:\n");
  exit(errcode);
}

bool instrarray(char **array, size_t size, char *item) {
  for (size_t i = 0; i < size; i++) {
    if (strcmp(array[i], item) == 0) {
      return true;
    }
  }
  return false;
}
