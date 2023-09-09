#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdint.h>

typedef struct {
  const char* name;
  uint64_t value;
  uint64_t previous_value;
} symbol_t;

typedef struct symbol_table_S {
  struct symbol_table_S* next;
  symbol_t sym;
} symbol_table_t;

extern symbol_table_t* add_symbol(symbol_table_t* table, const char* name,
                                  uint64_t value);
extern symbol_t* find_symbol_by_name(symbol_table_t* table, const char* name);
extern symbol_t* find_symbol_by_value(symbol_table_t* table, uint64_t value);
extern uint8_t is_dirty(symbol_table_t* table);

#endif /* _SYMBOL_TABLE_H_ */
