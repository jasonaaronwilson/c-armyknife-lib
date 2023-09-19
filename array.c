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
  uint8_t data[0];
} array_t;

extern array_t* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t* arr);
extern reference_t array_get_reference(array_t* arr, uint64_t position);
__attribute__((warn_unused_result)) extern array_t*
    array_add(array_t* arr, reference_t element);

#endif /* _ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

static inline void* array_address_of_element(array_t* array,
                                             uint64_t position) {
  return &(array->data[position * array->element_type->size]);
}

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
reference_t array_get(array_t* array, uint64_t position) {
  if (position < array->length) {
    return reference_of(array->element_type,
                        array_address_of_element(array, position));
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Add an element to the end of an array.
 */
__attribute__((warn_unused_result)) array_t* array_add(array_t* array,
                                                       reference_t reference) {
  if (reference.underlying_type != array->element_type) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  int size = array->element_type->size;
  if (array->length < array->capacity) {
    memcpy(array_address_of_element(array, array->length), reference.pointer,
           size);
    array->length++;
    return array;
  } else {
    array_t* result = make_array(array->element_type, array->capacity * 2);
    memcpy(array_address_of_element(result, 0),
           array_address_of_element(array, 0), size * array->length);
    free_bytes(array);
    return result;
  }
}
