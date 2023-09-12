#include "primitive.h"

/**
 * Example (+ 1 2) => 3 or (+ 1 2 3) => 6
 */
tagged_reference_t primtive_function_plus(pair_t* args) {
  uint64_t length = pair_list_length(args);
  uint64_t result = 0;
  for (int i = 0; i < length; i++) {
    result += untag_uint64_t(pair_list_get(args, i));
  }
  return tagged_reference(TAG_UINT64_T, result);
}
