///
/// Make an array of uint64 and add 16 elements to it.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

#define FAIL(testcase_name) \
  do { \
  fprintf(stderr, "%s:%d: -- FAIL (testcase=%s)\n", __FILE__, __LINE__, testcase_name); \
  exit(1); \
  } while (0)

int main(int argc, char** argv) {
  TRACE();

  array_t* array = make_array(uint64_type(), 1);
  for (int i = 0; i < 16; i++) {
    TRACE();
    fprintf(stderr, "i is %d\n", i);
    uint64_t value = i + 100;
    array = array_add(array, reference_of(uint64_type(), &value));
    /*
    uint64_t element = reference_to_uint64(array_get_reference(array, i));
    fprintf(stderr, "ith value %d should be %lu and is %lu\n", i, value, element);
    */
  }

  TRACE();
  for (int i = 0; i < 16; i++) {
    TRACE();
    uint64_t value = i + 100;
    uint64_t element = reference_to_uint64(array_get_reference(array, i));
    if (element != value) {
      fprintf(stderr, "%lu != %lu\n", value, element);
      FAIL("array_get_reference");
    }
  }

  exit(0);
}
