#line 2 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

extern array_t(char*) * tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"
#include "string-util.h"
#include "tokenizer.h"

array_t(char*) * add_duplicate(array_t(char*) * token_array, const char* data);

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */

array_t(char*) * tokenize(const char* str, const char* delimiters) {
  array_t(char*)* result = make_array(char_ptr_type(), 4);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    if (string_contains_char(delimiters, ch)) {
      token_data[cpos++] = '\0';
      if (strlen(token_data) > 0) {
        result = add_duplicate(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = '\0';
  if (strlen(token_data) > 0) {
    result = add_duplicate(result, token_data);
  }

  return result;
}

/**
 * Add a *copy* of the string named data to the token list.
 */
array_t(char*) * add_duplicate(array_t(char*) * token_array, const char* data) {
  return array_add(token_array, reference_of(token_array->element_type,
                                             string_duplicate(data)));
}
