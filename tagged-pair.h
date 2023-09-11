#ifndef _TAGGED_PAIR_H_
#define _TAGGED_PAIR_H_

#include "tagged-reference.h"

typedef struct {
  uint64_t head;
  uint64_t tail;
  uint64_t head_tag;
  uint64_t tail_tag;
} tagged_pair_t;

extern tagged_pair_t* make_tagged_pair(tag_t head_tag, uint64_t head,
                                       tag_t tail_tag, uint64_t tail);

extern uint64_t tagged_pair_list_length(tagged_pair_t* head);
extern uint64_t tagged_pair_list_get(tagged_pair_t* head, uint64_t index);
extern uint64_t tagged_pair_list_get_type(tagged_pair_t* head, uint64_t index);
extern void tagged_pair_list_set(tagged_pair_t* head, uint64_t index, tag_t tag,
                                 uint64_t element);
extern tagged_pair_t* tagged_pair_list_apppend(tagged_pair_t* lst, tag_t tag,
                                               uint64_t element);

#endif /* _TAGGED_PAIR_H_ */
