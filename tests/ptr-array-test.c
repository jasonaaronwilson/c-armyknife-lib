//
// Test some operations on pointer arrays.
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test() {
  ptr_array_t* array = make_ptr_array(1);
  ptr_array_add(array, "a");
  ptr_array_add(array, "b");
  ptr_array_add(array, "c");
  ptr_array_add(array, "d");
  ptr_array_add(array, "e");
  ptr_array_add(array, "f");

  if (!string_equal(ptr_array_get(array, 2), "c")) {
    ARMYKNIFE_TEST_FAIL("expected 'c'");
  }
}

int main(int argc, char** argv) {
  test();
  exit(0);
}
