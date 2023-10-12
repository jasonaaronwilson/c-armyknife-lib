///
/// Test string-hashtable.c
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_string_ht() {
  string_hashtable_t* ht = make_string_hashtable(2);
  value_result_t value;

  value = string_ht_find(ht, "a");
  if (value.found) {
    ARMYKNIFE_TEST_FAIL("find in empty ht should return NULL");
  }

  ht = string_ht_insert(ht, "a", (value_t) "A");
  value = string_ht_find(ht, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }

  ht = string_ht_insert(ht, "b", (value_t) "B");
  value = string_ht_find(ht, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "b");
  if (!value.found || strcmp("B", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'B'");
  }

  // Add a few more elements to the front of the ht.
  ht = string_ht_insert(ht, "c", (value_t) "C");
  ht = string_ht_insert(ht, "d", (value_t) "D");

  // Finally delete a node.
  ht = string_ht_delete(ht, "b");
  value = string_ht_find(ht, "b");
  if (value.found) {
    ARMYKNIFE_TEST_FAIL("should not have found a value for 'b'");
  }
  value = string_ht_find(ht, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "c");
  if (!value.found || strcmp("C", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'C'");
  }

  // clang-format off
  string_ht_foreach(ht, key, value, { 
      fprintf(stderr, "key=%s value = %s\n", key, value.str);
  });
  // clang-format on
}

int main(int argc, char** argv) {
  test_string_ht();
  exit(0);
}
