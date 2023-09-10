/**
 * @file interpreter.c
 *
 * This contains the interpreter for the Comet VM.
 */

#include <stdio.h>

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

#include "fatal-error.h"
#include "instruction-info.h"
#include "interpreter.h"
#include "opcodes.h"
#include "paged-memory.h"
#include "uleb128.h"

// ======================================================================
// Implementation Macros
// ======================================================================

#define AS_UINT64(x) ((uint64_t) x)

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

// ======================================================================
// Forward funtion declarations
// ======================================================================

int opcode_to_number_of_arguments(uint64_t opcode);

/**
 * This is the main loop of the VM interpreter.
 */
void interpret(cpu_thread_state_t* state, uint64_t max_instructions) {
  uint64_t num_instructions = 0;
  uint64_t pc = state->pc;
  paged_memory_t* memory = state->memory;
  uint64_t* ireg = &state->register_storage[0];

  while (1) {
    if (num_instructions >= max_instructions) {
      // TODO: copy back all "cached" constants.
      return;
    }

    // Make sure that register 0 is still zero after a write.
    ireg[0] = 0;

    uint64_t start_pc = pc;

    unsigned_decode_result_t opcode_result = decodeULEB128(memory, pc);
    if (opcode_result.size <= 0) {
      fatal_error(ERROR_OPCODE_DECODE_ERROR);
    }
    pc += opcode_result.size;
    uint64_t opcode = opcode_result.number;

    int num_args = opcode_to_number_of_arguments(opcode);

    uint64_t arg1 = 0;
    if (num_args > 0) {
      unsigned_decode_result_t arg_result = decodeULEB128(memory, pc);
      if (arg_result.size <= 0) {
        fatal_error(ERROR_ARG_DECODE_ERROR);
      }
      pc += arg_result.size;
      arg1 = arg_result.number;
    }

    uint64_t arg2 = 0;
    if (num_args > 1) {
      unsigned_decode_result_t arg_result = decodeULEB128(memory, pc);
      if (arg_result.size <= 0) {
        fatal_error(ERROR_ARG_DECODE_ERROR);
      }
      pc += arg_result.size;
      arg2 = arg_result.number;
    }

    uint64_t arg3 = 0;
    if (num_args > 2) {
      unsigned_decode_result_t arg_result = decodeULEB128(memory, pc);
      if (arg_result.size <= 0) {
        fatal_error(ERROR_ARG_DECODE_ERROR);
      }
      pc += arg_result.size;
      arg3 = arg_result.number;
    }

    // HERE MAX REG CHECK

    switch (opcode) {

    case BRK:
      return;

    case NOP:
      break;

    case BRZ: {
      uint64_t link_reg = arg1;
      uint64_t test_reg = arg2;
      uint64_t taken_address_reg = arg3;
      if (ireg[test_reg]) {
        ireg[link_reg] = pc; // link/return address
        pc = ireg[taken_address_reg];
      }
      break;
    }

    case IMM:
      ireg[arg1] = arg2;
      break;

    case SIMM:
      ireg[arg1] = -arg2;
      break;

    case PCIMM:
      ireg[arg1] = start_pc + arg2;
      break;

    case SPCIMM:
      ireg[arg1] = start_pc + ~arg2;
      break;

    case MOV:
      ireg[arg1] = ireg[arg2];
      break;

      //// MEMORY instructions are CURRENTLY alignment carefree and
      //// little endian.

    case LD_U8:
      ireg[arg1] = AS_UINT64(load8(memory, ireg[arg2]));
      break;

    case LD_U16:
      ireg[arg1] = AS_UINT64(load16(memory, ireg[arg2]));
      break;

    case LD_U32:
      ireg[arg1] = AS_UINT64(load32(memory, ireg[arg2]));
      break;

    case LD_U64:
      ireg[arg1] = load64(memory, ireg[arg2]);
      break;

    case LD_S8:
      ireg[arg1] = sign_extend_8(load8(memory, ireg[arg2]));
      break;

    case LD_S16:
      ireg[arg1] = sign_extend_16(load16(memory, ireg[arg2]));
      break;

    case LD_S32:
      ireg[arg1] = sign_extend_32(load32(memory, ireg[arg2]));
      break;

    case ST_8:
      store8(memory, ireg[arg1], ireg[arg2] & 0xff);
      break;

    case ST_16:
      store16(memory, ireg[arg1], ireg[arg2] & 0xffff);
      break;

    case ST_32:
      store32(memory, ireg[arg1], ireg[arg2] & 0xffffffff);
      break;

    case ST_64:
      store64(memory, ireg[arg1], ireg[arg2]);
      break;

    case CLZ:
      // TODO: backup implementation
      ireg[arg1] = __builtin_clz(ireg[arg2]);
      break;

    case CTZ:
      // TODO: backup implementation
      ireg[arg1] = __builtin_ctz(ireg[arg2]);
      break;

    case ADD:
      ireg[arg1] = ireg[arg2] + ireg[arg3];
      break;

    case SUB:
      ireg[arg1] = ireg[arg2] - ireg[arg3];
      break;

    case AND:
      ireg[arg1] = ireg[arg2] & ireg[arg3];
      break;

    case OR:
      ireg[arg1] = ireg[arg2] | ireg[arg3];
      break;

    case XOR:
      ireg[arg1] = ireg[arg2] ^ ireg[arg3];
      break;

    case SHL:
      ireg[arg1] = ireg[arg2] << ireg[arg3];
      break;

    case POPCNT:
      // TODO(jawilson) conditionalize and provide a backup
      ireg[arg1] = __builtin_popcount(ireg[arg2]);
      break;

    case MUL:
      ireg[arg1] = ireg[arg2] * ireg[arg3];
      break;

    default:
      fprintf(stderr, "opcode is %d\n", opcode);
      fatal_error(ERROR_UNIMLEMENTED_OPCODE);
      break;
    }

    state->pc = pc;

    num_instructions++;
  }
}

int opcode_to_number_of_arguments(uint64_t opcode) {
  instruction_info_t* info = find_instruction_info_by_opcode(opcode);
  return info->number_of_arguments;
}
