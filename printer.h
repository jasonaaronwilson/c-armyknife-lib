#ifndef _PRINTER_H_
#define _PRINTER_H_

#include "cpu-thread-state.h"
#include "paged-memory.h"
#include <stdint.h>

extern uint64_t print_instructions(paged_memory *memory, uint64_t address,
                                   uint64_t number_of_instructions);

extern void print_registers(cpu_thread_state *state, int num_gr_registers,
                            int num_fp_registers);

extern void print_data(paged_memory *memory, uint64_t start_address,
                       uint64_t end_address);

#endif /* _PRINTER_H_ */
