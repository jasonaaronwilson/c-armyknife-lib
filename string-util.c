#include <stdlib.h>
#include <string.h>

#include "string-util.h"

int string_is_null_or_empty(const char *str) {
  return (str == NULL) || (strlen(str) == 0);
}

int string_equal(const char *str1, const char *str2) {
  if (string_is_null_or_empty(str1)) {
    return string_is_null_or_empty(str2);
  }
  return strcmp(str1, str2) == 0;
}

int string_starts_with(const char *str1, const char *str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

int string_ends_with(const char *str1, const char *str2) {
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);

  if (len2 > len1) {
    return 0;
  }

  return strcmp(str1 + (len1 - len2), str2) == 0;
}

int string_contains(const char *str, char ch) {
  if (string_is_null_or_empty(str)) {
    return 0;
  }
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ch) {
      return 1;
    }
  }
  return 0;
}

// accept 0x,0b prefixes and even a negative sign
uint64_t string_to_uint64(const char *str) {
  // FIXME
  return 0;
}

uint64_t string_hash(const char *str) {
  // FIXME
  return 0;
}

char *string_substring(const char *str, int start, int end) {
  // TODO(jawilson): check length of str...
  int result_size = end - start + 1;
  char *result = (char *)(malloc(result_size));
  for (int i = start; (i < end); i++) {
    result[i - start] = str[i];
  }
  result[result_size] = '\0';
  return result;
}

uint64_t string_parse_uint64(const char *string) {
  uint64_t integer = 0;
  uint64_t digit;

  while (*string != '\0') {
    digit = *string - '0';
    integer = integer * 10 + digit;
    string++;
  }

  return integer;
}
