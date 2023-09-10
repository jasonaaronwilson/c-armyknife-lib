/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "fatal-error.h"
#include "string-util.h"
#include "tokenizer.h"

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */
token_list_t* tokenize(const char* str, const char* delimiters) {
  token_list_t* result = make_array(4);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    if (string_contains(delimiters, ch)) {
      token_data[cpos++] = '\0';
      if (strlen(token_data) > 0) {
        result = token_list_append(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = '\0';
  if (strlen(token_data) > 0) {
    result = token_list_append(result, token_data);
  }

  return result;
}

/**
 * Add a *copy* of the string named data to the token list.
 */
token_list_t* token_list_append(token_list_t* head, const char* data) {
  return array_add(head, (uint64_t) string_duplicate(data));
}

void token_list_free_all(token_list_t* head) {
  // TODO(jawilson): implement me.
}

char* token_list_get(token_list_t* head, int position) {
  return (char*) array_get(head, position);
}

int token_list_length(token_list_t* head) { return array_length(head); }
