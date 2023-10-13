#line 2 "value-array.c"

/**
 * @file ptr-array.c
 *
 * This file contains a growable array of pointers which is
 * significantly easier to use than array.c though it is not
 * contiguous.
 */

#ifndef _VALUE_ARRAY_H_
#define _VALUE_ARRAY_H_

struct value_array_S {
  uint32_t length;
  uint32_t capacity;
  value_t* elements;
};

typedef struct value_array_S value_array_t;

extern value_array_t* make_value_array(uint32_t initial_capacity);
extern value_t value_array_get(value_array_t* array, uint32_t index);
extern void value_array_add(value_array_t* array, value_t element);

#endif /* _VALUE_ARRAY_H_ */

value_array_t* make_value_array(uint32_t initial_capacity) {
  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  value_array_t* result = malloc_struct(value_array_t);
  result->capacity = initial_capacity;
  result->elements
      = (value_t*) malloc_bytes(sizeof(value_t) * initial_capacity);

  return result;
}

value_t value_array_get(value_array_t* array, uint32_t index) {
  if (index < array->length) {
    return array->elements[index];
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
#ifdef __TINYC__
  /* gcc and clang know fatal_error is _Noreturn but tcc doesn't */
  return (value_t){.u64 = 0};
#endif
}

void value_array_add(value_array_t* array, value_t element) {
  if (array->length == array->capacity) {
    uint32_t new_capacity = array->capacity * 2;
    value_t* new_elements
        = (value_t*) (malloc_bytes(sizeof(value_t) * new_capacity));
    for (int i = 0; i < array->capacity; i++) {
      new_elements[i] = array->elements[i];
    }
    array->capacity = new_capacity;
    free(array->elements);
    array->elements = new_elements;
    value_array_add(array, element);
    return;
  }
  array->elements[(array->length)++] = element;
}
