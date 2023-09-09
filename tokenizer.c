#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "fatal-error.h"
#include "string-util.h"
#include "tokenizer.h"

token_list_t* tokenize(const char* str, const char* delimiters) {
  token_list_t* result = NULL;
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

token_list_t* token_list_append(token_list_t* head, const char* data) {
  token_list_t* node = malloc_struct(token_list_t);
  node->next = NULL;
  node->data = string_duplicate(data);
  if (head == NULL) {
    return node;
  }
  token_list_t* lst = head;
  while (lst->next != NULL) {
    lst = lst->next;
  }
  lst->next = node;
  return head;
}

void token_list_free_all(token_list_t* head) {
  // TODO(jawilson): implement me.
}

char* token_list_get(token_list_t* head, int position) {
  while (position > 0 && head != NULL) {
    head = head->next;
    position--;
  }
  if (head == NULL) {
    // fatal_error(ERROR_TOKEN_LIST_GET);
    return NULL;
  } else {
    return head->data;
  }
}

int token_list_length(token_list_t* head) {
  int result = 0;
  while (head != NULL) {
    result++;
    head = head->next;
  }
  return result;
}
