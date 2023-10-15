#line 2 "value-array.c"

/**
 * @file ptr-array.c
 *
 * This file contains a growable array of values/pointers.
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
extern void value_array_push(value_array_t* array, value_t element);
extern value_t value_array_pop(value_array_t* array);
extern void value_array_insert_at(value_array_t* array, uint32_t position,
                                  value_t element);
extern value_t value_array_delete_at(value_array_t* array, uint32_t position);

#endif /* _VALUE_ARRAY_H_ */

/**
 * Make a value array with the given initial capacity (which must be >
 * 0). When the array runs out of capacity because of calls to add,
 * push, etc., then the backing array is automatically doubled in size
 * (this may change to a different fraction for "large arrays"
 * (greater than say 250 elements) in the future to save space).
 *
 * If the initial_capacity is zero or if malloc() can't allocate
 * everything, then a fatal_error() occurs.
 */
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

void value_array_ensure_capacity(value_array_t* array,
                                 uint32_t required_capacity) {
  if (array->capacity < required_capacity) {
    uint32_t new_capacity = array->capacity * 2;
    if (new_capacity < required_capacity) {
      new_capacity = required_capacity;
    }
    value_t* new_elements
        = (value_t*) (malloc_bytes(sizeof(value_t) * new_capacity));
    for (int i = 0; i < array->length; i++) {
      new_elements[i] = array->elements[i];
    }
    array->capacity = new_capacity;
    free(array->elements);
    array->elements = new_elements;
    return;
  }
}

/**
 * Get the value stored at index `index`. If the index is outside of
 * the range of valid elements, then a fatal_error is signaled.
 */
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

/**
 * Add an element to the end of an array. If more space is required
 * then the backing array is automatically resized. This resizing
 * means that a fatal_error() may occur if malloc() can not satisfy the
 * allocation request.
 */
void value_array_add(value_array_t* array, value_t element) {
  value_array_ensure_capacity(array, array->length + 1);
  array->elements[(array->length)++] = element;
}

/**
 * This is a synonym for value_array_add which serves to make it more
 * obvious that the array is actually being used like a stack.
 */
void value_array_push(value_array_t* array, value_t element) {
  value_array_add(array, element);
}

/**
 * Returns the last element of the array (typically added via push).
 *
 * If the array is currently empty, then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 */
value_t value_array_pop(value_array_t* array) {
  if (array->length == 0) {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
  uint32_t last_index = array->length - 1;
  value_t result = value_array_get(array, last_index);
  array->length--;
  return result;
}

/**
 * Insert an element into some existing position in the array (or at
 * the end if position == the current array length).
 *
 * This operation is not efficient for large arrays as potentially the
 * entire array must be moved to new locations (there are
 * data-structures like trees that can make this more efficient though
 * such a data-structure isn't in this library yet).
 *
 * If the position is > than the length length, then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 *
 * If memory allocation is required and malloc() fails, then
 * fatal_error(ERROR_MEMORY_ALLOCATION) is called.
 */
void value_array_insert_at(value_array_t* array, uint32_t position,
                           value_t element) {
  if (position == array->length) {
    value_array_add(array, element);
    return;
  }
  if (position > array->length) {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
    return;
  }

  value_array_ensure_capacity(array, array->length + 1);
  for (uint32_t i = position; i < array->length; i++) {
    array->elements[i + 1] = array->elements[i];
  }
  array->length++;
  array->elements[position] = element;
}

/**
 * Deletes the element at the given position (and return it so that it
 * can potentially be freed by the caller).
 *
 * If the position doesn't point to a valid element then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 */
value_t value_array_delete_at(value_array_t* array, uint32_t position) {
  value_t result = value_array_get(array, position);
  if (array->length > 1) {
    for (int i = 0; i < array->length - 1; i++) {
      array->elements[i] = array->elements[i + 1];
    }
  }
  array->length--;
  return result;
}
