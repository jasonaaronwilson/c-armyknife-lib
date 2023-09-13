#ifndef _TAGGED_REFERENCE_H_
#define _TAGGED_REFERENCE_H_

#include <stdint.h>

#include "fatal-error.h"

/**
 * These are the tags that "type" a pointer (or 64 bits of data).
 */
typedef enum {
  TAG_NULL,
  TAG_BOOLEAN_T,
  TAG_UINT64_T,
  TAG_UNICODE_CODE_POINT,
  TAG_STRING,        // aka const char* C string
  TAG_SCHEME_SYMBOL, // also a const char* C string
  TAG_SINGLETON_T,   // also a const char* C string
  TAG_ERROR_T,
  TAG_PAIR_T,
  TAG_VECTOR_T,
  TAG_RECORD_T,
  TAG_BYTE_VECTOR_T,
  TAG_PRIMITIVE,
  TAG_CLOSURE_T,
  TAG_CPU_THREAD_STATE_T
} tag_t;

/**
 * This struct holds a dynamically typed value, either immediate data
 * like a boolean, uint64_t or double, or else a pointer to some data
 * on the heap (in which case a tagged_reference_t is like a tagged
 * pointer).
 *
 * Normally scheme implementations try to jam a value into the
 * machines register size (so 32bit in the older days but now a roomy
 * 64 bits on modern machines), however this comes at some cost in
 * code complexity. Instead we use a clean and portable definition and
 * suck up the fact that the storage cost twice as large in some
 * cases. With some work, it should be possible to use a more space
 * efficient format (perhaps by implementing "bignums" as say int128_t
 * pointers).
 */
typedef struct {
  uint64_t data;
  uint64_t tag;
} tagged_reference_t;

/**
 * This macro converts from the C only domain to the scheme value
 * domain.
 */
#define tagged_reference(tag, data) ((tagged_reference_t){(uint64_t) data, tag})

#define NIL tagged_reference(TAG_NULL, 0)

/**
 * This macro checks that the tagged_reference_t has the correct tag.
 */
static inline void require_tag(tagged_reference_t reference, uint64_t tag) {
  if (reference.tag != tag) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
}

/**
 * This macro untags a tagged_reference_t if it is a integer and
 * otherwise causes a fatal error. It returns the integer as a
 * uint64_t for operations that don't care about the sign or are
 * explicitly just treating the integer as a bunch of bits.
 */
static inline uint64_t untag_uint64_t(tagged_reference_t reference) {
  require_tag(reference, TAG_UINT64_T);
  return (uint64_t) reference.data;
}

/**
 * This macro untags a tagged_reference_t if it is a integer and
 * otherwise causes a fatal error.
 */
static inline int64_t untag_int64_t(tagged_reference_t reference) {
  // Maybe we should rename the tag?
  require_tag(reference, TAG_UINT64_T);
  return (int64_t) reference.data;
}

#endif /* _TAGGED_REFERENCE_H_ */
