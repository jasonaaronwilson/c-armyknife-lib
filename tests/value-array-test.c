//
// Test some operations on pointer arrays.
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test() {
  value_array_t* array = make_value_array(1);
  value_array_add(array, str_to_value("a"));
  value_array_add(array, str_to_value("b"));
  value_array_add(array, str_to_value("c"));
  value_array_add(array, str_to_value("d"));
  value_array_add(array, str_to_value("e"));
  value_array_add(array, str_to_value("f"));

  if (!string_equal("c", value_array_get(array, 2).str)) {
    test_fail("expected 'c'");
  }
}

void test_push_pop() {
  value_array_t* array = make_value_array(1);
  value_array_push(array, str_to_value("a"));
  value_array_push(array, str_to_value("b"));
  value_array_push(array, str_to_value("c"));

  if (!string_equal("c", value_array_pop(array).str)) {
    test_fail("expected 'c'");
  }
  if (!string_equal("b", value_array_pop(array).str)) {
    test_fail("expected 'b'");
  }
  if (!string_equal("a", value_array_pop(array).str)) {
    test_fail("expected 'a'");
  }
}

void test_insert_at_and_delete_at() {
  value_array_t* array = make_value_array(1);
  value_array_insert_at(array, 0, str_to_value("a"));

  if (!string_equal("a", value_array_get(array, 0).str)) {
    test_fail("insert at failed #0");
  }

  value_array_insert_at(array, 0, str_to_value("b"));
  if (!string_equal("b", value_array_get(array, 0).str)
      || !string_equal("a", value_array_get(array, 1).str)) {
    test_fail("insert at failed #1");
  }

  value_array_insert_at(array, 0, str_to_value("c"));
  if (!string_equal("c", value_array_get(array, 0).str)
      || !string_equal("b", value_array_get(array, 1).str)
      || !string_equal("a", value_array_get(array, 2).str)) {
    for (int i = 0; i < 3; i++) {
      fprintf(stdout, "%s\n", value_array_get(array, i).str);
    }
    test_fail("insert at failed #2");
  }

  if (!string_equal("c", value_array_delete_at(array, 0).str)) {
    test_fail("expected 'c'");
  }
  if (!string_equal("b", value_array_delete_at(array, 0).str)) {
    test_fail("expected 'b'");
  }
  if (!string_equal("a", value_array_delete_at(array, 0).str)) {
    test_fail("expected 'a'");
  }
}

#define RANDOM_TEST_ITERATION_LIMIT 1000

void check_for_zero_or_duplicates(value_array_t* array) {
  int counts[RANDOM_TEST_ITERATION_LIMIT] = {0};

  for (int i = 0; i < array->length; i++) {
    uint64_t value = array->elements[i].u64;
    if (value == 0) {
      test_fail("shouldn't have a value of zero in the random array");
    }
    if (value > RANDOM_TEST_ITERATION_LIMIT) {
      test_fail("we shouldn't see this value in the array");
    }
    counts[value]++;
  }

  for (int i = 0; i < RANDOM_TEST_ITERATION_LIMIT; i++) {
    if (counts[i] > 1) {
      test_fail("shouldn't have a duplicate value in the random array");
    }
  }
}

void test_insert_at_and_delete_at_random() {
  value_array_t* array = make_value_array(1);

  random_state_t state = random_state_for_test();
  for (int i = 1; i < RANDOM_TEST_ITERATION_LIMIT; i++) {
    uint64_t next = random_next(&state);
    uint32_t position = next % (array->length + 1);
    value_array_insert_at(array, position, u64_to_value(i));
    if (array->length != i) {
      test_fail("the length of the array was not incremented");
    }
    check_for_zero_or_duplicates(array);
  }

  for (int i = 1; i < RANDOM_TEST_ITERATION_LIMIT; i++) {
    uint64_t next = random_next(&state);
    uint32_t position = next % array->length;
    value_array_delete_at(array, position);
    if (array->length != (RANDOM_TEST_ITERATION_LIMIT - (i + 1))) {
      test_fail("the length of the array was not decremented");
    }
    check_for_zero_or_duplicates(array);
  }
}

int main(int argc, char** argv) {
  test();
  test_push_pop();
  test_insert_at_and_delete_at();
  test_insert_at_and_delete_at_random();
  exit(0);
}
