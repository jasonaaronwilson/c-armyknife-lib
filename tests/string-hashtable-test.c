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
  test_assert(is_not_ok(value));

  ht = string_ht_insert(ht, "a", str_to_value("A"));
  value = string_ht_find(ht, "a");
  test_assert(is_ok(value) && string_equal("A", value.str));

  ht = string_ht_insert(ht, "b", str_to_value("B"));
  value = string_ht_find(ht, "a");
  test_assert(is_ok(value) && string_equal("A", value.str));
  value = string_ht_find(ht, "b");
  test_assert(is_ok(value) && string_equal("B", value.str));

  // Add a few more elements to the front of the ht.
  ht = string_ht_insert(ht, "c", str_to_value("C"));
  ht = string_ht_insert(ht, "d", str_to_value("D"));

  // Finally delete a node.
  ht = string_ht_delete(ht, "b");
  value = string_ht_find(ht, "b");
  test_assert(is_not_ok(value));

  value = string_ht_find(ht, "a");
  test_assert(is_ok(value) && string_equal("A", value.str));

  value = string_ht_find(ht, "c");
  test_assert(is_ok(value) && string_equal("C", value.str));

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
      test_assert(is_not_ok(string_ht_find(ht, uint64_to_string(next))));
    } else {
      test_assert(is_ok(string_ht_find(ht, uint64_to_string(next))));
    }
  }
}

int main(int argc, char** argv) {
  test_string_ht();
  test_ht_random();
  exit(0);
}
