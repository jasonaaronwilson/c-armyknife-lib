/**
 * This contains the bulk of the interpreter logic for the Comet VM.
 */

// ======================================================================
// Compile Time Parameters
// ======================================================================

// TODO(jawilson): do this for integer and floating point registers
#ifndef CHECK_MAX_REGISTERS
#define CHECK_MAX_REGISTERS 1
#endif

// TODO(jawilson): add alignment check for 16, 32, and 64 bit loads.

#include <stdint.h>
#include <stdlib.h>
#include "opcodes.h"
#include "interpreter.h"

/**
 * Forward declarations.
 */
typedef struct {
  uint64_t number;
  // Negative values mean an error occurred.
  int size;
} unsigned_decode_result;

// ======================================================================
// Implementation Macros
// ======================================================================

#define AS_UINT64(x) ((uint64_t)x)

// ======================================================================
// VM "Inlined" Functions
// ======================================================================

uint64_t sign_extend_8(uint64_t value) {
  return ((int64_t)(value << 56)) >> 56;
}

uint64_t sign_extend_16(uint64_t value) {
  return ((int64_t)(value << 48)) >> 48;
}

uint64_t sign_extend_32(uint64_t value) {
  return ((int64_t)(value << 32)) >> 32;
}


int opcode_to_number_of_arguments(uint64_t opcode);

#define ERROR_INSUFFICIENT_INPUT -1
#define ERROR_TOO_BIG -2

extern unsigned_decode_result decodeULEB128(const uint8_t *p,
                                            const uint8_t *end);

/**
 * This is the main loop of the VM interpreter.
 */
