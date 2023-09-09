#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint64_t elements[0];
} array;

extern array* make_array(uint32_t initial_capacity);
extern uint64_t array_length(array* arr);
extern uint64_t array_get(array* arr, uint64_t position);
extern array* array_add(array* arr, uint64_t element);

#endif /* _ARRAY_H */
