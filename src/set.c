#include "set.h"
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char* data;
  struct Node* child[2];
  struct Node* parent;
  size_t child_nodes;
} Node;

typedef struct SetStruct {
  struct Node* root;
}* Set;

typedef Node *lpnode;
typedef char const *lpstr;

static void free_node(lpnode node) {
  if (node == NULL)
    return;
  free(node->data);
  free_node(node->child[0]);
  free_node(node->child[1]);
  free(node);
}

static bool node_insert(lpnode node, lpstr value) {
  if (node->data == NULL) {
    size_t const size = strlen(value);
    node->data = malloc(sizeof(char) * (size + 1));
    strcpy(node->data, value);
    return true;
  }
  int const cmp = strcmp(value, node->data);
  if (cmp == 0) {
    return false;
  }
  bool const idx = cmp > 0;
  if (node->child[idx] == NULL) {
    node->child[idx] = calloc(sizeof(Node), 1);
    node->child[idx]->parent = node;
  }
  bool const res = node_insert(node->child[idx], value);
  node->child_nodes += res;
  return res;
}

static lpnode node_find(lpnode node, lpstr value) {
  if (node == NULL)
    return NULL;
  if (node->data == NULL) {
    return NULL;
  }
  int const cmp = strcmp(value, node->data);
  if (cmp == 0) {
    return node;
  }
  bool const idx = cmp > 0;
  return node_find(node->child[idx], value);
}

static lpnode find_dir_down(lpnode node, bool const dir) {
  if (node == NULL)
    return NULL;
  if (node->child[dir] == NULL)
    return NULL;
  node = node->child[dir];
  while (node->child[!dir] != NULL) {
    node = node->child[!dir];
  }
  return node;
}

static inline lpnode find_next_down(lpnode node) {
  return find_dir_down(node, true);
}

static inline lpnode find_prev_down(lpnode node) {
  return find_dir_down(node, false);
}

static inline bool is_right(lpnode node) {
  if (node->parent->child[1] == node)
    return true;
  return false;
}

static inline void nodes_swap(lpnode n1, lpnode n2) {
  char *tmp_data = n1->data;
  n1->data = n2->data;
  n2->data = tmp_data;
}

static bool node_extract(lpnode node) {
  lpnode prec = find_prev_down(node);
  bool right = false;
  if (prec == NULL) {
    prec = find_next_down(node);
    right = true;
  }
  if (prec != NULL) {
    nodes_swap(node, prec);
    prec->parent->child[is_right(prec)] = prec->child[right];
    if (prec->child[right])
      prec->child[right]->parent = prec->parent;
    prec->child[0] = NULL;
    prec->child[1] = NULL;
    lpnode tmp = prec->parent;
    while (tmp) {
      --tmp->child_nodes;
      tmp = tmp->parent;
    }
    free_node(prec);
    return false;
  }
  lpnode tmp = node->parent;
  while (tmp) {
    --tmp->child_nodes;
    tmp = tmp->parent;
  }
  free_node(node);
  return true;
}

static void node_erase(lpnode *node) {
  if (node_extract(*node))
    *node = NULL;
}

static bool node_remove(lpnode *node, lpstr value) {
  lpnode tmp = *node;
  if (tmp == NULL)
    return false;
  int const cmp = strcmp(value, tmp->data);
  if (cmp == 0) {
    node_erase(node);
    return true;
  }
  return node_remove(&tmp->child[cmp > 0], value);
}

static lpnode node_get(lpnode node, size_t idx) {
  if (node == NULL)
    return NULL;
  if (idx > node->child_nodes) {
    return NULL;
  }
  if (node->child[0] != NULL) {
    if (node->child[0]->child_nodes + 1 == idx)
      return node;
    if (node->child[0]->child_nodes == idx)
      return find_prev_down(node);
    if (node->child[0]->child_nodes > idx)
      return node_get(node->child[0], idx);
    return node_get(node->child[1], idx - node->child[0]->child_nodes - 2);
  } else if (idx == 0)
    return node;
  return node_get(node->child[1], idx - 1);
}

static inline lpstr node_str_get(lpnode node, size_t idx) {
  lpnode res = node_get(node, idx);
  if (res == NULL)
    return NULL;
  return res->data;
}


void node_save(lpnode node, FILE *file) {
  if (node == NULL) {
    return;
  }
  size_t strsize = strlen(node->data);
  char children = 0;
  if (node->child[0])
    children |= 1;
  if (node->child[1])
    children |= 2;
  fwrite(&strsize, sizeof(size_t), 1, file);
  fwrite(&node->child_nodes, sizeof(size_t), 1, file);
  fwrite(node->data, sizeof(char), strsize, file);
  fwrite(&children, sizeof(char), 1, file);
  node_save(node->child[0], file);
  node_save(node->child[1], file);
}

