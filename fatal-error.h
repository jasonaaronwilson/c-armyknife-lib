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
#define ERROR_TOKEN_LIST_GET 8
#define ERROR_EXPECTED_GENERAL_REGISTER 9
#define ERROR_EXPECTED_FLOATING_REGISTER 10
#define ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS 11
#define ERROR_UNKNOWN_ASSEMBLER_DIRECTIVE 12

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_ */
