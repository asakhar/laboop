#ifndef SET_H
#define SET_H

typedef unsigned long size_t;
typedef long ssize_t;

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct Set * Set;

typedef struct iterator {
  struct Node* it;
} iterator;

/**
 * @brief Set constructor function
 * 
 * @return Set 
 */
Set set();
/**
 * @brief Set pseudo-destructor (actually jush a regular function to dispose allocated memory)
 * 
 * @param set 
 */
void set_delete(Set set);
/**
 * @brief Insert `value` to the `set`
 * 
 * @param set 
 * @param value
 * @return true 
 * @return false 
 */
bool set_insert(Set set, char const* value);
/**
 * @brief Remove element from `set` by comparison to `value`
 * 
 * @param set 
 * @param value 
 * @return true 
 * @return false 
 */
bool set_remove(Set set, char const* value);
/**
 * @brief Remove element from `set` by `idx`
 * 
 * @param set 
 * @param idx 
 * @return true 
 * @return false 
 */
bool set_erase(Set set, size_t idx);
/**
 * @brief Clear the `set`
 * 
 * @param set 
 */
void set_clear(Set set);
/**
 * @brief Get element's value by `idx`
 * 
 * @param set 
 * @param idx 
 * @return char const* 
 */
char const* set_get(Set set, size_t idx);
/**
 * @brief Get iterator to walk the `set`
 * 
 * @param set 
 * @return iterator 
 */
iterator set_iter(Set set);
/**
 * @brief Get the value behind the `iter` and move it to the next element
 * 
 * @param iter 
 * @return char const* 
 */
char const* iter_next(iterator* iter);
/**
 * @brief Get the size of `set`
 * 
 * @param set 
 * @return size_t 
 */
size_t set_get_size(Set set);
/**
 * @brief Find an element by `value` and return iterator pointing at it
 * 
 * @param set 
 * @param value 
 * @return iterator 
 */
iterator set_find(Set set, char const* value);
/**
 * @brief Find an element by `value` and return it's index
 * 
 * @param set 
 * @param value 
 * @return ssize_t 
 */
ssize_t set_index_of(Set set, char const* value);
/**
 * @brief Check wheither `set` contains an element with data equal to `value`
 * 
 * @param set 
 * @param value 
 * @return true 
 * @return false 
 */
bool set_contains(Set set, char const* value);
/**
 * @brief Set union operation
 * Takes 2 sets and produces third containing elements that were contained by first, second or both of sets
 * 
 * @param set1 
 * @param set2 
 * @return Set 
 */
Set set_union(Set set1, Set set2);
/**
 * @brief Set inserset operation
 * Takes 2 set and produces third containing elements that were contained both by first and second sets
 *
 * @param set1 
 * @param set2 
 * @return Set 
 */
Set set_intersect(Set set1, Set set2);
/**
 * @brief Set difference operation
 * Takes 2 set and produces third containing elements that were contained by first set but not contained by second
 *
 * @param set1 
 * @param set2 
 * @return Set 
 */
Set set_difference(Set set1, Set set2);
/**
 * @brief Save `set` in binary format to the file by `filename`
 * 
 * @param set 
 * @param filename 
 * @return true 
 * @return false 
 */
bool set_save(Set set, char const *filename);
/**
 * @brief Load `set` from file that was created by call to set_save function
 * 
 * @param filename 
 * @return Set 
 */
Set set_load(char const *filename);

#define foreach(varname, setname)                              \
  iterator __iterator = set_iter(setname);                         \
  for (char const* varname = iter_next(&__iterator); varname != NULL; \
       varname = iter_next(&__iterator))

#endif  // SET_H
