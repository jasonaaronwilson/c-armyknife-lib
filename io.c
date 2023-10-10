#line 2 "io.c"
/**
 * @file io.c
 *
 * This contains routines to read the contents of a file or write a
 * new file.
 */

// ======================================================================
// This is block is extraced to allocate.h
// ======================================================================

#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);
extern void buffer_write_file(buffer_t* bytes, char* file_name);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

__attribute__((warn_unused_result)) buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "r");
  uint8_t buffer[1024];

  while (1) {
    uint64_t n_read = fread(buffer, 1, sizeof(buffer), file);
    if (n_read == 0) {
      break;
    }
    bytes = buffer_append_bytes(bytes, buffer, n_read);
  }

  fclose(file);

  return bytes;
}

void buffer_write_file(buffer_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "r");
  fwrite(&bytes->elements, 1, bytes->length, file);
  fclose(file);
}
