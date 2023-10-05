///
/// Testing appending references of things to a byte array.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_append_uint64() {
  byte_array_t* byte_array = make_byte_array(16);

  uint64_t number = 123456789;
  reference_t number_ref = reference_of_uint64(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "123456789") != 0) {
    ARMYKNIFE_TEST_FAIL("expected 123456789");
  }
}

void test_append_uint32() {
  byte_array_t* byte_array = make_byte_array(16);

  uint32_t number = 123456789;
  reference_t number_ref = reference_of_uint32(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "123456789") != 0) {
    ARMYKNIFE_TEST_FAIL("expected 123456789");
  }
}

void test_append_uint16() {
  byte_array_t* byte_array = make_byte_array(16);

  uint16_t number = 50000;
  reference_t number_ref = reference_of_uint16(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "50000") != 0) {
    ARMYKNIFE_TEST_FAIL("expected 50000");
  }
}

void test_append_uint8() {
  byte_array_t* byte_array = make_byte_array(16);

  uint8_t number = 200;
  reference_t number_ref = reference_of_uint8(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "200") != 0) {
    ARMYKNIFE_TEST_FAIL("expected 200");
  }
}

void test_append_int64() {
  byte_array_t* byte_array = make_byte_array(16);

  int64_t number = -123456789;
  reference_t number_ref = reference_of_int64(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "-123456789") != 0) {
    ARMYKNIFE_TEST_FAIL("expected -123456789");
  }
}

void test_append_int32() {
  byte_array_t* byte_array = make_byte_array(16);

  int32_t number = -123456789;
  reference_t number_ref = reference_of_int32(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "-123456789") != 0) {
    ARMYKNIFE_TEST_FAIL("expected -123456789");
  }
}

void test_append_int16() {
  byte_array_t* byte_array = make_byte_array(16);

  int32_t number = -30000;
  reference_t number_ref = reference_of_int32(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "-30000") != 0) {
    ARMYKNIFE_TEST_FAIL("expected -30000");
  }
}

void test_append_int8() {
  byte_array_t* byte_array = make_byte_array(16);

  int8_t number = -100;
  reference_t number_ref = reference_of_int8(&number);
  byte_array = byte_array_append_reference(byte_array, number_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);
  if (strcmp(str, "-100") != 0) {
    fprintf(stderr, "%s\n", str);
    ARMYKNIFE_TEST_FAIL("expected -100");
  }
}

void test_append_string() {
  byte_array_t* byte_array = make_byte_array(16);

  char* input_str = "Hello World!";
  char* expected_str = "\"Hello World!\"";

  reference_t str_ref = reference_of_char_ptr(&input_str);
  byte_array = byte_array_append_reference(byte_array, str_ref);
  char* str = byte_array_to_c_string(byte_array);
  free(byte_array);

  if (strcmp(expected_str, str) != 0) {
    fprintf(stderr, "%s", str);
    ARMYKNIFE_TEST_FAIL("expected Hello World!");
  }
}

int main(int argc, char** argv) {

  test_append_uint64();
  test_append_uint32();
  test_append_uint16();
  test_append_uint8();

  test_append_int64();
  test_append_int32();
  test_append_int16();
  test_append_int8();

  test_append_string();

  exit(0);
}
