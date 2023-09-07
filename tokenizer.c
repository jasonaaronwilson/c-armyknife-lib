#include "tokenizer.h"
#include "fatal-error.h"
#include <stdlib.h>

token_list *token_list_append(token_list *head, char *data) {
  token_list *node = (token_list *)(malloc(sizeof(token_list)));
  node->next = NULL;
  node->data = data;
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
