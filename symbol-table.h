#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdint.h>

typedef struct {
  const char *name;
  uint64_t value;
  uint64_t previous_value;
} symbol;

typedef struct symbol_table_S {
  struct symbol_table_S *next;
  symbol sym;
} symbol_table;

extern symbol_table *add_symbol(symbol_table *table, const char *name,
                                uint64_t value);
extern symbol *find_symbol_by_name(symbol_table *table, const char *name);
extern symbol *find_symbol_by_value(symbol_table *table, uint64_t value);
extern uint8_t is_dirty(symbol_table *table);

#endif /* _SYMBOL_TABLE_H_ */
