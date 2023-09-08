#include "fatal-error.h"
#include <stdio.h>
#include <stdlib.h>

void _Noreturn fatal_error_impl(char *file, int line, int error_code) {
  fprintf(stderr, "%s:%d: ERROR %d\n", file, line, error_code);
  fprintf(stderr, "Exiting...\n");
  exit(-(error_code + 100));
}
