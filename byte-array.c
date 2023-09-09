#include <stdlib.h>

#include "allocate.h"
#include "byte-array.h"
#include "fatal-error.h"

byte_array_t* make_byte_array(uint32_t initial_capacity) {
  byte_array_t* result
      = (byte_array_t*) (malloc_bytes(initial_capacity + sizeof(byte_array_t)));
  result->capacity = initial_capacity;
  return result;
}

uint64_t byte_array_length(byte_array_t* arr) { return arr->length; }

uint8_t byte_array_get(byte_array_t* arr, uint64_t position) {
  if (position < arr->length) {
    return arr->elements[position];
  } else {
    fatal_error(ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS);
  }
}

byte_array_t* byte_array_append_byte(byte_array_t* arr, uint8_t element) {
  if (arr->length < arr->capacity) {
    arr->elements[arr->length] = element;
    arr->length++;
    return arr;
  } else {
    byte_array_t* result = make_byte_array(arr->capacity * 2);
    for (int i = 0; i < arr->length; i++) {
      byte_array_append_byte(result, byte_array_get(arr, i));
    }
    free(arr);
    return result;
  }
}

byte_array_t* byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                                      uint64_t n_bytes) {
  for (int i = 0; i < n_bytes; i++) {
    arr = byte_array_append_byte(arr, bytes[n_bytes]);
  }
  return arr;
}
