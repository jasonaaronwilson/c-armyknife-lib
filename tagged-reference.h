#ifndef _TAGGED_REFERENCE_H_
#define _TAGGED_REFERENCE_H_

#include <stdint.h>

typedef enum {
  TAG_NULL,
  TAG_TAGGED_PAIR_T,
  TAG_STRING, // aka const char* C string
  TAG_UINT64_T,
  TAG_ERROR_T,
} tag_t;

typedef struct {
  uint64_t data;
  uint64_t tag;
} tagged_reference_t;

#define NIL ((tagged_reference_t){TAG_NULL, 0})

#endif /* _TAGGED_REFERENCE_H_ */
