#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

#include "byte-array.h"

extern uint64_t byte_array_append_file_contents(byte_array_t* bytes,
                                                char* file_name);
extern void byte_array_write_file(byte_array_t* bytes, char* file_name);

#endif /* _IO_H_ */
