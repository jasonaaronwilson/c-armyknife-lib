# @file string-hashtable.c

A hash map of string to a value_t.

It's high unlikely we are close to JVM level of performance in part
because we may be using a slower (but higher quality) hashing
function and this generally does not pay off. We also use chaining
instead of open addressing since this allowed the most code reuse
and a simpler implementation.
 
## @macro string_ht_foreach

Allows traversing all elements of a hashtable in an unspecified
order.
 
## @function make_string_hashtable

Create a hashtable with the given number of buckets. This
implementation currently never grows a hashtable so you may want to
start with a healthy initial capacity.
 
## @function string_ht_delete

Delete an association from the hashtable. It is not an error to
delete a key that doesn't exist in the hashtable.
 
## @function string_ht_find

Find an association in the hashtable.
 
## @function string_ht_insert

Insert an association into the hashtable.
 
## @function string_ht_num_entries

Returns the number of entries in the hashtable.
 
