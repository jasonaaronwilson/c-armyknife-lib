#ifndef _READER_H_
#define _READER_H_

#include "tagged-reference.h"

typedef struct {
  tagged_reference_t result;
  uint64_t end;
} read_expression_result_t;

extern read_expression_result_t read_expression(const char* str, uint64_t start);

#endif /* _READER_H_ */
