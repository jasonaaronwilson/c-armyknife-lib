#line 1 "type.c"
/**
 * @file type.c
 *
 * This is a runtime type so that containers can do dynamic type
 * checking since the C compiler isn't capable of type checking
 * automatically.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>
#include <stdlib.h>

#define MAX_TYPE_PARAMETERS 8

struct byte_buffer_S;
struct reference_S;
struct type_S;

typedef int (*compare_references_fn_t)(struct reference_S a,
                                       struct reference_S b);
typedef void (*append_text_representation_fn_t)(struct byte_buffer_S* buffer,
                                                struct reference_S object);
typedef uint64_t (*hash_reference_fn_t)(struct reference_S object);

struct type_S {
  char* name;
  int size;
  compare_references_fn_t compare_fn;
  append_text_representation_fn_t append_fn;
  hash_reference_fn_t hash_fn;
  uint64_t number_of_parameters;
  struct type_S* parameters[MAX_TYPE_PARAMETERS];
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t uint8_type_constant;
extern type_t uint16_type_constant;
extern type_t uint32_type_constant;
extern type_t uint64_type_constant;
extern type_t char_ptr_type_constant;

static inline type_t* uint64_type() { return &uint64_type_constant; }
static inline type_t* uint32_type() { return &uint32_type_constant; }
static inline type_t* uint16_type() { return &uint16_type_constant; }
static inline type_t* uint8_type() { return &uint8_type_constant; }

static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  // HERE: malloc storage space and copy type to the heap.
  return NULL;
}

#endif /* _TYPE_H_ */

type_t uint8_type_constant = {
    .name = "uint8_t",
    .size = sizeof(uint8_t),
};

type_t uint16_type_constant = {
    .name = "uint16_t",
    .size = sizeof(uint16_t),
};

type_t uint32_type_constant = {
    .name = "uint32_t",
    .size = sizeof(uint32_t),
};

type_t uint64_type_constant = {
    .name = "uint64_t",
    .size = sizeof(uint64_t),
};

type_t char_type_constant = {
    .name = "char",
    .size = sizeof(char),
};

type_t double_type_constant = {
    .name = "double",
    .size = sizeof(double),
};

type_t float_type_constant = {
    .name = "float",
    .size = sizeof(float),
};

type_t char_ptr_type_constant = {
    .name = "char*",
    .size = sizeof(char*),
};

// TODO(jawilson): more pointer types for the built in C types.
