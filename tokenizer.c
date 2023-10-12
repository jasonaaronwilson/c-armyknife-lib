#line 2 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern ptr_array_t* tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_duplicate(ptr_array_t* token_array, const char* data);

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */

ptr_array_t* tokenize(const char* str, const char* delimiters) {
  ptr_array_t* result = make_ptr_array(1);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    if (string_contains_char(delimiters, ch)) {
      token_data[cpos++] = '\0';
      if (strlen(token_data) > 0) {
        add_duplicate(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = '\0';
  if (strlen(token_data) > 0) {
    add_duplicate(result, token_data);
  }

  return result;
}

/**
 * Add a *copy* of the string named data to the token list.
 */
void add_duplicate(ptr_array_t* token_array, const char* data) {
  ptr_array_add(token_array, string_duplicate(data));
}
