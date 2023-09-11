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
#include "tagged-pair.h"

tagged_pair_t* make_tagged_pair(tag_t head_tag, uint64_t head, tag_t tail_tag,
                                uint64_t tail) {
  tagged_pair_t* result = malloc_struct(tagged_pair_t);
  result->head = head;
  result->head_tag = head_tag;
  result->tail = tail;
  result->tail_tag = tail_tag;
  return result;
}

uint64_t tagged_pair_list_length(tagged_pair_t* head) {
  uint64_t length = 0;
  while (head) {
    head = head->tail;
    length++;
  }
  return length;
}

uint64_t tagged_pair_list_get(tagged_pair_t* head, uint64_t index) {
  uint64_t length = 0;
  while (head) {
    if (length == index) {
      return head->head;
    }
    head = head->tail;
    length++;
  }
  fatal_error(ERROR_ILLEGAL_LIST_INDEX);
}

uint64_t tagged_pair_list_get_type(tagged_pair_t* head, uint64_t index) {
  uint64_t length = 0;
  while (head) {
    if (length == index) {
      return head->head_tag;
    }
    head = head->tail;
    length++;
  }
  fatal_error(ERROR_ILLEGAL_LIST_INDEX);
}

void tagged_pair_list_set(tagged_pair_t* head, uint64_t index, tag_t tag,
                          uint64_t element) {
  uint64_t length = 0;
  while (head) {
    if (length == index) {
      head->head = element;
      head->head_tag = tag;
      return;
    }
    length++;
  }
  fatal_error(ERROR_ILLEGAL_LIST_INDEX);
}

// TODO(jawilson): tagget_pair_alist_find, tagget_pair_alist_insert,
// tagget_pair_alist_remove

tagged_pair_t* tagged_pair_list_apppend(tagged_pair_t* lst, tag_t tag,
                                        uint64_t element) {
  tagged_pair_t* head = lst;
  tagged_pair_t* pair = make_tagged_pair(tag, element, TAG_NULL, 0);

  if (head) {
    while (head->tail) {
      head = head->tail;
    }
    head->tail = pair;
    head->tail_tag = TAG_TAGGED_PAIR_T;
    return lst;
  } else {
    return pair;
  }
}
