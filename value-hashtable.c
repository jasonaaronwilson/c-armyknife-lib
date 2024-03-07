#line 2 "value-hashtable.c"
/**
 * @file value-hashtable.c
 *
 * A very thread-unsafe hash map of C style zero terminated byte
 * "strings" to a value_t.
 *
 * Please don't expect C++, JVM, or Rust level of performance for at
 * least these reasons:
 *
 * 1) We are probably using a slower (but higher quality) hash
 * function than they use for strings, a design decision I made to
 * have a single "good enough" hash function to use everywhere in this
 * library because I assume you will find the right cryptography
 * library to meet those needs.
 *
 * 2) Probably a bigger impact on modern processors is that we use
 * chaining which is considered less friendly than open addressing and
 * other techniques. However, I wanted an implementation that was
 * simple and reusing string_alist_t seems to have done the trick.
 */

#ifndef _STRING_HASHTABLE_H_
#define _STRING_HASHTABLE_H_

/**
 * @compiliation_option ARMYKNIFE_HT_LOAD_FACTOR
 *
 * The "load factor" is the ratio of the number of keys in the hash
 * table to the most optimistic capacity for the table if every key
 * happened to be hashed to a different bucket. When the load factor
 * reaches this value, the hash table will be resized to a larger
 * capacity to improve performance. A higher value allows for a denser
 * hash table but can lead to more collisions and slower lookups and
 * insertions. A lower value wastes memory but reduces collisions.
 */
#ifndef ARMYKNIFE_HT_LOAD_FACTOR
#define ARMYKNIFE_HT_LOAD_FACTOR 0.75
#endif /* ARMYKNIFE_HT_LOAD_FACTOR */

/**
 * @compiliation_option AK_HT_UPSCALE_MULTIPLIER
 *
 * In all cases this should be a number > 1.0.
 */
#ifndef AK_HT_UPSCALE_MULTIPLIER
#define AK_HT_UPSCALE_MULTIPLIER 1.75
#endif /* AK_HT_UPSCALE_MULTIPLIER */

struct string_hashtable_S {
  uint64_t n_buckets;
  uint64_t n_entries;
  string_alist_t* buckets[0];
};

typedef struct string_hashtable_S string_hashtable_t;

extern string_hashtable_t* make_string_hashtable(uint64_t n_buckets);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_insert(string_hashtable_t* ht, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_hashtable_upsize_internal(string_hashtable_t* ht);

/**
 * @function string_ht_num_entries
 *
 * Returns the number of entries in the hashtable.
 */
static inline uint64_t string_ht_num_entries(string_hashtable_t* ht) {
  return ht->n_entries;
}

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
 * Create a hashtable with the given number of buckets.
 *
 * The minimum number of buckets is currently 2 to make it less likely
 * we run into some resize loop depending on the values of
 * ARMYKNIFE_HT_LOAD_FACTOR and AK_HT_UPSCALE_MULTIPLIER).
 */
string_hashtable_t* make_string_hashtable(uint64_t n_buckets) {
  if (n_buckets < 2) {
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
  uint64_t len = alist_length(list);
  list = alist_insert(list, key, value);
  ht->buckets[bucket] = list;
  uint64_t len_after = alist_length(list);
  if (len_after > len) {
    ht->n_entries++;
    // Without this, a hash table would never grow and thus as the
    // number of entries grows large, the hashtable would only improve
    // performance over an alist by a constant amount (which could
    // still be an impressive speedup...)
    if (ht->n_entries >= (ht->n_buckets * ARMYKNIFE_HT_LOAD_FACTOR)) {
      ht = string_hashtable_upsize_internal(ht);
    }
  }
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
  uint64_t len = alist_length(list);
  list = alist_delete(list, key);
  ht->buckets[bucket] = list;
  uint64_t len_after = alist_length(list);
  if (len_after < len) {
    ht->n_entries--;
  }
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

/**
 * @function string_hashtable_upsize_internal
 *
 * This function is called automatically when an insert brings the
 * number of entries above the number of buckets times
 * ARMYKNIFE_HT_LOAD_FACTOR (defaults to 75%). We don't even check
 * that constraint is valid (hence the _internal suffix).
 *
 * Hopefully based on the name you can infer this function will only
 * ever "grow" a hashtable by deciding on a size of the new larger
 * hash-table and copying

by making a new larger hashtable using
 * AK_HT_UPSCALE_MULTIPLIER to compute the new number of buckets
 * (currently 1.75).
 */
string_hashtable_t* string_hashtable_upsize_internal(string_hashtable_t* ht) {
  uint64_t new_num_buckets = ht->n_buckets * AK_HT_UPSCALE_MULTIPLIER;
  string_hashtable_t* result = make_string_hashtable(new_num_buckets);
  // clang-format off
  string_ht_foreach(ht, key, value, {
      string_hashtable_t* should_be_result = string_ht_insert(result, key, value);
      // If an insertion into the bigger hashtable results in it's own
      // resize, then the results will be unpredictable (at least
      // without more code). This is likely to only happen when
      // growing a very small hashtable and depends on values choosen
      // for ARMYKNIFE_HT_LOAD_FACTOR and AK_HT_UPSCALE_MULTIPLIER.
      if (result != should_be_result) {
	fatal_error(ERROR_ILLEGAL_STATE);
      }
  });
  free_bytes(ht);
  // clang-format on
  return result;
}
