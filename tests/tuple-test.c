///
/// Test some operations on "tuples".
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_sizes() {
  type_t* type = intern_tuple_type(1, uint64_type());
  if (type->size != 8) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint64_t) size");
  }

  // All members must be 64bit aligned (at least for now) which also
  // effectively means the size must be a multiple of 

  type = intern_tuple_type(1, uint32_type());
  if (type->size != 8) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint32_t) size");
  }

  type = intern_tuple_type(2, uint32_type(), uint64_type());
  if (type->size != 16) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint32_t, uint64_t) size");
  }

  type = intern_tuple_type(3, uint64_type(), uint32_type(), uint64_type());
  if (type->size != 24) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint64_t, uint32_t, uint64_t) size");
  }
}

void test_offsets() {
  uint64_t stack_tuple_space[16];
  type_t* type;
  reference_t tuple_reference;
  reference_t field_reference;

  type = intern_tuple_type(1, uint64_type());
  tuple_reference = reference_of(type, &stack_tuple_space);
  memset(stack_tuple_space, 0, sizeof(stack_tuple_space));
  field_reference = tuple_reference_of_element(tuple_reference, 0);
  uint64_t field_value = reference_to_uint64(field_reference);
  if (field_value != 0) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint64_t)");
  }
  write_to_uint64_reference(field_reference, 0xffffffffffffffff);
  field_value = reference_to_uint64(field_reference);
  if (field_value != 0xffffffffffffffff) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint64_t)");
  }
  if (stack_tuple_space[1] != 0) {
    ARMYKNIFE_TEST_FAIL("tuple_t(uint64_t)");
  }
}

int main(int argc, char** argv) {
  test_sizes();
  test_offsets();
  exit(0);
}
