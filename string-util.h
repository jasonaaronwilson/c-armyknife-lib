#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

extern int string_is_null_or_empty(char *str1);
extern int string_equal(char *str1, char *str2);
extern int string_starts_with(char *str1, char *str2);
extern int string_contains(char *str, char ch);
extern uint64_t
string_to_uint64(char *str); // accept 0x,0b prefixes and even a negative sign
extern uint64_t string_hash(char *str);

#endif /* _STRING_UTIL_H_ */
