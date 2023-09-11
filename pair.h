#ifndef _PAIR_H_
#define _PAIR_H_

#include "optional.h"
#include "tagged-reference.h"

typedef struct {
  tagged_reference_t head;
  tagged_reference_t tail;
} pair_t;

extern pair_t* make_pair(tagged_reference_t head, tagged_reference_t tail);

extern uint64_t pair_list_length(pair_t* head);
extern tagged_reference_t pair_list_get(pair_t* head, uint64_t index);
extern void pair_list_set(pair_t* head, uint64_t index,
                          tagged_reference_t element);
extern pair_t* pair_list_append(pair_t* lst1, pair_t* lst2);
extern optional_t pair_assoc_list_lookup(pair_t* lst, char* name);

static inline pair_t* untag_pair(tagged_reference_t reference) {
  require_tag(reference, TAG_PAIR_T);
  return (pair_t*) reference.data;
}

#endif /* _TAGGED_PAIR_H_ */
