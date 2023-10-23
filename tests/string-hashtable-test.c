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
  if (is_ok(value)) {
    ARMYKNIFE_TEST_FAIL("find in empty ht should return NULL");
  }

  ht = string_ht_insert(ht, "a", str_to_value("A"));
  value = string_ht_find(ht, "a");
  if (is_not_ok(value) || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }

  ht = string_ht_insert(ht, "b", str_to_value("B"));
  value = string_ht_find(ht, "a");
  if (is_not_ok(value) || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "b");
  if (is_not_ok(value) || strcmp("B", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'B'");
  }

  // Add a few more elements to the front of the ht.
  ht = string_ht_insert(ht, "c", str_to_value("C"));
  ht = string_ht_insert(ht, "d", str_to_value("D"));

  // Finally delete a node.
  ht = string_ht_delete(ht, "b");
  value = string_ht_find(ht, "b");
  if (is_ok(value)) {
    ARMYKNIFE_TEST_FAIL("should not have found a value for 'b'");
  }
  value = string_ht_find(ht, "a");
  if (is_not_ok(value) || strcmp("A", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'A'");
  }
  value = string_ht_find(ht, "c");
  if (is_not_ok(value) || strcmp("C", value.str) != 0) {
    ARMYKNIFE_TEST_FAIL("should have found 'C'");
  }

  // clang-format off
  string_ht_foreach(ht, key, value, { 
      fprintf(stderr, "key=%s value = %s\n", key, value.str);
  });
  // clang-format on
}

void test_ht_random() {
  string_hashtable_t* ht = make_string_hashtable(3);

  int iterations = 1000;
  random_state_t state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    ht = string_ht_insert(ht, uint64_to_string(next), u64_to_value(next));
  }

  // Now delete a bunch of things
  state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    if ((next & 3) == 0) {
      ht = string_ht_delete(ht, uint64_to_string(next));
    }
  }

  // Make sure everything deleted is not found
  state = random_state_for_test();
  for (int i = 0; i < iterations; i++) {
    uint64_t next = random_next(&state);
    if ((next & 3) == 0) {
      if (is_ok(string_ht_find(ht, uint64_to_string(next)))) {
        ARMYKNIFE_TEST_FAIL("found an element we deleted");
      }
    } else {
      if (is_not_ok(string_ht_find(ht, uint64_to_string(next)))) {
        ARMYKNIFE_TEST_FAIL("element should be found");
      }
    }
  }
}

int main(int argc, char** argv) {
  test_string_ht();
  test_ht_random();
  exit(0);
}
