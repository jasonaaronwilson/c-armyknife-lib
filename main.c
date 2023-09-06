#include <stdlib.h>

#include "interpreter.h"
#include "opcodes.h"
#include "printer.h"

void add_sample_program(cpu_thread_state *state, uint64_t address);

/**
 * This is a simple main routine for the interpreter.
 */
int main(int argc, char **argv) {

  int number_of_int_registers = 4096;
  int number_of_fp_registers = 4096;
  uint64_t memory_amount = 4096 * 128;
  uint64_t start_pc = 0x0;

  cpu_thread_state *state = (cpu_thread_state *)(malloc(
      sizeof(cpu_thread_state) + number_of_int_registers * 8 +
      number_of_fp_registers * 8));

  state->pc = start_pc;
  state->num_integer_regs = number_of_int_registers;
  state->num_fp_regs = number_of_fp_registers;
  state->memory_start = malloc(memory_amount);
  state->memory_end = state->memory_start + memory_amount;

  add_sample_program(state, 0);

  // Single step until we have a BRK instruction
  while (1) {
    uint64_t before_pc = state->pc;
    print_instructions(state->memory_start, state->pc, 1);
    interpret(state, 1);
    print_registers(state, 16, 16);
    if ((before_pc == state->pc) && (state->memory_start[state->pc] == 0)) {
      break;
    }
  }

  exit(0);
}

void add_sample_program(cpu_thread_state *state, uint64_t address) {
  state->memory_start[address++] = SIMM;
  state->memory_start[address++] = GR2;
  state->memory_start[address++] = 42;

  state->memory_start[address++] = IMM;
  state->memory_start[address++] = GR1;
  state->memory_start[address++] = 42;

  state->memory_start[address++] = ADD;
  state->memory_start[address++] = GR3;
  state->memory_start[address++] = GR1;
  state->memory_start[address++] = GR1;

  state->memory_start[address++] = SUB;
  state->memory_start[address++] = GR3;
  state->memory_start[address++] = GR1;
  state->memory_start[address++] = GR1;

  state->memory_start[address++] = AND;
  state->memory_start[address++] = GR3;
  state->memory_start[address++] = GR2;
  state->memory_start[address++] = GR2;

  state->memory_start[address++] = XOR;
  state->memory_start[address++] = GR3;
  state->memory_start[address++] = GR2;
  state->memory_start[address++] = GR2;

  state->memory_start[address++] = OR;
  state->memory_start[address++] = GR3;
  state->memory_start[address++] = GR2;
  state->memory_start[address++] = GR0;

  state->memory_start[address++] = MOV;
  state->memory_start[address++] = GR1;
  state->memory_start[address++] = GR0;

  state->memory_start[address++] = BRK;
}
