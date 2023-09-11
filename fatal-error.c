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

void print_error_code_name(int error_code) {
#ifndef NO_READABLE_ERROR_CODES
  fprintf(stderr, " ");
  do {
    fprintf(stderr, "*** ");
    switch (error_code) {
    case ERROR_UKNOWN:
      fprintf(stderr, "%s", "ERROR_UKNOWN");
      break;
    case ERROR_MEMORY_ALLOCATION:
      fprintf(stderr, "%s", "ERROR_MEMORY_ALLOCATION");
      break;
    case ERROR_MEMORY_READ:
      fprintf(stderr, "%s", "ERROR_MEMORY_READ");
      break;
    case ERROR_MEMORY_WRITE:
      fprintf(stderr, "%s", "ERROR_MEMORY_WRITE");
      break;
    case ERROR_OPCODE_UNKNOWN:
      fprintf(stderr, "%s", "ERROR_OPCODE_UNKNOWN");
      break;
    case ERROR_OPCODE_DECODE_ERROR:
      fprintf(stderr, "%s", "ERROR_OPCODE_DECODE_ERROR");
      break;
    case ERROR_ARG_DECODE_ERROR:
      fprintf(stderr, "%s", "ERROR_ARG_DECODE_ERROR");
      break;
    case ERROR_UNIMLEMENTED_OPCODE:
      fprintf(stderr, "%s", "ERROR_UNIMLEMENTED_OPCODE");
      break;
    case ERROR_TOKEN_LIST_GET:
      fprintf(stderr, "%s", "ERROR_TOKEN_LIST_GET");
      break;
    case ERROR_EXPECTED_GENERAL_REGISTER:
      fprintf(stderr, "%s", "ERROR_EXPECTED_GENERAL_REGISTER");
      break;
    case ERROR_EXPECTED_FLOATING_REGISTER:
      fprintf(stderr, "%s", "ERROR_EXPECTED_FLOATING_REGISTER");
      break;
    case ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS:
      fprintf(stderr, "%s", "ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS");
      break;
    case ERROR_UNKNOWN_ASSEMBLER_DIRECTIVE:
      fprintf(stderr, "%s", "ERROR_UNKNOWN_ASSEMBLER_DIRECTIVE");
      break;
    case ERROR_DEBUGGER_EXPECT_FAILURE:
      fprintf(stderr, "%s", "ERROR_DEBUGGER_EXPECT_FAILURE");
      break;
    default:
      fprintf(stderr, "%s", "(non handled error code translation)");
      break;
    }
    fprintf(stderr, " ***");
  } while (0);
#endif /* NO_READABLE_ERROR_CODES */
  fprintf(stderr, "%s\n", "ERROR_UKNOWN");
}
