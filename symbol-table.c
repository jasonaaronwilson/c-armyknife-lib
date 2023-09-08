#include <stdlib.h>
#include <string.h>

#include "string-util.h"
#include "symbol-table.h"

symbol_table *add_symbol(symbol_table *table, const char *name,
                         uint64_t value) {
  symbol *sym = find_symbol_by_name(table, name);
  if (sym == NULL) {
    symbol_table *result = (symbol_table *)(malloc(sizeof(symbol_table)));
    result->next = table;
    result->sym.name = strdup(name);
    result->sym.value = value;
    result->sym.previous_value = value - 1;
    return result;
  } else {
    sym->previous_value = sym->value;
    sym->value = value;
  }
  return table;
}

symbol *find_symbol_by_name(symbol_table *table, const char *name) {

  while (table) {
    if (string_equal(table->sym.name, name)) {
      return &table->sym;
    }
    table = table->next;
  }

  return NULL;
}

symbol *find_symbol_by_value(symbol_table *table, uint64_t value) {
  while (table) {
    if (table->sym.value == value) {
      return &table->sym;
    }
    table = table->next;
  }

  return NULL;
}
