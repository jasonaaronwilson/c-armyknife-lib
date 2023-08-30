#ifndef _INTERPRETER_H_

typedef struct {
  // This is the program counter.
  uint64_t pc;

  // The number of integer registers that have been allocated to this
  uint32_t num_integer_regs;

  uint32_t num_fp_regs;

  // This is the actual memory allocated for this virtual
  // machine. While cpu_state would be unique to each thread, this may
  // be shared by multiple threads.
  uint8_t *memory_start;
  uint8_t *memory_end;

  // Contains storage for the integer and floating point registers
  // (integer registers come first). Note: this must come last in the
  // cpu_state structure.
  uint64_t register_storage[0];
} cpu_thread_state;

#endif /* _INTERPRETER_H_ */

