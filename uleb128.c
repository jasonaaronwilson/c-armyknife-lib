#include "uleb128.h"
#include "paged-memory.h"

/**
 * Utility function to decode a ULEB128 value.
 */
unsigned_decode_result decodeULEB128(paged_memory* memory, uint64_t address) {
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
  unsigned_decode_result result = {value, (unsigned) (address - orig_address)};
  return result;
}

unsigned encodeULEB128(paged_memory* memory, uint64_t address, uint64_t value) {
  uint64_t orig_address = address;
  do {
    uint8_t byte = value & 0x7f;
    value >>= 7;
    if (value != 0) {
      byte |= 0x80; // Mark this byte to show that more bytes will follow.
    }
    store8(memory, address++, byte);
  } while (value != 0);

  return (unsigned) (address - orig_address);
}
