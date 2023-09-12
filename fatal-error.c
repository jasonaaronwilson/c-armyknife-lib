/**
 * @file: fatal-error.c
 *
 * The intent is that everything but a normal program exit will end up
 * here. (Currently, we don't catch any signals so this is definitely
 * not true.)
 *
 * In this case C's macros are paying off as the file and line number
 * are easy to obtain.
 *
 * TODO(jawilson): command line flag to be quieter...
 */

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "fatal-error.h"

void print_fatal_error_banner();
void print_backtrace();
void print_error_code_name(int error_code);

void _Noreturn fatal_error_impl(char* file, int line, int error_code) {
  print_fatal_error_banner();
  print_backtrace();
  fprintf(stderr, "%s:%d: FATAL ERROR %d", file, line, error_code);
  print_error_code_name(error_code);

  fprintf(stderr, "Exiting...\n");
  exit(-(error_code + 100));
}

void print_fatal_error_banner() {
  // As the first thing we print, also responsible for at least one
  // newline to start a new line if we may not be at one.
  fprintf(stderr, "\n========== FATAL_ERROR ==========\n");
}

void print_backtrace() {
#ifndef NO_VM_BACKTRACE_ON_FATAL_ERROR
  do {
    void* array[10];
    int size = backtrace(array, 10);
    char** strings = backtrace_symbols(array, size);

    // Print the backtrace
    for (int i = 0; i < size; i++) {
      printf("#%d %s\n", i, strings[i]);
    }
  } while (0);
#endif /* NO_VM_BACKTRACE_ON_FATAL_ERROR */
}

const char* fatal_error_code_to_string(int error_code) {
  switch (error_code) {
  case ERROR_UKNOWN:
    return "ERROR_UKNOWN";
  case ERROR_MEMORY_ALLOCATION:
    return "ERROR_MEMORY_ALLOCATION";
  case ERROR_MEMORY_READ:
    return "ERROR_MEMORY_READ";
  case ERROR_MEMORY_WRITE:
    return "ERROR_MEMORY_WRITE";
  case ERROR_OPCODE_UNKNOWN:
    return "ERROR_OPCODE_UNKNOWN";
  case ERROR_OPCODE_DECODE_ERROR:
    return "ERROR_OPCODE_DECODE_ERROR";
  case ERROR_ARG_DECODE_ERROR:
    return "ERROR_ARG_DECODE_ERROR";
  case ERROR_UNIMLEMENTED_OPCODE:
    return "ERROR_UNIMLEMENTED_OPCODE";
  case ERROR_TOKEN_LIST_GET:
    return "ERROR_TOKEN_LIST_GET";
  case ERROR_EXPECTED_GENERAL_REGISTER:
    return "ERROR_EXPECTED_GENERAL_REGISTER";
  case ERROR_EXPECTED_FLOATING_REGISTER:
    return "ERROR_EXPECTED_FLOATING_REGISTER";
  case ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS:
    return "ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS";
  case ERROR_UNKNOWN_ASSEMBLER_DIRECTIVE:
    return "ERROR_UNKNOWN_ASSEMBLER_DIRECTIVE";
  case ERROR_DEBUGGER_EXPECT_FAILURE:
    return "ERROR_DEBUGGER_EXPECT_FAILURE";
  case ERROR_ILLEGAL_LIST_INDEX:
    return "ERROR_ILLEGAL_LIST_INDEX";
  case ERROR_CANT_EVAL_EMPTY_EXPRESSION:
    return "ERROR_CANT_EVAL_EMPTY_EXPRESSION";
  case ERROR_VARIABLE_NOT_FOUND:
    return "ERROR_VARIABLE_NOT_FOUND";
  case ERROR_REFERENCE_NOT_EXPECTED_TYPE:
    return "ERROR_REFERENCE_NOT_EXPECTED_TYPE";
  case ERROR_NOT_REACHED:
    return "ERROR_NOT_REACHED";
  default:
    return "error";
  }
}

void print_error_code_name(int error_code) {
#ifndef NO_READABLE_ERROR_CODES
  fprintf(stderr, " ");
  do {
    fprintf(stderr, "*** ");
    fprintf(stderr, "%d -- %s", error_code,
            fatal_error_code_to_string(error_code));
    fprintf(stderr, " ***\n");
  } while (0);
#else
  fprintf(stderr, "%d %s\n", error_code, "ERROR_UKNOWN");
#endif /* NO_READABLE_ERROR_CODES */
}
