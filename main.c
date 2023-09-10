#include <stdlib.h>

#include "allocate.h"
#include "debug-repl.h"
#include "interpreter.h"
#include "opcodes.h"
#include "paged-memory.h"
#include "printer.h"

void add_sample_program(cpu_thread_state_t* state, uint64_t address);

/**
 * This is a simple main routine for the interpreter.
 */
int main(int argc, char** argv) {

  int number_of_int_registers = 4096;
  int number_of_fp_registers = 4096;
  uint64_t memory_amount = 4096 * 128;
  uint64_t start_pc = PAGE_SIZE;

  cpu_thread_state_t* state = (cpu_thread_state_t*) (malloc_bytes(
      sizeof(cpu_thread_state_t) + number_of_int_registers * 8
      + number_of_fp_registers * 8));

  state->pc = start_pc;
  state->num_integer_regs = number_of_int_registers;
  state->num_fp_regs = number_of_fp_registers;
  // Don't put anything on the first page so that random reads and
  // writes will fail to addresses near "NULL".
  state->memory = allocate_page(0, PAGE_SIZE);

  // add_sample_program(state, PAGE_SIZE);

  char* debug = getenv("COMET_VM_DEBUG");
  if (debug != NULL || 1) {
    debug_repl(state);
    exit(0);
  }

  // Single step until we have a BRK instruction
  while (1) {
    uint64_t before_pc = state->pc;
    print_instructions(state->memory, state->pc, 1);
    interpret(state, 1);
    print_registers(state, 16, 16);
    // might need is_mapped...
    if ((before_pc == state->pc) && (load8(state->memory, state->pc) == 0)) {
      break;
    }
  }

  debug_repl(state);
  exit(0);
}

#if 0

// If really necessary, you can easily put some code into memory like
// so...

void add_sample_program(cpu_thread_state_t* state, uint64_t address) {
  store8(state->memory, address++, OPCODE_IMM);
  store8(state->memory, address++, R1);
  store8(state->memory, address++, 42);

  store8(state->memory, address++, BRK);
}
#endif
