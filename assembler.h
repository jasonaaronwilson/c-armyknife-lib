#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdint.h>

// Returns the number of bytes assembled.
extern uint64_t assemble(char **statements, uint8_t *memory_start,
                         uint8_t *memory_end, uint64_t memory_start_address);

#endif /* _ASSEMBLER_H_ */
