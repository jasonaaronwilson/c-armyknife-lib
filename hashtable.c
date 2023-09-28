#line 2 "hashtable.c"
/**
 * @file hashtable.c
 *
 * A simple hashtable from keys to values.
 */

// ======================================================================
// This is block is extraced to hashtable.h
// ======================================================================

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdint.h>

typedef struct {
  type_t* type;
  array_of_type(tuple_of_type(uint64_type(), key_type, value_type))* storage;
  uint64_t number_of_keys;
} hashtable_t;

extern hashtable_t* make_hashtable(type_t* key_type, type_t* value_type, uint32_t initial_capacity);
extern uint64_t hashtable_number_of_keys(hashtable_t* hashtable);
extern reference_t hashtable_get_reference_to_value(hashtable_t* hashtable, reference_t key_reference);
extern void hashtable_set_value(hashtable_t* ht, reference_t key_reference, reference_t value_reference);
extern int hashtable_compare(hashtable_t* a, hashtable_t* b);

#endif /* _HASHTABLE_H_ */

// ======================================================================

#include <stdlib.h>

#define HT_ENTRY_HASHCODE_TUPLE_POSITION 0
#define HT_ENTRY_KEY_TUPLE_POSITION 1
#define HT_ENTRY_VALUE_TUPLE_POSITION 2

/**
 * Make an array with the given initial_capacity.
 */
hashtable_t* make_hashtable(type_t* key_type, type_t* value_type, uint32_t initial_capacity) {
  TRACE();

  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  byte_array_t* name = make_byte_array(32);
  name = byte_array_append_string(name, "hashtable(");
  name = byte_array_append_string(name, key_type->name);
  name = byte_array_append_string(name, ",");
  name = byte_array_append_string(name, value_type->name);
  name = byte_array_append_string(name, ")");

  type_t* hashtable_type = malloc_struct(type_t);
  hashtable_type->name = byte_array_c_substring(name, 0, byte_array_length(name));
  hashtable_type->size = sizeof(hashtable_t);
  // ...

  type_t* storage_type = intern_tuple_type(uint64_type(), key_type, value_type);
  hashtable_t* result = (hashtable_t*) (malloc_bytes(hashtable_type->size));
  result->storage = HERE;

  return result;
}

/**
 * Return the number of actual entries in an array.
 */
uint64_t hashtable_size(hashtable_t* ht) { return ht->number_of_keys; }

/**
 * Lookup a key in a hashtable.
 */
reference_t hashtable_get_reference_to_value(hashtable_t* ht, reference_t key_reference) {
  TRACE();

  uint64_t hash = ht->key_type->hash_fn(key_reference);
  uint64_t position = hash % ht->keys->length;
  reference_t stored_key_reference = array_get_reference(ht->keys, position);
  if (!reference_eq(stored_key_reference, null_ptr_reference())) {
    // check hashcode and then key equality!!!
    return array_get_reference(ht->values, position);
  }
  return null_ptr_reference();
}

void hashtable_set_value(hashtable_t* ht, reference_t key_reference, reference_t value_reference) {
  TRACE();

  uint64_t hash = ht->key_type->hash_fn(key_reference);
  uint64_t position = hash % ht->keys->length;
  reference_t stored_key_reference = array_get_reference(ht->keys, position);
  // TODO(jawilson):
  if (reference_eq(stored_key_reference, null_ptr_reference())) {
    array_set(ht->keys, position, value_reference);
  } else {
    hashtable_t* new_ht = make_hashtable(ht->key_type, ht->value_type, ht->keys->capacity);
    for (int i = 0; (i < array_length(ht->storage)); i++) {
      reference_t entry_reference = array_get_reference(ht->storage, i);
      if (!reference_is_null(tuple_get_ref

      tuple_t* entry = 
      hashtable_set_value(new_ht, array_get_reference(ht->keys, i), array_get_reference(ht->values, i));
    }
    free(ht->keys);
    free(ht->values);
    ht->keys = new_ht->keys;
    ht->values = new_ht->values;
    free(new_ht);
    hashtable_set_value(ht, key_reference, value_reference);
  }
}
