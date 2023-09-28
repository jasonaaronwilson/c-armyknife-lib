///
/// Make an array of uint64 and add 16 elements to it.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  array_t* array = make_array(uint64_type(), 1);
  for (int i = 0; i < 16; i++) {
    fprintf(stderr, "i is %d\n", i);
    uint64_t value = i + 100;
    array = array_add(array, reference_of(uint64_type(), &value));
    /*
    uint64_t element = reference_to_uint64(array_get_reference(array, i));
    fprintf(stderr, "ith value %d should be %lu and is %lu\n", i, value, element);
    */
  }

  for (int i = 0; i < 16; i++) {
    uint64_t value = i + 100;
    uint64_t element = reference_to_uint64(array_get_reference(array, i));
    if (element != value) {
      fprintf(stderr, "%lu != %lu\n", value, element);
      ARMYKNIFE_TEST_FAIL("array_get_reference");
    }
  }

  exit(0);
}
