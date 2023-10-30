# @file string-tree.c

This is a balanced binary tree to associate a string and a value.

Generally a string_alist is prefered for small "maps", and
string_hashtable is prefered for large maps, but string_tree is the
easiest way to get sorted results (which is most important for
reproducibility).

Currently we are using "AA" trees (see
https://en.wikipedia.org/wiki/AA_tree) since it has simpler code
than many other balanced trees like RB trees and the Wikipedia
article and paper spell out most of the non-trivial details.
 
## @function string_tree_delete

Delete the association of key (if it exists in the tree).
 
## @function string_tree_find

Find the value associate with the key in the tree.
 
## @function string_tree_insert

Insert an association of key and a value.
 
## @macro string_tree_foreach

Perform an inorder traversal of a string-tree.

key_var is created in a new block scope with type char*.

value_var is created in a new block scope with type value_t and you
will probably want to use something like ".ptr" or ".u64" on the
value to obtain the actual value.

statements should be a normal C block, aka, something like:
```
{
  statement1();
  statement2();
}
```
 
