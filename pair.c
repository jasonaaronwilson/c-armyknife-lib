/**
 * @file tagged-pair.c
 *
 * tagged pairs are used to build up lists or trees of elements. These
 * are primarily utilized by the tokenizer, reader and the repl
 * interpreter where the flexibility of dynamic typing outweighs any
 * performance concerns.
 */

#include <stdint.h>

#include "allocate.h"
#include "fatal-error.h"
#include "pair.h"

pair_t* make_pair(tagged_reference_t head, tagged_reference_t tail) {
  pair_t* result = malloc_struct(pair_t);
  result->head = head;
  result->tail = tail;
  return result;
}

uint64_t tagged_pair_list_length(pair_t* lst) {
  uint64_t length = 0;
  while (lst) {
    lst = (pair_t*) (lst->tail.data);
    length++;
  }
  return length;
}

tagged_reference_t tagged_pair_list_get(pair_t* lst, uint64_t index) {
  uint64_t length = 0;
  while (lst) {
    if (length == index) {
      return lst->head;
    }
    lst = (pair_t*) (lst->tail.data);
    length++;
  }
  fatal_error(ERROR_ILLEGAL_LIST_INDEX);
}

void tagged_pair_list_set(pair_t* head, uint64_t index,
                          tagged_reference_t element) {
  uint64_t length = 0;
  while (head) {
    if (length == index) {
      head->head = element;
      return;
    }
    length++;
  }
  fatal_error(ERROR_ILLEGAL_LIST_INDEX);
}

pair_t* tagged_pair_list_append(pair_t* lst_1, pair_t* lst_2) {
  if (lst_1 && lst_2) {
    pair_t* head = lst_1;
    while (head->tail.data) {
      head = (pair_t*) head->tail.data;
    }
    head->tail = (tagged_reference_t){lst_2, TAG_PAIR_T};
    return lst_1;
  } else if (lst_1) {
    return lst_1;
  } else {
    return lst_2;
  }
}

// TODO(jawilson): tagged_pair_alist_find, tagged_pair_alist_insert,
// tagged_pair_alist_remove
