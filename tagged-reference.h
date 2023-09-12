#ifndef _TAGGED_REFERENCE_H_
#define _TAGGED_REFERENCE_H_

#include <stdint.h>

#include "fatal-error.h"

typedef enum {
  TAG_NULL,
  TAG_PAIR_T,
  TAG_STRING,        // aka const char* C string
  TAG_READER_SYMBOL, // also a const char* C string
  TAG_UINT64_T,
  TAG_ERROR_T,
  TAG_BOOLEAN_T,
  TAG_PRIMITIVE
} tag_t;

typedef struct {
  uint64_t data;
  uint64_t tag;
} tagged_reference_t;

#define tagged_reference(tag, data) ((tagged_reference_t){(uint64_t) data, tag})

#define NIL tagged_reference(TAG_NULL, 0)

static inline void require_tag(tagged_reference_t reference, uint64_t tag) {
  if (reference.tag != tag) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
}

static inline uint64_t untag_uint64_t(tagged_reference_t reference) {
  require_tag(reference, TAG_UINT64_T);
  return (uint64_t) reference.data;
}

#endif /* _TAGGED_REFERENCE_H_ */
