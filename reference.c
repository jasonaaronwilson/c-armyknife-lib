#line 1 "reference.c"
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

static inline uint64_t reference_to_uint64(reference_t reference) {
  if (reference.underlying_type != &uint64_type_constant) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

#endif /* _REFERENCE_H_ */
