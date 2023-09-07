#include "uleb128.h"

/**
 * Utility function to decode a ULEB128 value.
 */
unsigned_decode_result decodeULEB128(paged_memory *memory, uint64_t address) {
  uint64_t orig_address = address;
  uint64_t value = 0;
  unsigned shift = 0;
  do {
    /*
    if (address == end) {
      unsigned_decode_result result = {0, ERROR_INSUFFICIENT_INPUT};
      return result;
    }
    */
    uint64_t slice = load8(memory, address) & 0x7f;
    if ((shift >= 64 && slice != 0) || slice << shift >> shift != slice) {
      unsigned_decode_result result = {0, ERROR_TOO_BIG};
      return result;
    }
    value += slice << shift;
    shift += 7;
  } while (load8(memory, address++) >= 128);
  unsigned_decode_result result = {value, (unsigned)(address - orig_address)};
  return result;
}
