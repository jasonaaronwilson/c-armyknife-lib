#ifndef _TAGGED_PAIR_H_
#define _TAGGED_PAIR_H_

#include "tagged-reference.h"

typedef struct {
  tagged_reference_t head;
  tagged_reference_t tail;
} tagged_pair_t;

extern tagged_pair_t* make_tagged_pair(tagged_reference_t head,
                                       tagged_reference_t tail);

extern uint64_t tagged_pair_list_length(tagged_pair_t* head);
extern tagged_reference_t tagged_pair_list_get(tagged_pair_t* head,
                                               uint64_t index);
extern void tagged_pair_list_set(tagged_pair_t* head, uint64_t index,
                                 tagged_reference_t element);
extern tagged_pair_t* tagged_pair_list_append(tagged_pair_t* lst1,
                                              tagged_pair_t* lst2);

#endif /* _TAGGED_PAIR_H_ */
