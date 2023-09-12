#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "pair.h"
#include "tagged-reference.h"

// tagged_reference_t some_primitive(pair_t* args)

typedef tagged_reference_t (*primitive_t)(pair_t* args);

#endif /* _PRIMITIVE_H_ */
