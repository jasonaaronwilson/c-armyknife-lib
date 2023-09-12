#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "string-util.h"

char* upper_case(char *str) {
  str = strdup(str);
  for (int i = 0; str[i]; i++) {
    str[i] = toupper(str[i]);
  }
  return str;
}

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    fprintf(stdout, "#define HASHCODE_%s UINT64_C(%lu)\n", upper_case(argv[i]), string_hash(argv[i]));
  }
  exit(0);
}
