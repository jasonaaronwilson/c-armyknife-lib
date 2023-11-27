#line 2 "string-alist.c"
/**
 * @file string-alist.c
 *
 * An association list (a type of map) from a string to a value_t.
 */

#ifndef _STRING_ALIST_H_
#define _STRING_ALIST_H_

struct string_alist_S {
  struct string_alist_S* next;
  char* key;
  value_t value;
};

typedef struct string_alist_S string_alist_t;

extern value_result_t alist_find(string_alist_t* list, char* key);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_insert(string_alist_t* list, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_delete(string_alist_t* list, char* key);

/**
 * @macro string_alist_foreach
 *
 * Allows iteration over the keys and values in a string association
 * list.
 */
#define string_alist_foreach(alist, key_var, value_var, statements)            \
  do {                                                                         \
    string_alist_t* head = alist;                                              \
    while (head) {                                                             \
      char* key_var = head->key;                                               \
      value_t value_var = head->value;                                         \
      statements;                                                              \
      head = head->next;                                                       \
    }                                                                          \
  } while (0)

#endif /* _STRING_ALIST_H_ */

/**
 * @function alist_insert
 *
 * Insert a new key and value into an assocation list.
 */
string_alist_t* alist_insert(string_alist_t* list, char* key, value_t value) {
  string_alist_t* result = (malloc_struct(string_alist_t));
  result->next = alist_delete(list, key);
  result->key = key;
  result->value = value;
  return result;
}

/**
 * @function alist_delete
 *
 * Delete the key and associated value from the given association
 * list. Neither the key nor the value associated are themselves
 * freed.
 */
string_alist_t* alist_delete(string_alist_t* list, char* key) {
  // This appears to be logically correct but could easily blow out
  // the stack with a long list.
  if (list == NULL) {
    return list;
  }
  if (strcmp(key, list->key) == 0) {
    string_alist_t* result = list->next;
    free_bytes(list);
    return result;
  }
  list->next = alist_delete(list->next, key);
  return list;
}

/**
 * @function alist_find
 *
 * Find the value associate with the given key. Use is_ok() or
 * is_not_ok() to see if the value is valid (i.e., if the key was
 * actually found).
 */
value_result_t alist_find(string_alist_t* list, char* key) {
  while (list) {
    if (strcmp(key, list->key) == 0) {
      return (value_result_t){.val = list->value};
    }
    list = list->next;
  }
  return (value_result_t){.nf_error = NF_ERROR_NOT_FOUND};
}
