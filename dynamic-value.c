#line 2 "dynaic-value.c"

#ifndef _DYNAMIC_VALUE_H_
#define _DYNAMIC_VALUE_H_

#include <stdint.h>

/**
 * @file dynamic-value.c
 *
 * A dynamic value_t is similar to value_t except that they are tagged
 * so that they have a run-time "type" tag so we know what we have
 * dynamically at run-time. The big downside besides a little bit of
 * extra checking and tag manipulation if that they are about twice
 * the size of value_t.
 *
 * Dynamic value's are a good fit for something like s-expressions as
 * well as the values that may be manipulated inside of a dynamically
 * typed domain specific language.
 */

typedef enum {
  DYNAMIC_VALUE_UNKNOWN,
  DYNAMIC_VALUE_UINT64,
  DYNAMIC_VALUE_INT64,
  DYNAMIC_VALUE_DOUBLE,
  DYNAMIC_VALUE_STRING,
  DYNAMIC_VALUE_DYNAMIC_ARRAY,
} dynamic_value_tag_t;

struct dynamic_array_S;

typedef struct dynamic_value_S {
  dynamic_value_tag_t tag;
  union {
    uint64_t u64;
    int64_t i64;
    double dbl;
    char* str;
    struct dynamic_array_S dynamic_array;
  };
} dynamic_value_t;

#endif /* _DYNAMIC_VALUE_H_ */
