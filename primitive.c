/**
 * Implementation of the basic scheme primitives that don't have a
 * good home elsewhere.
 */

#include "primitive.h"

/**
 * Example (+ 1 2) => 3 or (+ 1 2 3) => 6
 */
tagged_reference_t primtive_function_plus(primitive_arguments_t arguments) {
  // checkargs
  int64_t result = untag_int64_t(arguments.args[0]);
  for (int i = 1; i < arguments.n_args; i++) {
    result += untag_int64_t(arguments.args[i]);
  }
  return tagged_reference(TAG_UINT64_T, result);
}

/**
 * Example (- 10 4) => 6
 */
tagged_reference_t primtive_function_sub(primitive_arguments_t arguments) {
  // checkargs
  int64_t result = untag_int64_t(arguments.args[0]);
  for (int i = 1; i < arguments.n_args; i++) {
    result -= untag_int64_t(arguments.args[i]);
  }
  return tagged_reference(TAG_UINT64_T, result);
}

/**
 * Example (* 10 4) => 40
 */
tagged_reference_t primtive_function_mul(primitive_arguments_t arguments) {
  // checkargs
  int64_t result = untag_int64_t(arguments.args[0]);
  for (int i = 1; i < arguments.n_args; i++) {
    result *= untag_int64_t(arguments.args[i]);
  }
  return tagged_reference(TAG_UINT64_T, result);
}

/**
 * Example (/ 10 2) => 5
 */
tagged_reference_t primtive_function_div(primitive_arguments_t arguments) {
  if (arguments.n_args != 2) {
    fatal_error(ERROR_WRONG_NUMBER_OF_ARGS);
  }

  int64_t result
      = untag_int64_t(arguments.args[0]) / untag_int64_t(arguments.args[0]);
  return tagged_reference(TAG_UINT64_T, result);
}

/**
 * comet-vm:get-tag returns the tag number of a scheme object. This is
 * used to implement primitives like pair? in pure scheme.
 */
tagged_reference_t primtive_comet_vm_get_tag(primitive_arguments_t arguments) {
  if (arguments.n_args != 1) {
    fatal_error(ERROR_WRONG_NUMBER_OF_ARGS);
  }
  uint64_t result = arguments.args[0].tag;
  return tagged_reference(TAG_UINT64_T, result);
}