void interpret(cpu_thread_state *state, uint64_t max_instructions) {
  uint64_t num_instructions = 0;
  uint64_t pc = state->pc;
  uint8_t *memory = state->memory_start;
  uint8_t *end = state->memory_end;
  uint64_t *ireg = &state->register_storage[0];

  while (1) {
    if (num_instructions >= max_instructions) {
      // TODO: copy back all "cached" constants.
      return;
    }

    // Make sure that register 0 is still zero after a write.
    ireg[0] = 0;

    uint64_t start_pc = pc;

    unsigned_decode_result opcode_result = decodeULEB128(memory + pc, end);
    if (opcode_result.size <= 0) {
      exit(1);
    }
    pc += opcode_result.size;
    uint64_t opcode = opcode_result.number;

    int num_args = opcode_to_number_of_arguments(opcode);

    uint64_t arg1 = 0;
    if (num_args > 0) {
      unsigned_decode_result arg_result = decodeULEB128(memory + pc, end);
      if (arg_result.size <= 0) {
        exit(1);
      }
      pc += arg_result.size;
      arg1 = arg_result.number;
    }

    uint64_t arg2 = 0;
    if (num_args > 1) {
      unsigned_decode_result arg_result = decodeULEB128(memory + pc, end);
      if (arg_result.size <= 0) {
        exit(1);
      }
      pc += arg_result.size;
      arg2 = arg_result.number;
    }

    uint64_t arg3 = 0;
    if (num_args > 2) {
      unsigned_decode_result arg_result = decodeULEB128(memory + pc, end);
      if (arg_result.size <= 0) {
        exit(1);
      }
      pc += arg_result.size;
      arg3 = arg_result.number;
    }

    // HERE MAX REG CHECK

    switch (opcode) {

    case BRK:
      // FIXME
      pc = -1;
      return;

    case NOP:
      break;

    case BRZ:
      if (!(ireg[arg1])) {
        ireg[arg3] = pc; // link/return address
        pc = ireg[arg2];
      }
      break;

    case IMM:
      ireg[arg1] = arg2;
      break;

    case NIMM:
      ireg[arg1] = ~arg2;
      break;

    case PCIMM:
      ireg[arg1] = start_pc + arg2;
      break;

    case NPCIMM:
      ireg[arg1] = start_pc + ~arg2;
      break;

    case MOV:
      ireg[arg2] = ireg[arg1];
      break;

      //// MEMORY instructions are CURRENTLY alignment carefree and
      //// little endian.

    case LD_U8:
      ireg[arg2] = AS_UINT64(memory[ireg[arg1]]);
      break;

    case LD_U16:
      // TODO check alignment!
      ireg[arg2] = AS_UINT64(memory[ireg[arg1]]) |
                   AS_UINT64(memory[ireg[arg1 + 1]]) << 8;
      break;

    case LD_U32:
      // TODO check alignment!
      ireg[arg2] = AS_UINT64(memory[ireg[arg1]]) |
                   AS_UINT64(memory[ireg[arg1 + 1]]) << 8 |
                   AS_UINT64(memory[ireg[arg1 + 2]]) << 16 |
                   AS_UINT64(memory[ireg[arg1 + 3]]) << 24;
      break;

    case LD_U64:
      // TODO check alignment!
      ireg[arg2] = AS_UINT64(memory[ireg[arg1]]) |
                   AS_UINT64(memory[ireg[arg1 + 1]]) << 8 |
                   AS_UINT64(memory[ireg[arg1 + 2]]) << 16 |
                   AS_UINT64(memory[ireg[arg1 + 3]]) << 24 |
                   AS_UINT64(memory[ireg[arg1 + 4]]) << 32 |
                   AS_UINT64(memory[ireg[arg1 + 5]]) << 40 |
                   AS_UINT64(memory[ireg[arg1 + 6]]) << 48 |
                   AS_UINT64(memory[ireg[arg1 + 7]]) << 56;
      break;

    case LD_S8:
      ireg[arg2] = sign_extend_8(AS_UINT64(memory[ireg[arg1]]));
      break;

    case LD_S16:
      // TODO check alignment!
      ireg[arg2] = sign_extend_16(AS_UINT64(memory[ireg[arg1]]) |
                                  AS_UINT64(memory[ireg[arg1 + 1]]) << 8);
      break;

    case LD_S32:
      // TODO check alignment!
      ireg[arg2] = sign_extend_32(AS_UINT64(memory[ireg[arg1]]) |
                                  AS_UINT64(memory[ireg[arg1 + 1]]) << 8 |
                                  AS_UINT64(memory[ireg[arg1 + 2]]) << 16 |
                                  AS_UINT64(memory[ireg[arg1 + 3]]) << 24);
      break;

    case ST_8:
      memory[arg2] = (ireg[arg1] & 0xff);
      break;

    case ST_16:
      memory[arg2] = (ireg[arg1]) & 0xff;
      memory[arg2+1] = (ireg[arg1] >> 8) & 0xff;
      break;

    case ST_32:
      memory[arg2] = (ireg[arg1]) & 0xff;
      memory[arg2+1] = (ireg[arg1] >> 8) & 0xff;
      memory[arg2+2] = (ireg[arg1] >> 16) & 0xff;
      memory[arg2+3] = (ireg[arg1] >> 24) & 0xff;
      break;

    case ST_64:
      memory[arg2] = (ireg[arg1]) & 0xff;
      memory[arg2+1] = (ireg[arg1] >> 8) & 0xff;
      memory[arg2+2] = (ireg[arg1] >> 16) & 0xff;
      memory[arg2+3] = (ireg[arg1] >> 24) & 0xff;
      memory[arg2+4] = (ireg[arg1] >> 32) & 0xff;
      memory[arg2+5] = (ireg[arg1] >> 40) & 0xff;
      memory[arg2+6] = (ireg[arg1] >> 48) & 0xff;
      memory[arg2+7] = (ireg[arg1] >> 56) & 0xff;
      break;

    case CLZ:
      // TODO: backup implementation
      ireg[arg2] = __builtin_clz(ireg[arg1]);
      break;

    case CTZ:
      // TODO: backup implementation
      ireg[arg2] = __builtin_ctz(ireg[arg1]);
      break;

    case ADD:
      ireg[arg3] = ireg[arg1] + ireg[arg2];
      break;

    case SUB:
      ireg[arg3] = ireg[arg1] - ireg[arg2];
      break;

    case AND:
      ireg[arg3] = ireg[arg1] & ireg[arg2];
      break;

    case OR:
      ireg[arg3] = ireg[arg1] | ireg[arg2];
      break;

    case XOR:
      ireg[arg3] = ireg[arg1] ^ ireg[arg2];
      break;

    case SHL:
      ireg[arg3] = ireg[arg1] << ireg[arg2];
      break;

    case POPCNT:
      // TODO(jawilson) conditionalize and provide a backup
      ireg[arg3] = __builtin_popcount(ireg[arg1]);
      break;
      
    default:
      // printf?
      exit(1);
      break;
    }

    num_instructions++;
  }
}

int opcode_to_number_of_arguments(uint64_t opcode) {
  switch (opcode) {

  case BRK:
    return 0;

  case NOP:
    return 0;

  // BRZ src_address, jump_src_address, link_tgt
  case BRZ:
    return 3;

  case ADD:
  case AND:
  case OR:
  case SUB:
  case XOR:
  case MUL:
    return 3;

  case POPCNT:
  case CTZ:
  case CLZ:
    return 2;
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
      unsigned_decode_result result = {0, ERROR_INSUFFICIENT_INPUT};
      return result;
    }
    uint64_t Slice = *p & 0x7f;
    if ((Shift >= 64 && Slice != 0) || Slice << Shift >> Shift != Slice) {
      unsigned_decode_result result = {0, ERROR_TOO_BIG};
      return result;
    }
    Value += Slice << Shift;
    Shift += 7;
  } while (*p++ >= 128);
  unsigned_decode_result result = {Value, (unsigned)(p - orig_p)};
  return result;
}
