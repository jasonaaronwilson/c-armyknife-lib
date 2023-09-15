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
extern uint64_t byte_array_length(byte_array_t* arr);
extern uint8_t byte_array_get(byte_array_t* arr, uint64_t position);
extern char* byte_array_c_substring(byte_array_t* arr, uint64_t start,
                                    uint64_t end);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* arr, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* arr, const char* str);

#endif /* _BYTE_ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "byte-array.h"
#include "ct-assert.h"
#include "fatal-error.h"

byte_array_t* make_byte_array(uint32_t initial_capacity) {

  // We make the assumption that casting (char*) to (uint8_t*) and
  // vice-versa is completely reasonable which it is on all modern
  // architecures.
  ct_assert(sizeof(char) == 1);

  byte_array_t* result
      = (byte_array_t*) (malloc_bytes(initial_capacity + sizeof(byte_array_t)));
  result->capacity = initial_capacity;
  return result;
}

uint64_t byte_array_length(byte_array_t* array) { return array->length; }

uint8_t byte_array_get(byte_array_t* arr, uint64_t position) {
  if (position < arr->length) {
    return arr->elements[position];
  } else {
    fatal_error(ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Extract a newly allocated string contain the bytes from start to
 * end (appending a zero byte to make sure it's a legal C string).
 */
char* byte_array_c_substring(byte_array_t* arr, uint64_t start, uint64_t end) {
  // Add one extra byte for a NUL string terminator byte
  char* result = (char*) (malloc_bytes(end - start + 1));
  for (int i = start; i < end; i++) {
    result[i - start] = arr->elements[i];
  }
  return result;
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_byte(byte_array_t* arr, uint8_t element) {
  if (arr->length < arr->capacity) {
    arr->elements[arr->length] = element;
    arr->length++;
    return arr;
  } else {
    byte_array_t* result = make_byte_array(arr->capacity * 2);
    for (int i = 0; i < arr->length; i++) {
      result = byte_array_append_byte(result, byte_array_get(arr, i));
    }
    free_bytes(arr);

    return result;
  }
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                            uint64_t n_bytes) {
  for (int i = 0; i < n_bytes; i++) {
    arr = byte_array_append_byte(arr, bytes[i]);
  }
  return arr;
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_string(byte_array_t* arr, const char* str) {
  return byte_array_append_bytes(arr, (uint8_t*) str, strlen(str));
}
