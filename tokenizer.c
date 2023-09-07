#include <stdlib.h>
#include <string.h>

#include "fatal-error.h"
#include "tokenizer.h"

token_list *tokenize(const char *str, const char *delimiters) {
  token_list *result = NULL;
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    // FIXME(jawilson): look at all delimiters!
    if (ch == delimiters[0]) {
      token_data[cpos++] = 0;
      if (strlen(token_data) > 0) {
        result = token_list_append(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = 0;
  if (strlen(token_data) > 0) {
    result = token_list_append(result, token_data);
  }
  cpos = 0;

  return result;
}

token_list *token_list_append(token_list *head, const char *data) {
  token_list *node = (token_list *)(malloc(sizeof(token_list)));
  node->next = NULL;
  node->data = strdup(data);
  if (head == NULL) {
    return node;
  }
  token_list *lst = head;
  while (lst->next != NULL) {
    lst = lst->next;
  }
  lst->next = node;
  return head;
}

void token_list_free_all(token_list *head) {
  // TODO(jawilson): implement me.
}

char *token_list_get(token_list *head, int position) {
  while (position > 0 && head != NULL) {
    head = head->next;
    position--;
  }
  if (head == NULL) {
    fatal_error(ERROR_TOKEN_LIST_GET);
  } else {
    return head->data;
  }
}

int token_list_length(token_list *head, int position) {
  int result = 0;
  while (head != NULL) {
    result++;
    head = head->next;
  }
  return result;
}
