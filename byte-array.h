#ifndef _BYTE_ARRAY_H_
#define _BYTE_ARRAY_H_

#include <stdint.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
} byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);
extern uint64_t byte_array_length(byte_array_t* arr);
extern uint8_t byte_array_get(byte_array_t* arr, uint64_t position);
extern uint8_t* byte_array_substring(byte_array_t* arr, uint64_t start,
                                     uint64_t end);
extern byte_array_t* byte_array_append_byte(byte_array_t* arr, uint8_t byte);
extern byte_array_t* byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                                             uint64_t n_bytes);

#endif /* _BYTE_ARRAY_H_ */
