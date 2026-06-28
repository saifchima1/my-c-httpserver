#include "def.h"
#include <stdio.h>
#include <stdlib.h>

void errorhandle(int errcode) {
  fprintf(stderr, "error code:\n");
  exit(errcode);
}
