#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include "tagged-reference.h"

typedef int boolean_t;

#define true ((boolean_t) 1)
#define false ((boolean_t) 0)

inline boolean_t untag_boolean(tagged_reference_t reference) {
  require_tag(reference, TAG_BOOLEAN_T);
  return (boolean_t) reference.data;
}

#endif /* _BOOLEAN_H_ */
