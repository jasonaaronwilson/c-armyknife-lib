#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint64_t elements[0];
} array_t;

extern array_t* make_array(uint32_t initial_capacity);
extern uint64_t array_length(array_t* arr);
extern uint64_t array_get(array_t* arr, uint64_t position);
extern array_t* array_add(array_t* arr, uint64_t element);

#endif /* _ARRAY_H */
