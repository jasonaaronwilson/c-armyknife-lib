#ifndef _OPTIONAL_H_
#define _OPTIONAL_H_

#include "boolean.h"
#include "tagged-reference.h"

typedef struct {
  tagged_reference_t reference;
  boolean_t is_present;
} optional_t;

#define optional_empty() ((optional_t){NIL, false})
#define optional_of(value) ((optional_t){value, true})
#define optional_is_present(optional) (optional.is_present)
#define optional_value(optional) (optional.reference)

#endif /* _OPTIONAL_H_ */
