#ifndef SET_H
#define SET_H

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

struct Node {
  char* data;
  struct Node* child[2];
  struct Node* parent;
  size_t child_nodes;
};

typedef struct Set {
  struct Node* root;
} * Set;

typedef struct iterator {
  struct Node* it;
} iterator;

Set set();
void set_delete(Set set);
bool set_insert(Set set, char const* value);
bool set_remove(Set set, char const* value);
bool set_erase(Set set, size_t idx);
void set_clear(Set set);
char const* set_get(Set set, size_t idx);
iterator set_iter(Set set);
char const* iter_next(iterator* iter);
size_t set_get_size(Set set);
iterator set_find(Set set, char const* value);
ssize_t set_index_of(Set set, char const* value);
bool set_contains(Set set, char const* value);
Set set_union(Set set1, Set set2);
Set set_intersect(Set set1, Set set2);
Set set_difference(Set set1, Set set2);
bool set_save(Set set, char const *filename);
Set set_load(char const *filename);

#define auto __auto_type

#define foreach(varname, setname)                              \
  auto __iterator = set_iter(setname);                         \
  for (auto varname = iter_next(&__iterator); varname != NULL; \
       varname = iter_next(&__iterator))

#endif  // SET_H
