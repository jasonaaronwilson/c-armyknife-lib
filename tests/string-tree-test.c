//
/// Test string-tree.c
///

#include <stdio.h>
#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_tree() {
  string_tree_t* tree = NULL;
  value_result_t value;

  value = string_tree_find(tree, "a");
  if (value.found) {
    ARMYKNIFE_TEST_FAIL("find in empty list should return NULL");
  }

  tree = string_tree_insert(tree, "a", (value_t) "A");
  value = string_tree_find(tree, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }

  tree = string_tree_insert(tree, "b", (value_t) "B");
  value = string_tree_find(tree, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_tree_find(tree, "b");
  if (!value.found || strcmp("B", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'B'");
  }

  // Add a few more elements
  tree = string_tree_insert(tree, "c", (value_t) "C");
  tree = string_tree_insert(tree, "d", (value_t) "D");

  /*

  // Finally delete a node.
  list = alist_delete(list, "b");
  value = alist_find(list, "b");
  if (value.found) {
    ARMYKNIFE_TEST_FAIL("should not have found a value for 'b'");
  }
  value = alist_find(list, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = alist_find(list, "c");
  if (!value.found || strcmp("C", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'C'");
  }

  // clang-format off
  string_alist_foreach(list, key, value, { 
      fprintf(stderr, "key=%s value = %s\n", key, value.str);
  });
  // clang-format on
  */
}

int main(int argc, char** argv) {
  test_tree();
  exit(0);
}
