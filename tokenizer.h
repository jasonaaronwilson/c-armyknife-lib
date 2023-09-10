#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

typedef array_t token_list_t;

extern token_list_t* tokenize(const char* str, const char* delimiters);
extern token_list_t* token_list_append(token_list_t* head, const char* data);
extern void token_list_free_all(token_list_t* head);
extern char* token_list_get(token_list_t* head, int position);
extern int token_list_length(token_list_t* head);

#endif /* _TOKENIZER_H_ */
