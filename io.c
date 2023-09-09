#include <stdio.h>

#include "byte-array.h"

uint64_t byte_array_append_file_contents(byte_array_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "r");
  uint8_t buffer[1024];
  uint64_t n_added = 0;

  while (1) {
    uint64_t n_read = fread(buffer, 1, sizeof(buffer), file);
    if (n_read == 0) {
      break;
    }
    byte_array_append_bytes(bytes, buffer, n_read);
    n_added++;
  }

  fclose(file);

  return n_added;
}

void byte_array_write_file(byte_array_t* bytes, char* file_name) {}
