# @file value-hashtable.c

A very thread-unsafe hash map of C style zero terminated byte
"strings" to a value_t.

Please don't expect C++, JVM, or Rust level of performance for at
least these reasons:

1) We are probably using a slower (but higher quality) hash
function than they use for strings, a design decision I made to
have a single "good enough" hash function to use everywhere in this
library because I assume you will find the right cryptography
library to meet those needs.

2) Probably a bigger impact on modern processors is that we use
chaining which is considered less friendly than open addressing and
other techniques. However, I wanted an implementation that was
simple and reusing string_alist_t seems to have done the trick.
 
## @macro string_ht_foreach

Allows traversing all elements of a hashtable in an unspecified
order.
 
## @function make_string_hashtable

Create a hashtable with the given number of buckets.

The minimum number of buckets is currently 2 to make it less likely
we run into some resize loop depending on the values of
ARMYKNIFE_HT_LOAD_FACTOR and AK_HT_UPSCALE_MULTIPLIER).
 
## @function string_hashtable_upsize_internal

This function is called automatically when an insert brings the
number of entries above the number of buckets times
ARMYKNIFE_HT_LOAD_FACTOR (defaults to 75%). We don't even check
that constraint is valid (hence the _internal suffix).

Hopefully based on the name you can infer this function will only
ever "grow" a hashtable by deciding on a size of the new larger
hash-table and copying
by making a new larger hashtable using
AK_HT_UPSCALE_MULTIPLIER to compute the new number of buckets
(currently 1.75).
 
## @function string_ht_delete

Delete an association from the hashtable. It is not an error to
delete a key that doesn't exist in the hashtable.
 
## @function string_ht_find

Find an association in the hashtable.
 
## @function string_ht_insert

Insert an association into the hashtable.
 
## @function string_ht_num_entries

Returns the number of entries in the hashtable.
 
## @compiliation_option AK_HT_UPSCALE_MULTIPLIER

In all cases this should be a number > 1.0.
 
## @compiliation_option ARMYKNIFE_HT_LOAD_FACTOR

The "load factor" is the ratio of the number of keys in the hash
table to the most optimistic capacity for the table if every key
happened to be hashed to a different bucket. When the load factor
reaches this value, the hash table will be resized to a larger
capacity to improve performance. A higher value allows for a denser
hash table but can lead to more collisions and slower lookups and
insertions. A lower value wastes memory but reduces collisions.
 
