/**
 * This contains the bulk of the core logic of the VM interpreter.
 */
#include <stdint.h>
#include "opcodes.h"

typedef struct {
  // This is the program counter.
  uint64_t pc;

  // The number of integer registers that have been allocated to this 
  uint32_t num_integer_regs;

  uint32_t num_fp_regs;

  // This is the actual memory allocated for this virtual
  // machine. While cpu_state would be unique to each thread, this may
  // be shared by multiple threads.
  uint8_t memory;

  // Contains storage for the integer and floating point registers
  // (integer registers come first). Note: this must come last in the
  // cpu_state structure.
  uint64_t register_storage[0];
} cpu_thread_state;

/**
 * Forward declarations.
 */
typedef struct {
  uint64_t number;
  // Negative values mean an error occurred.
  int size;
} unsigned_decode_result;

typedef struct {
  uint64_t number;
  // Negative values mean an error occurred.
  int size;
} signed_decode_result;

int opcode_to_number_of_arguments(uint64_t opcode);

#define ERROR_INSUFFICIENT_INPUT -1
#define ERROR_TOO_BIG -2

extern unsigned_decode_result decodeULEB128(const uint8_t *p, const uint8_t *end);
extern signed_decode_result decodeSLEB128(const uint8_t *p, const uint8_t *end);

/**
 * This is the main loop of the VM interpreter.
 */
void interpret(cpu_thread_state* state, uint64_t max_instructions) {
  uint64_t num_instructions = 0;
  uint64_t pc = state->pc;

  while (1) {
    if (num_instructions == max_instructions) {
      return;
    }

    uint64_t opcode = 10;

    int num_args = opcode_to_number_of_arguments(opcode);
    
    uint64_t arg0 = 0;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;
    
    switch (opcode) {

    case BRK:
      // FIXME
      return;

    case NOP:
      break;

      

    }

    num_instructions++;
  }
}

int opcode_to_number_of_arguments(uint64_t opcode) {
  switch (opcode) {
  case BRK: return 0;
  case NOP: return 0;
  // BRZI src_address, jump_src_address, link_tgt
  case BRZI: return 3;
  case ADD: return 3;
  }

  return 0;
}

/// Utility function to decode a ULEB128 value.
unsigned_decode_result decodeULEB128(const uint8_t *p, const uint8_t *end) {
  const uint8_t *orig_p = p;
  uint64_t Value = 0;
  unsigned Shift = 0;
  do {
    if (p == end) {
      unsigned_decode_result result = { 0, ERROR_INSUFFICIENT_INPUT };
      return result;
    }
    uint64_t Slice = *p & 0x7f;
    if ((Shift >= 64 && Slice != 0) || Slice << Shift >> Shift != Slice) {
      unsigned_decode_result result = { 0, ERROR_TOO_BIG };
      return result;
    }
    Value += Slice << Shift;
    Shift += 7;
  } while (*p++ >= 128);
  unsigned_decode_result result = { Value, (unsigned)(p - orig_p) };
  return result;
}
 
/// Utility function to decode a SLEB128 value.
signed_decode_result decodeSLEB128(const uint8_t *p, const uint8_t *end) {
  const uint8_t *orig_p = p;
  int64_t Value = 0;
  unsigned Shift = 0;
  uint8_t Byte;
  do {
    if (p == end) {
      signed_decode_result result = { 0, ERROR_INSUFFICIENT_INPUT };
      return result;
    }
    Byte = *p;
    uint64_t Slice = Byte & 0x7f;
    // This handles decoding padded numbers, otherwise we might be
    // able to test very easily at the end of the loop.
    if ((Shift >= 64 && Slice != (Value < 0 ? 0x7f : 0x00)) ||
        (Shift == 63 && Slice != 0 && Slice != 0x7f)) {
      signed_decode_result result = { 0, ERROR_TOO_BIG };
      return result;
    }
    Value |= Slice << Shift;
    Shift += 7;
    ++p;
  } while (Byte >= 128);
  // Sign extend negative numbers if needed.
  if (Shift < 64 && (Byte & 0x40))
    Value |= (-1ULL) << Shift;
  signed_decode_result result = { Value, (p - orig_p) };
  return result;
}
