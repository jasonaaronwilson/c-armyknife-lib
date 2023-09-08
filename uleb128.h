#ifndef _ULEB128_H_
#define _ULEB128_H_

#include <stdint.h>

#include "paged-memory.h"

#define ERROR_INSUFFICIENT_INPUT -1
#define ERROR_TOO_BIG -2

/**
 * Forward declarations.
 */
typedef struct {
  uint64_t number;
  // Negative values mean an error occurred.
  int size;
} unsigned_decode_result;

extern unsigned_decode_result decodeULEB128(paged_memory *memory,
                                            uint64_t address);
extern unsigned encodeULEB128(paged_memory *memory,
                              uint64_t address,
                              uint64_t value);

#endif /* _ULEB128_H_ */
