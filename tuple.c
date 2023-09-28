#line 2 "tuple.c"
/**
 * @file tuple.c
 *
 * Tuples are a low-level construct which are most useful for
 * implementing containers. The abstraction is like a C structure
 * except that "members" are referenced by position rather than name
 * and that the layout of tuples are determined by the tuple type not
 * the static declaration of the structure.
 */

// ======================================================================
// This section is extraced to tuple.h
// ======================================================================

#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <stdint.h>

typedef struct {
  __attribute__((aligned(8))) uint8_t data[0];
} tuple_t;

extern type_t* intern_tuple_type(int number_of_parameters, ...);
extern reference_t tuple_reference_of_element(reference_t tuple,
                                              uint64_t position);
extern reference_t tuple_reference_of_element_from_pointer(
    type_t* type, tuple_t* tuple_pointer, uint64_t position);

#endif /* _TUPLE_H_ */

// ======================================================================

#include <stdarg.h>
#include <stdlib.h>

#define TUPLE_ALIGN_OFFSET(offset) ((offset + 7) & ~7)

/**
 * Make a tuple type.
 */
type_t* intern_tuple_type(int number_of_parameters, ...) {
  type_t* result = (malloc_struct(type_t));

  byte_array_t* name = make_byte_array(32);
  name = byte_array_append_string(name, "tuple(");

  uint64_t size = 0;
  va_list args;
  va_start(args, number_of_parameters);
  for (int i = 0; (i < number_of_parameters); i++) {
    type_t* element_type = va_arg(args, type_t*);
    if (element_type->size <= 0) {
      fatal_error(ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER);
    }
    result->parameters[result->number_of_parameters++] = element_type;
    if (i > 0) {
      name = byte_array_append_string(name, ",");
    }
    name = byte_array_append_string(name, element_type->name);
    size += element_type->size;
    size = TUPLE_ALIGN_OFFSET(size);
  }
  va_end(args);

  result->size = size;
  name = byte_array_append_string(name, ")");
  result->name = byte_array_c_substring(name, 0, byte_array_length(name));
  free(name);

  // TODO(jawilson): compare_fn, append_fn, hash_fn
  // TODO(jawilson): actually intern the type!

  return result;
}

/**
 * Get a reference to a "member" element (from a reference to a tuple).
 */
reference_t tuple_reference_of_element(reference_t tuple_ref,
                                       uint64_t position) {
  // Make sure the reference is to a tuple?
  type_t* type = tuple_ref.underlying_type;
  tuple_t* tuple_pointer = tuple_ref.pointer;

  uint64_t offset = 0;
  for (int i = 0; (i < type->number_of_parameters); i++) {
    type_t* element_type = type->parameters[i];
    if (i == position) {
      return reference_of(element_type, &tuple_pointer->data[i]);
    }
    offset += element_type->size;
    offset = TUPLE_ALIGN_OFFSET(offset);
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
}
