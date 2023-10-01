///
/// Test out some hashtree operations.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_string_to_string_htree() {
  pointer_t(hashtree_t(char*, char*)) htree
      = make_empty_hashtree_node(char_ptr_type(), char_ptr_type());
  char* key = "key1000";
  char* value = "value100";
  boolean_t result = hashtree_insert(htree, char_ptr_type(), char_ptr_type(),
                                     1000, reference_of_char_ptr(&key),
                                     reference_of_char_ptr(&value));
}

int main(int argc, char** argv) {
  test_string_to_string_htree();
  exit(0);
}
