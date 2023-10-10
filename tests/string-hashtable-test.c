///
/// Test string-hashtable.c
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_string_ht() {
  string_hashtable_t* ht = make_string_hashtable(2);
  void* value;

  value = string_ht_find(ht, "a");
  if (value != NULL) {
    ARMYKNIFE_TEST_FAIL("find in empty ht should return NULL");
  }

  ht = string_ht_insert(ht, "a", "A");
  value = string_ht_find(ht, "a");
  if (value == NULL || strcmp("A", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }

  ht = string_ht_insert(ht, "b", "B");
  value = string_ht_find(ht, "a");
  if (value == NULL || strcmp("A", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "b");
  if (value == NULL || strcmp("B", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'B'");
  }

  // Add a few more elements to the front of the ht.
  ht = string_ht_insert(ht, "c", "C");
  ht = string_ht_insert(ht, "d", "D");

  // Finally delete a node.
  ht = string_ht_delete(ht, "b");
  value = string_ht_find(ht, "b");
  if (value != NULL) {
    ARMYKNIFE_TEST_FAIL("should not have found a value for 'b'");
  }
  value = string_ht_find(ht, "a");
  if (value == NULL || strcmp("A", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "c");
  if (value == NULL || strcmp("C", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'C'");
  }

  string_ht_foreach(ht, key, char*, value,
                    { fprintf(stderr, "key=%s value = %s\n", key, value); });
}

int main(int argc, char** argv) {
  test_string_ht();
  exit(0);
}
