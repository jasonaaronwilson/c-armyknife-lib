#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

#include "fatal-error.h"
#include "tagged-reference.h"

extern int string_is_null_or_empty(const char* str1);
extern int string_equal(const char* str1, const char* str2);
extern int string_starts_with(const char* str1, const char* str2);
extern int string_ends_with(const char* str1, const char* str2);
extern int string_contains(const char* str, const char ch);
extern uint64_t string_to_uint64(const char* str);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);

static inline char* untag_string(tagged_reference_t reference) {
  require_tag(reference, TAG_STRING);
  return (char*) reference.data;
}

static inline char* untag_reader_symbol(tagged_reference_t reference) {
  require_tag(reference, TAG_READER_SYMBOL);
  return (char*) reference.data;
}

static inline char*
    untag_string_or_reader_symbol(tagged_reference_t reference) {
  if (reference.tag == TAG_STRING || reference.tag == TAG_READER_SYMBOL) {
    return (char*) reference.data;
  }
  fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
}

#endif /* _STRING_UTIL_H_ */
