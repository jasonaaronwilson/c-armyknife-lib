#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "fatal-error.h"

/**
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
uint8_t* checked_malloc(char* file, int line, uint64_t amount) {
  uint8_t* result = malloc(amount);
  if (result == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_ALLOCATION);
  }
  memset(result, 0, amount);
  return result;
}
