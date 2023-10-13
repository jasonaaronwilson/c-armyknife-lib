#line 2 "value.c"

#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

typedef struct {
  union {
    uint64_t u64;
    uint64_t i64;
    char* str;
    void* ptr;
    void* dbl;
    value_t val;
  };
  boolean_t found;
} value_result_t;

#define u64_to_value(x) ((value_t){.u64 = x})
#define i64_to_value(x) ((value_t){.i64 = x})
#define str_to_value(x) ((value_t){.str = x})
#define ptr_to_value(x) ((value_t){.ptr = x})
#define dbl_to_value(x) ((value_t){.dbl = x})

#endif /* _VALUE_H_ */
