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
} tag_t;

typedef struct {
  uint64_t data;
  uint64_t tag;
} tagged_reference_t;

#define NIL ((tagged_reference_t){TAG_NULL, 0})

inline void require_tag(tagged_reference_t reference, uint64_t tag) {
  if (reference.tag != tag) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
}

#endif /* _TAGGED_REFERENCE_H_ */
