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

#include <limits.h>
#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);

extern void buffer_write_file(buffer_t* bytes, char* file_name);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_read_until(buffer_t* buffer, FILE* input, char end_of_line);

int file_peek_byte(FILE* input);

boolean_t file_eof(FILE* input);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

// This is optional...
#include <sys/stat.h>

/**
 * @function buffer_append_file_contents
 *
 * Completely reads a file and appends the contents to the passed in
 * buffer. This is often much more convenient than streaming a file.
 */
__attribute__((warn_unused_result)) buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name) {

  uint64_t capacity = bytes->capacity;

  // This is optional
  {
    struct stat st;
    stat(file_name, &st);
    capacity = st.st_size;
  }

  bytes = buffer_increase_capacity(bytes, capacity);

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

/**
 * @function buffer_write_file
 *
 * Writes the contents of the buffer to the given file.
 */
void buffer_write_file(buffer_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "w");
  fwrite(&bytes->elements, 1, bytes->length, file);
  fclose(file);
}

/**
 * @function buffer_read_until
 *
 * Read from a FILE* until either the end of file is reached or a
 * particular "end-of-line" character is read. Every character except
 * the end_of_line character is appended to the buffer.
 *
 * This can be used to read normal Unix "lines" or another use is to
 * read "lines" that end with NUL (U+0000) or until say a seperator
 * like "," is encountered.
 *
 * This function (and string_tokenize) should take a character set
 * which can unify their interface and allow U+0000 to be a member of
 * the set (unlike using a C string (aka char*) as the character set).
 *
 * TODO(jawilson): write a proper test!
 */
buffer_t* buffer_read_until(buffer_t* buffer, FILE* input, char end_of_line) {
  while (!feof(input)) {
    int ch = fgetc(input);
    if (ch < 0) {
      return buffer;
    }
    if (ch == end_of_line) {
      return buffer;
    }
    buffer = buffer_append_byte(buffer, ch);
  }
  return buffer;
}

/**
 * @function file_peek_char
 *
 * Returns the next byte from the input (as an int not u8_t) or -1 if
 * the end of the file input has been reached. A a byte is read, then
 * the byte is "pushed back" into the input stream so that if
 * file_peek_char, fgetc, or a host of other functions attempt to read
 * the input then
 */
int file_peek_byte(FILE* input) {
  if (feof(input)) {
    return -1;
  }
  int result = fgetc(input);
  // ungetc doesn't "push back" -1 according to
  // https://en.cppreference.com/w/c/io/ungetc
  // But who is going to trust that...
  if (result >= 0) {
    ungetc(result, input);
  }
  return result;
}

/**
 * Return true if an input stream is at the end of the file. I don't
 * know what "feof" really does but it doesn't actually do this.
 */
boolean_t file_eof(FILE* input) {
  return feof(input) || file_peek_byte(input) < 0;
}

#define FILE_COPY_STREAM_BUFFER_SIZE 1024

/**
 * Copy some or all of an input stream to an output stream.
 */
void file_copy_stream(FILE* input, FILE* output, boolean_t until_eof,
                      uint64_t size) {
  if (until_eof) {
    size = ULLONG_MAX;
  }

  uint8_t buffer[FILE_COPY_STREAM_BUFFER_SIZE];
  while (size > 0) {
    int minimum = size < FILE_COPY_STREAM_BUFFER_SIZE
                      ? size
                      : FILE_COPY_STREAM_BUFFER_SIZE;
    uint64_t n_read = fread(buffer, 1, minimum, input);
    if (n_read == 0) {
      break;
    }
    fwrite(buffer, 1, n_read, output);
    size -= n_read;
  }
}
