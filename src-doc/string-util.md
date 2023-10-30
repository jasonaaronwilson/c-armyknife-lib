# @file string-util.c

This contains additional string function to operate on "strings"
since the C libary has only basic routines.
 
## @function string_append

Return a freshly allocated string that is the concatentation of the
two input strings (neither of which should be NULL);
 
## @function string_contains_char

Return true if str contains that given character ch.
 
## @function string_duplicate

Just like strdup except NULL is a valid source argument and we use
malloc_bytes which checks the return result from malloc.
 
## @function string_ends_with

Return true if str1 ends with all of str2.
 
## @function string_equal

Return true if two strings are equal.
 
## @function string_hash

Return a fast but generally high-quality 64bit hash of an input
string.
 
## @function string_index_of_char
 
## @function string_is_null_or_empty

Return true if the given string is NULL or strlen is zero.
 
## @function string_left_pad

Prefix a string with left padding (if necessary) to make it at
least N bytes long.
 
## @function string_parse_uint64

Parse a string as a uint64_t.

If the string begins with "0x", the the number should be a well
formed hexidecimal number (in all lower-case).

If the string begins with "0b", the the number should be a well
formed binary number.

Ortherwise the number should be a well-formed decimal number.

While currently overflow is not detected, that is likely to be
detected in future versions of the library.
 
## @function string_printf

Perform printf to a buffer and return the result as a dynamically
allocated string. The string is automatically allocated to the
appropriate size.
 
## @function string_starts_with

Return true if str1 starts with all of str2.
 
## @function string_substring

Return a substring of the given string as a newly allocated string.
 
## @function uint64_to_string

Convert a uint64_t number to a string.
 
