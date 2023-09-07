#include "string-util.h"
#include <string.h>

int string_is_null_or_empty(char *str1) {
  return (str1 == NULL) || (strlen(str1) == 0);
}

int string_equal(char *str1, char *str2) {
  if (string_is_null_or_empty(str1)) {
    return string_is_null_or_empty(str2);
  }
  return strcmp(str1, str2) == 0;
}

int string_starts_with(char *str1, char *str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

int string_contains(char *str, char ch) {
  if (string_is_null_or_empty(str)) {
    return 0;
  }
  while (*str++) {
    if (*str == ch) {
      return 1;
    }
  }
  return 0;
}

// accept 0x,0b prefixes and even a negative sign
uint64_t string_to_uint64(char *str) {
  // FIXME
  return 0;
}

uint64_t string_hash(char *str) {
  // FIXME
  return 0;
}
