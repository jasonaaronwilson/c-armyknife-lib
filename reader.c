#include <string.h>

#include "reader.h"
#include "tagged-pair.h"
#include "tagged-reference.h"

int is_whitespace(char ch) { return ch == ' ' || ch == '\n'; }

int is_delimiter(char ch) { return ch == '(' || ch == ')'; }

int is_token_end(char ch) { return is_delimiter(ch) || is_whitespace(ch); }

int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

/**
 * Return true if all characters from start to the end of the string
 * are whitespace or if we are already at the end of the string.
 */
int all_whitespace_or_end(const char* str, uint64_t start) {
  while (str[start++]) {
    if (!is_whitespace(str[start])) {
      return 0;
    }
  }
  return 1;
}

/**
 * This is a light-weight "s-expression" reader.
 */
tagged_reference_t read(const char* str, uint64_t start) {
  uint64_t limit = strlen(str);
  while (is_whitespace(str[start])) {
    start++;
  }
  if (str[start] == '(') {
    start++;
    tagged_pair_t* result = NULL;
    while (!all_whitespace_or_end(str, start)) {
      tagged_reference_t child = read(str, start);
      if (child.tag == TAG_ERROR_T) {
        return child;
      } else if (child.tag == TAG_NULL) {
        tagged_reference_t reference;
        reference.tag = TAG_TAGGED_PAIR_T;
        reference.data = result;
        return reference;
      } else {
        result = tagged_pair_list_append(result, make_tagged_pair(child, NIL));
      }
    }
  } else if (str[start] == ')') {
    tagged_reference_t reference;
    reference.tag = TAG_NULL;
    return reference;
  } else if (is_digit(str[start])) {
    uint64_t end = start + 1;
    while (!is_token_end(str[end])) {
      end++;
    }
    char* token = string_substring(str, start, end);
    uint64_t number = string_parse_uint64(token);
    free(token);
    tagged_reference_t reference;
    reference.tag = TAG_UINT64_T;
    reference.data = number;
    return reference;
  } else {
    // a "symbol" in lisp parlance
    uint64_t end = start + 1;
    while (!is_token_end(str[end])) {
      end++;
    }
    char* token = string_substring(str, start, end);
    tagged_reference_t reference;
    reference.tag = TAG_STRING;
    reference.data = token;
    return reference;
  }
}
