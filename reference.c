#line 2 "reference.c"
/**
 * C does not have parameterized types which makes generic containers
 * more difficult to work with.
 *
 * Therefore we we resort to dynamic (aka runtime) type checking of
 * the generic parameters.  types are interned so we can check a type
 * with a single comparison of a type_t pointer. Additionally, we pass
 * these structs by value which the C compiler can probably return in
 * registers.
 *
 * Containers which store elements of the same type only need to store
 * the type once so this doesn't impact the overall space occupied by
 * the collection.
 */

#ifndef _REFERENCE_H_
#define _REFERENCE_H_

#include <stdint.h>

struct reference_S {
  type_t* underlying_type;
  void* pointer;
};
typedef struct reference_S reference_t;

static inline reference_t reference_of(type_t* type, void* pointer) {
  reference_t result;
  result.underlying_type = type;
  result.pointer = pointer;
  return result;
}

static inline int compare_references(reference_t ref_a, reference_t ref_b) {
  if (ref_a.underlying_type != ref_b.underlying_type) {
    // An aribrary ordering based on the "random" layout of references
    // in memory.
    return (int) (((uint64_t) ref_a.underlying_type)
                  - ((uint64_t) ref_b.underlying_type));
  }
  return ref_a.underlying_type->compare_fn(ref_a, ref_b);
}

static inline reference_t reference_of_uint64(uint64_t* pointer) {
  reference_t result;
  result.underlying_type = uint64_type();
  result.pointer = pointer;
  return result;
}

static inline reference_t reference_of_char_ptr(char** pointer) {
  reference_t result;
  result.underlying_type = char_ptr_type();
  result.pointer = pointer;
  return result;
}

static inline uint64_t dereference_uint64(reference_t reference) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

static inline void write_to_uint64_reference(reference_t reference,
                                             uint64_t value) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint64_t*) reference.pointer) = value;
}

static inline uint64_t reference_to_uint32(reference_t reference) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint32_t*) reference.pointer);
}

static inline void write_to_uint32_reference(reference_t reference,
                                             uint32_t value) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint32_t*) reference.pointer) = value;
}

static inline uint64_t reference_to_uint16(reference_t reference) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint16_t*) reference.pointer);
}

static inline void write_to_uint16_reference(reference_t reference,
                                             uint16_t value) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint16_t*) reference.pointer) = value;
}

static inline uint64_t reference_to_uint8(reference_t reference) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint8_t*) reference.pointer);
}

static inline void write_to_uint8_reference(reference_t reference,
                                            uint8_t value) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint8_t*) reference.pointer) = value;
}

static inline char* dereference_char_ptr(reference_t reference) {
  if (reference.underlying_type != char_ptr_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return ((char*) reference.pointer);
}

static inline reference_t nil() { return reference_of(nil_type(), 0); }

#endif /* _REFERENCE_H_ */
