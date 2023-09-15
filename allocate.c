/**
 * @file allocate.c
 *
 * This file contains wrappers around malloc to make it more
 * convenient and possibly safer.
 */

// ======================================================================
// This is block is extraced to allocate.h
// ======================================================================

#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

#endif /* _ALLOCATE_H_ */

// ======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "boolean.h"
#include "fatal-error.h"

boolean_t is_initialized = false;
boolean_t should_log_value = false;

static inline boolean_t should_log() {
  if (is_initialized) {
    return should_log_value;
  }
  char* var = getenv("ARMYKNIFE_LOG_MEMORY_ALLOCATION");
  is_initialized = true;
  if (var != NULL && strcmp(var, "true") == 0) {
    should_log_value = true;
  }
  return should_log_value;
}

/**
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
uint8_t* checked_malloc(char* file, int line, uint64_t amount) {
  if (should_log()) {
    fprintf(stderr, "ALLOCATE %s:%d -- %d\n", file, line, amount);
  }
  uint8_t* result = malloc(amount);
  if (result == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_ALLOCATION);
  }
  memset(result, 0, amount);
  return result;
}

/**
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
void checked_free(char* file, int line, void* pointer) {
  if (should_log()) {
    fprintf(stderr, "DEALLOCATE %s:%d -- %d\n", file, line, pointer);
  }
  if (pointer == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_FREE_NULL);
  }
  free(pointer);
}
