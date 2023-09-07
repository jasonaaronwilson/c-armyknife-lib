#include <stdlib.h>

#include "string-util.h"
#include "symbol-table.h"

symbol NOT_FOUND = {NULL, 0};

symbol_table *add_symbol(symbol_table *table, const char *name,
                         uint64_t value) {
  symbol *sym = find_symbol_by_name(table, name);
  if (string_is_null_or_empty(sym->name)) {
    symbol_table *result = (symbol_table *)(malloc(sizeof(symbol_table)));
    result->next = table;
    result->sym.name = name;
    result->sym.value = value;
    return result;
  }
  sym->value = value;
  return table;
}

symbol *find_symbol_by_name(symbol_table *table, const char *name) {

  while (table) {
    if (string_equal(table->sym.name, name)) {
      return &table->sym;
    }
    table = table->next;
  }

  return &NOT_FOUND;
}

symbol *find_symbol_by_value(symbol_table *table, uint64_t value) {
  while (table) {
    if (table->sym.value == value) {
      return &table->sym;
    }
    table = table->next;
  }

  return &NOT_FOUND;
}
