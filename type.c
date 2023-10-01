#line 2 "type.c"
/**
 * @file type.c
 *
 * This is a runtime type so that containers can do dynamic type
 * checking since the C compiler isn't capable of type checking
 * automatically.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdalign.h>
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
  int alignment;
  uint64_t number_of_parameters;
  struct type_S* parameters[MAX_TYPE_PARAMETERS];
  compare_references_fn_t compare_fn;
  append_text_representation_fn_t append_fn;
  hash_reference_fn_t hash_fn;
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t uint8_type_constant;
extern type_t uint16_type_constant;
extern type_t uint32_type_constant;
extern type_t uint64_type_constant;
extern type_t char_ptr_type_constant;
extern type_t nil_type_constant;

static inline type_t* uint64_type() { return &uint64_type_constant; }
static inline type_t* uint32_type() { return &uint32_type_constant; }
static inline type_t* uint16_type() { return &uint16_type_constant; }
static inline type_t* uint8_type() { return &uint8_type_constant; }
static inline type_t* nil_type() { return &nil_type_constant; }

static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }

// This is used to indicate that a type is recursive in that it
// contains a pointer to the same type. When the type is finally
// interned then these are replaced with a pointer(xyz)
#define POINTER_TO_SELF_TYPE ((type_t*) 0x1)

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  WARN("intern_type is not actually doing interning");
  type_t* result = (type_t*) malloc_copy_of((uint8_t*) &type, sizeof(type));

  type_t* ptr_to_self_type = NULL;
  for (int i = 0; (i < result->number_of_parameters); i++) {
    if (result->parameters[i] == POINTER_TO_SELF_TYPE) {
      if (ptr_to_self_type == NULL) {
        ptr_to_self_type = (malloc_struct(type_t));
        ptr_to_self_type->name = string_append(type.name, "*");
        ptr_to_self_type->size = sizeof(char*);
        ptr_to_self_type->alignment = alignof(char*);
        WARN("POINTER_TO_SELF_TYPE only partially implemented");
      }
      result->parameters[i] = ptr_to_self_type;
    }
  }

  // TODO(jawilson): make sure size and alignment still hold because
  // of any POINTER_TO_SELF_TYPE adjustments that the caller
  // (intern_tuple_type) should have accounted for...
  return result;
}

#endif /* _TYPE_H_ */

#include <stdalign.h>

uint64_t hash_reference_bytes(reference_t reference) {
  // Actually call fasthash64!
  return 12;
}

uint64_t hash_string_reference(reference_t reference) {
  // Hash the underlying string (we won't know it's size like above).
  return 12;
}

int compare_string_references(reference_t ref_a, reference_t ref_b) {
  return strcmp(dereference_char_ptr(ref_a), dereference_char_ptr(ref_b));
}

type_t uint8_type_constant = {
    .name = "uint8_t",
    .size = sizeof(uint8_t),
    .alignment = alignof(uint8_t),
    .hash_fn = &hash_reference_bytes,
};

type_t uint16_type_constant = {
    .name = "uint16_t",
    .size = sizeof(uint16_t),
    .alignment = alignof(uint16_t),
    .hash_fn = &hash_reference_bytes,
};

type_t uint32_type_constant = {
    .name = "uint32_t",
    .size = sizeof(uint32_t),
    .alignment = alignof(uint32_t),
    .hash_fn = &hash_reference_bytes,
};

type_t uint64_type_constant = {
    .name = "uint64_t",
    .size = sizeof(uint64_t),
    .alignment = alignof(uint64_t),
    .hash_fn = &hash_reference_bytes,
};

type_t char_type_constant = {
    .name = "char",
    .size = sizeof(char),
    .alignment = alignof(char),
    .hash_fn = &hash_reference_bytes,
};

type_t double_type_constant = {
    .name = "double",
    .size = sizeof(double),
    .alignment = alignof(double),
    .hash_fn = &hash_reference_bytes,
};

type_t float_type_constant = {
    .name = "float",
    .size = sizeof(float),
    .alignment = alignof(float),
    .hash_fn = &hash_reference_bytes,
};

type_t char_ptr_type_constant = {
    .name = "char*",
    .size = sizeof(char*),
    .alignment = alignof(char*),
    .hash_fn = &hash_string_reference,
    .compare_fn = &compare_string_references,
};

type_t nil_type_constant = {
    .name = "nil",
    .size = 0,
    .alignment = 0,
    .hash_fn = &hash_reference_bytes,
};

// TODO(jawilson): more pointer types for the built in C types.
