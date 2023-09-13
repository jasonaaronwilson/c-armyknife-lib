#ifndef _SCHEME_SYMBOL_
#define _SCHEME_SYMBOL_

#include "tagged-reference.h"

static inline char* untag_scheme_symbol(tagged_reference_t symbol) {
  require_tag(symbol, TAG_SCHEME_SYMBOL);
  return (char*) symbol.data;
}

#endif /* _SCHEME_SYMBOL_ */
