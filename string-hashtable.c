#line 2 "string-hashtable.c"
/**
 * @file string-hashtable.c
 *
 * A hash map of string to a value_t.
 *
 * It's high unlikely we are close to JVM level of performance in part
 * because we may be using a slower (but higher quality) hashing
 * function and this generally does not pay off. We also use chaining
 * instead of open addressing since this allowed the most code reuse
 * and a simpler implementation.
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

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_insert(string_hashtable_t* ht, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

/**
 * @macro string_ht_foreach
 *
 * Allows traversing all elements of a hashtable in an unspecified
 * order.
 */
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
 * @function make_string_hashtable
 *
 * Create a hashtable with the given number of buckets. This
 * implementation currently never grows a hashtable so you may want to
 * start with a healthy initial capacity.
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
 * @function string_ht_insert
 *
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
 * @function string_ht_delete
 *
 * Delete an association from the hashtable. It is not an error to
 * delete a key that doesn't exist in the hashtable.
 */
string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_delete(list, key);
  return ht;
}

/**
 * @function string_ht_find
 *
 * Find an association in the hashtable.
 */
value_result_t string_ht_find(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  return alist_find(list, key);
}
