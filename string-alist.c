#line 2 "string-alist.c"
/**
 * @file string-alist.c
 *
 * A simple map between a string with a pointer to some other
 * data-structure.
 */

#ifndef _STRING_ALIST_H_
#define _STRING_ALIST_H_

struct string_alist_S {
  struct string_alist_S* next;
  char* key;
  void* value;
};

typedef struct string_alist_S string_alist_t;

extern string_alist_t* alist_insert(string_alist_t* list, char* key,
                                    void* value);
extern string_alist_t* alist_delete(string_alist_t* list, char* key);
extern void* alist_find(string_alist_t* list, char* key);

#endif /* _STRING_ALIST_H_ */

string_alist_t* alist_insert(string_alist_t* list, char* key, void* value) {
  string_alist_t* result = (malloc_struct(string_alist_t));
  result->next = alist_delete(list, key);
  result->key = key;
  result->value = value;
  return result;
}

string_alist_t* alist_delete(string_alist_t* list, char* key) {
  // This appears to be logically correct but could easily blow out
  // the stack with a long list.
  if (list == NULL) {
    return list;
  }
  if (strcmp(key, list->key) == 0) {
    string_alist_t* result = list->next;
    free(list);
    return result;
  }
  list->next = alist_delete(list->next, key);
  return list;
}

void* alist_find(string_alist_t* list, char* key) {
  while (list) {
    if (strcmp(key, list->key) == 0) {
      return list->value;
    }
    list = list->next;
  }
  return NULL;
}
