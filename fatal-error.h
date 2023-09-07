#ifndef _FATAL_ERROR_
#define _FATAL_ERROR_

#define ERROR_UKNOWN 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_MEMORY_READ 2
#define ERROR_MEMORY_WRITE 3
#define ERROR_OPCODE_UNKNOWN 4
#define ERROR_OPCODE_DECODE_ERROR 5
#define ERROR_ARG_DECODE_ERROR 6
#define ERROR_UNIMLEMENTED_OPCODE 7

extern _Noreturn void fatal_error(int error_code);

#endif /* _FATAL_ERROR_ */
