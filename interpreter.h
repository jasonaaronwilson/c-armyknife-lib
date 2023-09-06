#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "cpu-thread-state.h"

extern void interpret(cpu_thread_state *state, uint64_t max_instructions);

#endif /* _INTERPRETER_H_ */
