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

  // Finally delete a node.
  tree = string_tree_delete(tree, "b");
  value = string_tree_find(tree, "b");
  if (value.found) {
    ARMYKNIFE_TEST_FAIL("should not have found a value for 'b'");
  }

  value = string_tree_find(tree, "a");
  if (!value.found || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_tree_find(tree, "c");
  if (!value.found || strcmp("C", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'C'");
  }

  /*
  // clang-format off
  string_alist_foreach(list, key, value, {
      fprintf(stderr, "key=%s value = %s\n", key, value.str);
  });
  // clang-format on
  */
}

void check_levels(string_tree_t* t) {
  if (t) {
    int left_level = t->left ? t->left->level : 0;
    int right_level = t->right ? t->right->level : 0;
    if (t->level != left_level + 1) {
      ARMYKNIFE_TEST_FAIL("check level failure left");
    }
    if (t->level != right_level + 1) {
      ARMYKNIFE_TEST_FAIL("check level failure right");
    }
    check_levels(t->left);
    check_levels(t->right);
  }
}

void check_values(string_tree_t* t) {
  if (t) {
    char* value_to_key_string = uint64_to_string((t->value).u64);
    if (strcmp(t->key, value_to_key_string) != 0) {
      ARMYKNIFE_TEST_FAIL("the value and key are not matching");
    }
    check_values(t->left);
    check_values(t->right);
  }
}

void test_random_insertion_and_deletion() {
  // Could increasing this to 1000 invoke the birthday paradox (since
  // the reference implementation, string_hashtable, fails to match, I
  // don't think that is the problem.
  int iterations = 103;

  string_tree_t* tree = NULL;
  string_hashtable_t* ht = make_string_hashtable(100);

  random_state_t state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    tree = string_tree_insert(tree, uint64_to_string(next), u64_to_value(next));
    ht = string_ht_insert(ht, uint64_to_string(next), u64_to_value(next));
    fprintf(stderr, "iteration=%d\n", i);
    check_levels(tree);
    check_values(tree);
  }

  check_levels(tree);
  check_values(tree);

  // TODO(jawilson): check keys and values being the same

  // TODO(jawilson): delete 1/4 of the keys and repeat above checks.
  state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    if ((next & 3) == 0) {
      tree = string_tree_delete(tree, uint64_to_string(next));
      ht = string_ht_delete(ht, uint64_to_string(next));
      check_levels(tree);
    }
  }

  // Check we have just the correct keys!
  state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    value_result_t lookup_result
        = string_tree_find(tree, uint64_to_string(next));
    value_result_t lookup_result_reference
        = string_ht_find(ht, uint64_to_string(next));

    if (lookup_result.found != lookup_result_reference.found) {
      ARMYKNIFE_TEST_FAIL("tree and hashtable reference are not in agreement!");
    }

    if ((next & 3) == 0) {
      if (lookup_result.found) {
        ARMYKNIFE_TEST_FAIL("deleted item should not be found!");
      }
    } else {
      if (!lookup_result.found) {
        ARMYKNIFE_TEST_FAIL("non deleted item should be found");
      }
    }
  }
}

int main(int argc, char** argv) {
  test_tree();
  test_random_insertion_and_deletion();
  exit(0);
}
