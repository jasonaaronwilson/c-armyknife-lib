///
/// Testing appending references of things to a byte array.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_alist() {
  string_alist_t* list = NULL;
  void* value;

  value = alist_find(list, "a");
  if (value != NULL) {
    ARMYKNIFE_TEST_FAIL("find in empty list should return NULL");
  }

  list = alist_insert(list, "a", "A");
  value = alist_find(list, "a");
  if (value == NULL || strcmp("A", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }

  list = alist_insert(list, "b", "B");
  value = alist_find(list, "a");
  if (value == NULL || strcmp("A", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = alist_find(list, "b");
  if (value == NULL || strcmp("B", (char*) value) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'B'");
  }

  list = alist_insert(list, "c", "C");
  list = alist_insert(list, "d", "D");
}

int main(int argc, char** argv) {
  test_alist();
  exit(0);
}
