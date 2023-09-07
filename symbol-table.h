#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdint.h>

typedef struct symbol_table_S {
  struct symbol_table_S *next;
  char *name;
  uint64_t value;
} symbol_table;

typedef struct {
  char *name;
  uint64_t value;
} symbol;

extern symbol_table *add_symbol(symbol_table *table, char *name,
                                uint64_t value);
extern symbol find_symbol_by_name(symbol_table *table, char *name);
extern symbol find_symbol_by_value(symbol_table *table, uint64_t value);

#endif /* _SYMBOL_TABLE_H_ */
