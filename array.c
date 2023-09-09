/**
 * @file array.c
 *
 * This file contains a growable array of 64 bit values. Any function
 * that changes the length of an array may return a new pointer.
 */

#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

/**
 * Make an array with the given initial_capacity.
 */
array_t* make_array(uint32_t initial_capacity) {
  array_t* result
      = (array_t*) (malloc_bytes(8 * initial_capacity + sizeof(array_t)));
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of actual entries in an array.
 */
uint64_t array_length(array_t* arr) { return arr->length; }

/**
 * Get the nth element from an array.
 */
uint64_t array_get(array_t* arr, uint64_t position) {
  if (position < arr->length) {
    return arr->elements[position];
  } else {
    fatal_error(ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Add an element to the end of an array.
 */
array_t* array_add(array_t* arr, uint64_t element) {
  if (arr->length < arr->capacity) {
    arr->elements[arr->length] = element;
    arr->length++;
    return arr;
  } else {
    array_t* result = make_array(arr->capacity * 2);
    for (int i = 0; i < arr->length; i++) {
      array_add(result, array_get(arr, i));
    }
    free(arr);
    return result;
  }
}
