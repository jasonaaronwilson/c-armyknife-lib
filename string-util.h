#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

extern int string_is_null_or_empty(const char *str1);
extern int string_equal(const char *str1, const char *str2);
extern int string_starts_with(const char *str1, const char *str2);
extern int string_ends_with(const char *str1, const char *str2);
extern int string_contains(const char *str, const char ch);
extern uint64_t string_to_uint64(const char *str);
extern uint64_t string_hash(const char *str);
extern char *string_substring(const char *str, int start, int end);

#endif /* _STRING_UTIL_H_ */
