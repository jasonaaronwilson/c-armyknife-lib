#line 2 "byte-array.c"
/**
 * @file byte-array.c
 */

// ======================================================================
// This is block is extraced to byte-array.h
// ======================================================================

#ifndef _BYTE_ARRAY_H_
#define _BYTE_ARRAY_H_

#include <stdint.h>
#include <string.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
} byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);

extern uint64_t byte_array_length(byte_array_t* byte_array);

extern uint8_t byte_array_get(byte_array_t* byte_array, uint64_t position);

extern char* byte_array_c_substring(byte_array_t* byte_array, uint64_t start,
                                    uint64_t end);

extern char* byte_array_to_c_strring(byte_array_t* byte_array);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* byte_array, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* byte_array, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* byte_array, const char* str);

#endif /* _BYTE_ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "byte-array.h"
#include "ct-assert.h"
#include "fatal-error.h"

/**
 * Make an empty byte array with the given initial capacity.
 */
byte_array_t* make_byte_array(uint32_t initial_capacity) {

  // We make the assumption that casting (char*) to (uint8_t*) and
  // vice-versa is completely reasonable which it is on all modern
  // architecures.
  ct_assert(sizeof(char) == 1);

  if (initial_capacity < 1) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  byte_array_t* result
      = (byte_array_t*) (malloc_bytes(initial_capacity + sizeof(byte_array_t)));
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of bytes that have been added to this byte array.
 */
uint64_t byte_array_length(byte_array_t* array) { return array->length; }

/**
 * Get a single byte from a byte array.
 */
uint8_t byte_array_get(byte_array_t* byte_array, uint64_t position) {
  if (position < byte_array->length) {
    return byte_array->elements[position];
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Extract a newly allocated string that contain the bytes from start
 * to end (appending a zero byte to make sure it's a legal C string).
 */
char* byte_array_c_substring(byte_array_t* byte_array, uint64_t start,
                             uint64_t end) {
  // Add one extra byte for a NUL string terminator byte
  char* result = (char*) (malloc_bytes(end - start + 1));
  for (int i = start; i < end; i++) {
    result[i - start] = byte_array->elements[i];
  }
  return result;
}

/**
 * Extract a newly allocated string that contain all of the bytes in the byte
 * buffer as a NU * terminated C string.
 */
char* byte_array_to_c_string(byte_array_t* byte_array) {
  return byte_array_c_substring(byte_array, 0, byte_array->length);
}

/**
 * Append a single byte to the byte array.
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_byte(byte_array_t* byte_array, uint8_t element) {
  if (byte_array->length < byte_array->capacity) {
    byte_array->elements[byte_array->length] = element;
    byte_array->length++;
    return byte_array;
  } else {
    byte_array_t* result = make_byte_array(byte_array->capacity * 2);
    for (int i = 0; i < byte_array->length; i++) {
      result = byte_array_append_byte(result, byte_array_get(byte_array, i));
    }
    free_bytes(byte_array);
    return byte_array_append_byte(result, element);
  }
}

/**
 * Append multiple bytes to the byte array.
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_bytes(byte_array_t* byte_array, uint8_t* bytes,
                            uint64_t n_bytes) {
  // Obviously this can be optimized...
  for (int i = 0; i < n_bytes; i++) {
    byte_array = byte_array_append_byte(byte_array, bytes[i]);
  }
  return byte_array;
}

/**
 * Append all of the bytes from a C string (except the ending NUL
 * char).
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_string(byte_array_t* byte_array, const char* str) {
  return byte_array_append_bytes(byte_array, (uint8_t*) str, strlen(str));
}
