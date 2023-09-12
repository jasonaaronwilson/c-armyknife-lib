#ifndef _READER_H_
#define _READER_H_

#include "tagged-reference.h"

extern tagged_reference_t read_expression(const char* str, uint64_t start);

#endif /* _READER_H_ */