lpnode node_load(lpnode parent, FILE *file) {
  lpnode node = malloc(sizeof(struct Node));
  size_t strsize;
  char chilren = 0;
  fread(&strsize, sizeof(size_t), 1, file);
  fread(&node->child_nodes, sizeof(size_t), 1, file);
  node->data = malloc(strsize + 1);
  fread(node->data, sizeof(char), strsize, file);
  node->data[strsize] = '\0';
  fread(&chilren, sizeof(char), 1, file);
  node->child[0] = chilren & 1 ? node_load(node, file) : NULL;
  node->child[1] = chilren & 2 ? node_load(node, file) : NULL;
  node->parent = parent;
  return node;
}

Set set() { return calloc(sizeof(struct SetStruct), 1); }
void set_delete(Set set) {
  set_clear(set);
  free(set);
}
bool set_insert(Set set, lpstr value) {
  if (value == NULL) {
    return false;
  }
  if (set->root == NULL) {
    set->root = calloc(sizeof(Node), 1);
  }
  return node_insert(set->root, value);
}
bool set_remove(Set set, lpstr value) {
  if (set->root == NULL) {
    return false;
  }
  return node_remove(&set->root, value);
}
bool set_erase(Set set, size_t idx) {
  lpnode res = node_get(set->root, idx);
  if (res == NULL)
    return false;
  if (res->parent) {
    node_erase(&res->parent->child[is_right(res)]);
    return true;
  }
  free_node(set->root);
  set->root = NULL;
  return true;
}
void set_clear(Set set) {
  free_node(set->root);
  set->root = NULL;
}
lpstr set_get(Set set, size_t idx) {
  assert(set != NULL);
  return node_str_get(set->root, idx);
}
iterator set_iter(Set set) {
  lpnode node = set->root;
  while (node != NULL && node->child[0] != NULL)
    node = node->child[0];
  return (struct iterator){node};
}
char const *iter_next(iterator *iter) {
  lpnode node = iter->it;
  if (node == NULL)
    return NULL;
  lpstr res = node->data;
  if (node->child[1] != NULL) {
    iter->it = find_next_down(node);
    return res;
  }
  while (node != NULL && node->parent != NULL && is_right(node))
    node = node->parent;
  if (node == NULL) {
    iter->it = NULL;
    return res;
  }
  iter->it = node->parent;
  return res;
}

size_t set_get_size(Set set) {
  if (set->root == NULL)
    return 0;
  return set->root->child_nodes + 1;
}

iterator set_find(Set set, lpstr value) {
  return (struct iterator){node_find(set->root, value)};
}

ssize_t set_index_of(Set set, lpstr value) {
  iterator res = set_find(set, value);
  if (res.it == NULL) {
    return -1;
  }
  if (res.it->child[0] == NULL)
    return 0;
  return res.it->child[0]->child_nodes + 1;
}

Set set_union(Set set1, Set set2) {
  Set newset = set();
  {
    ssize_t size = set_get_size(set1);
    for (ssize_t i = 0; i < size; ++i) {
      size_t idx = (((~i & 1) << 1) - 1) * (i + 1) / 2 + size / 2;
      set_insert(newset, set_get(set1, idx));
    }
  }
  ssize_t size = set_get_size(set2);
  for (ssize_t i = 0; i < size; ++i) {
    size_t idx = (((~i & 1) << 1) - 1) * (i + 1) / 2 + size / 2;
    char const* item = set_get(set2, idx);
    if (!set_contains(newset, item)) {
      set_insert(newset, item);
    }
  }
  return newset;
}
bool set_contains(Set set, char const *value) {
  return set_find(set, value).it != NULL;
}

Set set_intersect(Set set1, Set set2) {
  Set newset = set();
  Set sets[2] = {set1, set2};
  bool second = set_get_size(sets[0]) > set_get_size(sets[1]);
  ssize_t size = set_get_size(sets[second]);
  for (ssize_t i = 0; i < size; ++i) {
    size_t idx = (((~i & 1) << 1) - 1) * (i + 1) / 2 + size / 2;
    char const* item = set_get(sets[second], idx);
    if (set_contains(sets[!second], item))
      set_insert(newset, item);
  }
  return newset;
}
Set set_difference(Set set1, Set set2) {
  Set newset = set();
  ssize_t size = set_get_size(set1);
  for (ssize_t i = 0; i < size; ++i) {
    size_t idx = (((~i & 1) << 1) - 1) * (i + 1) / 2 + size / 2;
    char const* item = set_get(set1, idx);
    if (!set_contains(set2, item)) {
      set_insert(newset, item);
    }
  }
  return newset;
}

bool set_save(Set set, char const *filename) {
  FILE* file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Set not saved");
    return false;
  }
  node_save(set->root, file);
  fclose(file);
  return true;
}
Set set_load(char const *filename) {
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Set not loaded");
    return NULL;
  }
  Set newset = set();
  newset->root = node_load(NULL, file);
  fclose(file);
  return newset;
}
