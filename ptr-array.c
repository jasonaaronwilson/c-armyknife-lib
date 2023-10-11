#line 2 "ptr-array.c"

/**
 * @file ptr-array.c
 *
 * This file contains a growable array of pointers which is
 * significantly easier to use than array.c though it is not
 * contiguous like
 */

#ifndef _PTR_ARRAY_H_
#define _PTR_ARRAY_H_

struct ptr_array_S {
  uint32_t length;
  uint32_t capacity;
  void** elements;
};

typedef struct ptr_array_S ptr_array_t;

extern ptr_array_t* make_ptr_array(uint32_t initial_capacity);
extern void* ptr_array_get(ptr_array_t* array, uint32_t index);
extern void ptr_array_add(ptr_array_t* array, void* element);

#endif /* _PTR_ARRAY_H_ */

ptr_array_t* make_ptr_array(uint32_t initial_capacity) {
  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  ptr_array_t* result = malloc_struct(ptr_array_t);
  result->capacity = initial_capacity;
  result->elements = (void**) malloc_bytes(sizeof(void*) * initial_capacity);

  return result;
}

void* ptr_array_get(ptr_array_t* array, uint32_t index) {
  if (index < array->length) {
    return array->elements[index];
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
}

void ptr_array_add(ptr_array_t* array, void* element) {
  if (array->length == array->capacity) {
    uint32_t new_capacity = array->capacity * 2;
    void** new_elements = (void**) (malloc_bytes(sizeof(void*) * new_capacity));
    for (int i = 0; i < array->capacity; i++) {
      new_elements[i] = array->elements[i];
    }
    array->capacity = new_capacity;
    free(array->elements);
    array->elements = new_elements;
    ptr_array_add(array, element);
    return;
  }
  array->elements[(array->length)++] = element;
}
