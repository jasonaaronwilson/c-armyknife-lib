#include "fatal-error.h"
#include <stdio.h>
#include <stdlib.h>

void _Noreturn fatal_error(int error_code) {
  fprintf(stderr, "Fatal error %d.\n", error_code);
  fprintf(stderr, "Exiting...\n");
  exit(-(error_code + 100));
}
