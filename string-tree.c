#line 2 "string-tree.c"

/**
 * This is a balanced binary tree to associated a string and a value.
 *
 * Currently we are using "AA" trees (see
 * https://en.wikipedia.org/wiki/AA_tree) since it has simpler code
 * than many other balanced trees like RB trees.
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

typdef struct string_tree_S string_tree_t;

extern value_result_t find(string_tree_t* t, char* key);
extern string_tree_t* insert(string_tree_t* t, char* key, value_t value);
extern string_tree_t* delete(string_tree_t* t, char* key);

// TODO(jawilson): in order traversal, aka string_tree_foreach

#endf /* _STRING_TREE_H_ */

value_result_t find(string_tree_t* t, char* key) {
  if (t == NULL) {
    return (value_result_t) {.found = 0};
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    return find(t->left, key);
  } else if (cmp_result > 0) {
    return find(t->right, key);
  } else {
    return (value_result_t) {.val = t->value, .found = true };
  }
}

string_tree_t* skew(string_tree_t* t) {
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

string_tree_t* split(string_tree_t* t) {
  if (t == NULL) {
    return NULL;
  }
  if (t->right == NULL || t->right->right == NULL) {
    return t;
  } 
  if (t->level == t->right->right->level) {
    // We have two horizontal right links.  Take the middle node,
    // elevate it, and return it.
    string_tree_t R = t->right;
    t->right = R->left;
    R->left := t;
    R->level = R->level + 1;
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

string_tree_t* insert(string_tree_t* t, char* key, value_t value) {
  if (t == NULL) {
    // Create a new leaf node
    return make_string_tree_leaf(key, value);
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    t->left = insert(X, left(T));
  } else if (cmp_result > 0) {
    right(T) = insert(X, right(T));
  } else {
    // might need to free either key or t->key...
    t->value = value;
    return t;
  }
  t = skew(t);
  t = split(t);
  return t;
}

string_tree_t* delete(string_tree_t* t, char* key) {

  if (t == NULL) {
    return t;
  }

  int cmp_result = strcmp(key, t->key);
  if (cmp_result  > t->key) {
    t->right = delete(t, t->right);
  } else if (X < value(T)) {
    left(T) = delete(X, left(T));
  } else {
    // If we're a leaf, easy, otherwise reduce to leaf case.
    if (is_leaf(t)) {
      return t->right;
    } else if (t->left == NULL) {
      string_tree_t* L = successor(t);
      t->right = delete(t->right, L->key);
      t->key = left->key;
      t->value = left->value;
    } else {
      string_tree_t* L = predecessor(t);
      left(T) = delete(value(L), left(T));
      value(T) = value(L);
    }
  }

  // Rebalance the tree. Decrease the level of all nodes in this level
  // if necessary, and then skew and split all nodes in the new level.

  t = decrease_level(t);
  t = skew(t);
  t->right = skew(t->right);
  if (t->right != NULL) {
    right(right(T)) = skew(right(right(T)));
  }
  t = split(t);
  t->right = split(t->right);
  return t;
}

string_tree_t* decrease_level(string_tree_t* t) {
  uint32_t should_be = min(t->left->level, t->right->left) + 1;
  if (should_be < t->level) {
    t->level = should_be;
    if (should_be < t->right->level) {
      t->right->level = should_be;
    }
  }
  return t;
}

string_tree_t* predecessor(string_tree_t* t) {
  t = t->left;
  while (t->right != NULL) {
    t = t->right;
  }
  return t;
}

string_tree_t* successor(string_tree_t* t) {
  t = t->right;
  while (t->left != NULL) {
    t = t->left;
  }
  return t;
}

