#include "closure.h"
#include "allocate.h"

/**
 * Allocate the space for a closure accepting at most "N"
 * arguments. The caller must fill in the values of all fields in the
 * closure (see evaluator.c).
 */
closure_t* allocate_closure(uint64_t n_arg_names) {
  return (closure_t*) malloc_bytes(sizeof(closure_t)
                                   + n_arg_names * sizeof(char*));
}
