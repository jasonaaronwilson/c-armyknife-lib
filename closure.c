#include "closure.h"
#include "allocate.h"

closure_t* allocate_closure(uint64_t n_arg_names) {
  return (closure_t*) malloc_bytes(sizeof(closure_t)
                                   + n_arg_names * sizeof(char*));
}
