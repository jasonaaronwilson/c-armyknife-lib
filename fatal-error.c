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

// ======================================================================
// This is block is extraced to fatal-error.h
// ======================================================================

#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

typedef enum {
  ERROR_UKNOWN,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS,
  ERROR_ILLEGAL_LIST_INDEX,
  ERROR_CANT_EVAL_EMPTY_EXPRESSION,
  ERROR_VARIABLE_NOT_FOUND,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_NOT_REACHED,
  ERROR_MAX_PRIMITIVE_ARGS,
  ERROR_WRONG_NUMBER_OF_ARGS,
  ERROR_CLOSURE_HAS_NO_BODY,
  ERROR_NULL_ENVIRONMENT,
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */

// ======================================================================

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
  fprintf(stderr, "Necessaria Morte Mori...\n");
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
  case ERROR_MEMORY_FREE_NULL:
    return "ERROR_MEMORY_FREE_NULL";
  case ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS:
    return "ERROR_ARRAY_ACCESS_OUT_OF_BOUNDS";
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
  case ERROR_MAX_PRIMITIVE_ARGS:
    return "ERROR_MAX_PRIMITIVE_ARGS";
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
