#ifndef _PAIR_H_
#define _PAIR_H_

#include "tagged-reference.h"

typedef struct {
  tagged_reference_t head;
  tagged_reference_t tail;
} pair_t;

extern pair_t* make_pair(tagged_reference_t head, tagged_reference_t tail);

extern uint64_t tagged_pair_list_length(pair_t* head);
extern tagged_reference_t tagged_pair_list_get(pair_t* head, uint64_t index);
extern void tagged_pair_list_set(pair_t* head, uint64_t index,
                                 tagged_reference_t element);
extern pair_t* tagged_pair_list_append(pair_t* lst1, pair_t* lst2);

#endif /* _TAGGED_PAIR_H_ */
