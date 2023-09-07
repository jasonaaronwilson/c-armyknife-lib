#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

typedef struct token_list_S {
  struct token_list_S *next;
  char *data;
} token_list;

extern token_list *token_list_append(token_list *head, char *data);
extern void token_list_free_all(token_list *head);
extern char *token_list_get(token_list *head, int position);
extern int token_list_length(token_list *head, int position);

#endif /* _TOKENIZER_H_ */
