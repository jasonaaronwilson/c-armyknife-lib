///
/// Test some operations on "tuples".
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
  
  type_t* type = intern_tuple_type(1, uint64_type());
  if (type->size != 8) {
    FAIL("tuple_t(uint64_t) size");
  }

  type = intern_tuple_type(1, uint32_type());
  if (type->size != 4) {
    FAIL("tuple_t(uint32_t) size");
  }

  exit(0);
}
