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
    ARMYKNIFE_TEST_FAIL("expected 'c'");
  }
}

void test_insert_at_and_delete_at() {
  value_array_t* array = make_value_array(1);
  value_array_insert_at(array, 0, str_to_value("a"));
  value_array_insert_at(array, 0, str_to_value("b"));
  value_array_insert_at(array, 0, str_to_value("c"));

  if (!string_equal("c", value_array_delete_at(array, 0).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'c'");
  }
  if (!string_equal("b", value_array_delete_at(array, 0).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'b'");
  }
  if (!string_equal("a", value_array_delete_at(array, 0).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'a'");
  }

  random_state_t state = random_state_for_test();

  for (int i = 0; i < 25; i++) {
    uint64_t next = random_next(&state);
    uint32_t position = next % (array->length + 1);
    value_array_insert_at(array, position, u64_to_value(i));
  }
  for (int i = 0; i < 25; i++) {
    uint64_t next = random_next(&state);
    uint32_t position = next % array->length;
    value_array_delete_at(array, position);
  }
}

void test_push_pop() {
  value_array_t* array = make_value_array(1);
  value_array_push(array, str_to_value("a"));
  value_array_push(array, str_to_value("b"));
  value_array_push(array, str_to_value("c"));

  if (!string_equal("c", value_array_pop(array).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'c'");
  }
  if (!string_equal("b", value_array_pop(array).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'b'");
  }
  if (!string_equal("a", value_array_pop(array).str)) {
    ARMYKNIFE_TEST_FAIL("expected 'a'");
  }
}

int main(int argc, char** argv) {
  test();
  test_push_pop();
  test_insert_at_and_delete_at();
  exit(0);
}
