#line 1 "array.c"
/**
 * @file array.c
 *
 * This file contains a growable array of 64 bit values. Any function
 * that changes the length of an array may return a new pointer.
 *
 * (For scheme vectors, we actually will have a different
 * implemetation that doesn't have a capacity and knows it is holding
 * tagged_reference_t which are like 128 bits in size, not 64 bits.
 */

// ======================================================================
// This is block is extraced to array.h
// ======================================================================

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

typedef struct {
  type_t* element_type;
  uint32_t length;
  uint32_t capacity;
  uint64_t elements[0];
} array_t;

extern array_t* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t* arr);
extern uint64_t array_get(array_t* arr, uint64_t position);
extern array_t* array_add(array_t* arr, uint64_t element);

#endif /* _ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

/**
 * Make an array with the given initial_capacity.
 */
array_t* make_array(type_t* type, uint32_t initial_capacity) {
  int element_size = type->size;
  if (element_size < 0) {
    fatal_error(ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER);
  }
  array_t* result = (array_t*) (malloc_bytes(
      sizeof(array_t) + (element_size * initial_capacity)));
  result->element_type = type;
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
array_t* array_add(array_t* array, uint64_t element) {
  if (array->length < array->capacity) {
    array->elements[array->length] = element;
    array->length++;
    return array;
  } else {
    array_t* result = make_array(array->element_type, array->capacity * 2);
    for (int i = 0; i < array->length; i++) {
      array_add(result, array_get(array, i));
    }
    free_bytes(array);
    return result;
  }
}
