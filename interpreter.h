#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "cpu-thread-state.h"

extern void interpret(cpu_thread_state_t* state, uint64_t max_instructions);

#endif /* _INTERPRETER_H_ */
