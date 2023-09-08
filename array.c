#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

array *make_array(uint32_t initial_capacity) {
  array *result = (array *)(malloc_bytes(8 * initial_capacity + sizeof(array)));
  result->capacity = initial_capacity;
  return result;
}

uint64_t array_length(array *arr) { return arr->length; }

uint64_t array_get(array *arr, uint64_t position) {
  if (position < arr->length) {
    return arr->elements[position];
  } else {
    fatal_error(ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS);
  }
}

array *array_add(array *arr, uint64_t element) {
  if (arr->length < arr->capacity) {
    arr->elements[arr->length] = element;
    arr->length++;
    return arr;
  } else {
    array *result = make_array(arr->capacity * 2);
    for (int i = 0; i < arr->length; i++) {
      array_add(result, array_get(arr, i));
    }
    free(arr);
    return result;
  }
}
