#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "string-util.h"
#include "symbol-table.h"

symbol_table *add_symbol(symbol_table *table, const char *name,
                         uint64_t value) {
  symbol *sym = find_symbol_by_name(table, name);
  if (sym == NULL) {
    symbol_table *result = malloc_struct(symbol_table);
    result->next = table;
    result->sym.name = string_duplicate(name);
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

uint8_t is_dirty(symbol_table *table) {
  while (table) {
    if (table->sym.value != table->sym.previous_value) {
      return 1;
    }
    table = table->next;
  }

  return 0;
}
