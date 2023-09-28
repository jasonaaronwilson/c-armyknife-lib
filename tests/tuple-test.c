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

int main(int argc, char** argv) {
  test_sizes();
  exit(0);
}
