#include <stdio.h>
#include <stdlib.h>

#include "string-util.h"

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    fprintf(stdout, "#define HASHCODE_%s %lu\n", argv[i], string_hash(argv[i]));
  }
  exit(0);
}
