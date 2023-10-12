#line 2 "string-hashtable.c"
/**
 * @file string-hashtable.c
 *
 * A hash map of string to a value_t.
 */

#ifndef _STRING_HASHTABLE_H_
#define _STRING_HASHTABLE_H_

struct string_hashtable_S {
  uint64_t n_buckets;
  // TODO(jawilson): keep track of the number of entries in the
  // hashtable so we can automtically grow the hashtable.
  string_alist_t* buckets[0];
};

typedef struct string_hashtable_S string_hashtable_t;

extern string_hashtable_t* make_string_hashtable(uint64_t n_buckets);

extern string_hashtable_t* string_ht_insert(string_hashtable_t* ht, char* key,
                                            value_t value);

extern string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

#define string_ht_foreach(ht, key_var, value_var, statements)                  \
  do {                                                                         \
    for (int ht_index = 0; ht_index < ht->n_buckets; ht_index++) {             \
      string_alist_t* alist = ht->buckets[ht_index];                           \
      if (alist != NULL) {                                                     \
        string_alist_foreach(alist, key_var, value_var, statements);           \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif /* _STRING_HASHTABLE_H_ */

/**
 * Create a hashtable with the given initial capacity.
 */
string_hashtable_t* make_string_hashtable(uint64_t n_buckets) {
  if (n_buckets == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }
  string_hashtable_t* result = (string_hashtable_t*) (malloc_bytes(
      sizeof(string_hashtable_t) + sizeof(string_alist_t*) * n_buckets));
  result->n_buckets = n_buckets;
  return result;
}

/**
 * Insert an association into the hashtable.
 */
string_hashtable_t* string_ht_insert(string_hashtable_t* ht, char* key,
                                     value_t value) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_insert(list, key, value);
  return ht;
}

/**
 * Delete an association from the hashtable.
 */
string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_delete(list, key);
  return ht;
}

/**
 * Find an association in the hashtable.
 */
value_result_t string_ht_find(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  return alist_find(list, key);
}
