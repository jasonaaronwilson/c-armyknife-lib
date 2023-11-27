#line 2 "string-tree.c"

/**
 * @file string-tree.c
 *
 * This is a balanced binary tree to associate a string to a value.
 *
 * Generally a string_alist is prefered for small "maps", and
 * string_hashtable is prefered for large maps, but string_tree is the
 * easiest way to get sorted results (which is most important for
 * reproducibility).
 *
 * Currently we are using "AA" trees (see
 * https://en.wikipedia.org/wiki/AA_tree) since it has simpler code
 * than many other balanced trees (like red-block trees) and the
 * Wikipedia article and paper spell out *most* of the non-trivial
 * details.
 */

#ifndef _STRING_TREE_H_
#define _STRING_TREE_H_

struct string_tree_S {
  char* key;
  value_t value;
  uint32_t level;
  struct string_tree_S* left;
  struct string_tree_S* right;
};

typedef struct string_tree_S string_tree_t;

extern value_result_t string_tree_find(string_tree_t* t, char* key);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_insert(string_tree_t* t, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_delete(string_tree_t* t, char* key);

/**
 * @macro string_tree_foreach
 *
 * Perform an inorder traversal of a string-tree.
 *
 * key_var is created in a new block scope with type char*.
 *
 * value_var is created in a new block scope with type value_t and you
 * will probably want to use something like ".ptr" or ".u64" on the
 * value to obtain the actual value.
 *
 * statements should be a normal C block, aka, something like:
 * ```
 * {
 *   statement1();
 *   statement2();
 * }
 * ```
 *
 * Unforunately it is not possible to use "break" or "continue" with
 * this style of loop (and worse, there will be no compilation error
 * or warning if you accidentally do that...)
 */
#define string_tree_foreach(tree, key_var, value_var, statements)              \
  do {                                                                         \
    int stack_n_elements = 0;                                                  \
    string_tree_t* stack[64];                                                  \
    string_tree_t* current = tree;                                             \
    while (current != NULL || stack_n_elements > 0) {                          \
      while (current != NULL) {                                                \
        stack[stack_n_elements++] = current;                                   \
        current = current->left;                                               \
      }                                                                        \
      current = stack[--stack_n_elements];                                     \
      char* key_var = current->key;                                            \
      value_t value_var = current->value;                                      \
      statements;                                                              \
      current = current->right;                                                \
    }                                                                          \
  } while (0)

#endif /* _STRING_TREE_H_ */

/**
 * @function string_tree_find
 *
 * Find the value associate with the key in the tree.
 */
value_result_t string_tree_find(string_tree_t* t, char* key) {
  if (t == NULL) {
    return (value_result_t){.nf_error = NF_ERROR_NOT_FOUND};
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    return string_tree_find(t->left, key);
  } else if (cmp_result > 0) {
    return string_tree_find(t->right, key);
  } else {
    return (value_result_t){
        .val = t->value,
    };
  }
}

string_tree_t* string_tree_skew(string_tree_t* t) {
  if (t == NULL) {
    return NULL;
  }
  if (t->left == NULL) {
    return t;
  }
  if (t->left->level == t->level) {
    string_tree_t* L = t->left;
    t->left = L->right;
    L->right = t;
    return L;
  }
  return t;
}

string_tree_t* string_tree_split(string_tree_t* t) {
  if (t == NULL) {
    return NULL;
  }
  if (t->right == NULL || t->right->right == NULL) {
    return t;
  }
  if (t->level == t->right->right->level) {
    // We have two horizontal right links.  Take the middle node,
    // elevate it, and return it.
    string_tree_t* R = t->right;
    t->right = R->left;
    R->left = t;
    R->level++;
    return R;
  }
  return t;
}

string_tree_t* make_string_tree_leaf(char* key, value_t value) {
  string_tree_t* result = malloc_struct(string_tree_t);
  result->level = 1;
  result->key = key;
  result->value = value;
  return result;
}

/**
 * @function string_tree_insert
 *
 * Insert an association of key and a value (or update the current
 * value stored in the tree).
 */
string_tree_t* string_tree_insert(string_tree_t* t, char* key, value_t value) {
  if (t == NULL) {
    // Create a new leaf node
    return make_string_tree_leaf(key, value);
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    t->left = string_tree_insert(t->left, key, value);
  } else if (cmp_result > 0) {
    t->right = string_tree_insert(t->right, key, value);
  } else {
    // Either key or t->key might need to be freed but it isn't even
    // possible to tell if either has been "malloced" so good luck
    // figuring that out.
    t->value = value;
    return t;
  }

  t = string_tree_skew(t);
  t = string_tree_split(t);

  return t;
}

static inline uint64_t min_level(uint32_t a, uint32_t b) {
  return a < b ? a : b;
}

string_tree_t* string_tree_decrease_level(string_tree_t* t) {
  if (t->left && t->right) {
    uint32_t should_be = min_level(t->left->level, t->right->level) + 1;
    if (should_be < t->level) {
      t->level = should_be;
      if (should_be < t->right->level) {
        t->right->level = should_be;
      }
    }
  }
  return t;
}

string_tree_t* string_tree_predecessor(string_tree_t* t) {
  t = t->left;
  while (t->right != NULL) {
    t = t->right;
  }
  return t;
}

string_tree_t* string_tree_successor(string_tree_t* t) {
  t = t->right;
  while (t->left != NULL) {
    t = t->left;
  }
  return t;
}

static inline boolean_t string_tree_is_leaf(string_tree_t* t) {
  return t->left == NULL && t->right == NULL;
}

/**
 * @function string_tree_delete
 *
 * Delete the association of key (if it exists in the tree). It is not
 * an error to delete a key that isn't present in the table.
 */
string_tree_t* string_tree_delete(string_tree_t* t, char* key) {

  if (t == NULL) {
    return t;
  }

  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    t->left = string_tree_delete(t->left, key);
  } else if (cmp_result > 0) {
    t->right = string_tree_delete(t->right, key);
  } else {
    if (string_tree_is_leaf(t)) {
      // Since we are a leaf, nothing special to do except make sure
      // this leaf node is no longer in the tree. wikipedia says
      // "return right(T)" which is technically correct, but this is
      // clearer.
      return NULL;
    } else if (t->left == NULL) {
      string_tree_t* L = string_tree_successor(t);
      // Note: wikipedia or the orginal article may have a bug. Doing
      // the delete and then the key/value assignment leads to a
      // divergence with a reference implementation.
      t->key = L->key;
      t->value = L->value;
      t->right = string_tree_delete(t->right, L->key);
    } else {
      string_tree_t* L = string_tree_predecessor(t);
      // Note: wikipedia or the orginal article may have a bug. Doing
      // the delete and then the key/value assignment leads to a
      // divergence with a reference implementation.
      t->key = L->key;
      t->value = L->value;
      t->left = string_tree_delete(t->left, L->key);
    }
  }

  // Rebalance the tree. Decrease the level of all nodes in this level
  // if necessary, and then skew and split all nodes in the new level.

  t = string_tree_decrease_level(t);
  t = string_tree_skew(t);
  t->right = string_tree_skew(t->right);
  if (t->right != NULL) {
    t->right->right = string_tree_skew(t->right->right);
  }
  t = string_tree_split(t);
  t->right = string_tree_split(t->right);
  return t;
}
