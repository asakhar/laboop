#include "set.h"
#include "sort_and_unify.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();

int main(int argc, char const *argv[]) {
  FILE *file = fopen("/dev/random", "r");
  int seed;
  fread(&seed, sizeof(int), 1, file);
  fclose(file);
  srand(seed);

  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  return 0;
}

bool checkset(Set set, size_t setsize) {
  size_t real_set_size = 0;
  char const *prev = "";
  foreach (val, set) {
    assert(strcmp(prev, val) < 0);
    prev = val;
    ++real_set_size;
  };
  size_t recv_size = set_get_size(set);
  assert(real_set_size == setsize);
  assert(recv_size == setsize);
  return true;
}

#define assert_str_eq(str1, str2)                                              \
  {                                                                            \
    char const *__res1 = str1;                                                 \
    char const *__res2 = str2;                                                 \
    assert((__res1 == NULL) == (__res2 == NULL));                              \
    if (__res1 != NULL)                                                        \
      assert(strcmp(__res1, __res2) == 0);                                     \
  }

char *gen_rand_str() {
  size_t size = rand() % 5 + 1;
  char *string = malloc(size + 1);
  for (size_t i = 0; i < size; ++i) {
    string[i] = rand() % 10 + 65;
  }
  string[size] = '\0';
  return string;
}

bool array_contains(char const *needle, char const **haystack, size_t size) {
  for (size_t i = 0; i < size; ++i)
    if (strcmp(needle, haystack[i]) == 0)
      return true;
  return false;
}

void array_remove(char const *needle, char const **haystack, size_t size) {
  size_t idx = 0;
  for (; idx < size; ++idx) {
    if (strcmp(needle, haystack[idx]) == 0)
      break;
  }
  if (idx >= size - 1)
    return;
  memcpy(haystack + idx, haystack + idx + 1, (size - idx - 1) * sizeof(void *));
}

struct Node {
  char *data;
  struct Node *child[2];
  struct Node *parent;
  size_t child_nodes;
};

struct Set {
  struct Node *root;
};

size_t get_sub_tree_size(struct Node *node) {
  if (node == NULL)
    return 0;
  if (!(node->child[0] || node->child[1]))
    return 1;
  assert(get_sub_tree_size(node->child[0]) +
             get_sub_tree_size(node->child[1]) ==
         node->child_nodes);
  return node->child_nodes + 1;
}

void test1() {
  Set myset = set();

  assert(set_insert(myset, "abc") == true);
  checkset(myset, 1);
  assert(set_insert(myset, "aaa") == true);
  checkset(myset, 2);
  assert(set_insert(myset, "ab") == true);

  checkset(myset, 3);

  assert(set_insert(myset, "abc") == false);
  checkset(myset, 3);
  assert(set_insert(myset, "aaa") == false);
  checkset(myset, 3);
  assert(set_insert(myset, "ab") == false);

  checkset(myset, 3);

  assert(set_remove(myset, "aaa") == true);
  checkset(myset, 2);
  assert(set_remove(myset, "abc") == true);
  checkset(myset, 1);
  assert(set_remove(myset, "ab") == true);

  checkset(myset, 0);

  assert(set_insert(myset, "aaa") == true);
  checkset(myset, 1);
  assert(set_insert(myset, "abc") == true);
  checkset(myset, 2);
  assert(set_insert(myset, "ab") == true);
  checkset(myset, 3);

  set_delete(myset);
}

void test2() {
  Set myset = set();

  char const *string[] = {"b", "a", "c", "F", "D", "E"};

  assert(set_insert(myset, string[1]) == true);
  checkset(myset, 1);
  assert(set_insert(myset, string[0]) == true);
  checkset(myset, 2);
  assert(set_insert(myset, string[2]) == true);
  checkset(myset, 3);
  assert(set_insert(myset, string[3]) == true);
  checkset(myset, 4);
  assert(set_insert(myset, string[5]) == true);
  checkset(myset, 5);
  assert(set_insert(myset, string[4]) == true);
  checkset(myset, 6);

  assert(set_remove(myset, string[0]) == true);
  checkset(myset, 5);
  assert(set_remove(myset, string[1]) == true);
  checkset(myset, 4);
  assert(set_remove(myset, string[2]) == true);
  checkset(myset, 3);
  assert(set_remove(myset, string[3]) == true);
  checkset(myset, 2);
  assert(set_remove(myset, string[4]) == true);
  checkset(myset, 1);
  assert(set_remove(myset, string[5]) == true);
  checkset(myset, 0);

  set_delete(myset);
}

void test3() {
  Set myset = set();

  char const *string[] = {"b", "a", "c", "F", "D", "E"};

  assert(set_insert(myset, string[1]) == true);
  checkset(myset, 1);
  assert(set_insert(myset, string[0]) == true);
  checkset(myset, 2);
  assert(set_insert(myset, string[2]) == true);
  checkset(myset, 3);
  assert(set_insert(myset, string[3]) == true);
  checkset(myset, 4);
  assert(set_insert(myset, string[5]) == true);
  checkset(myset, 5);
  assert(set_insert(myset, string[4]) == true);
  checkset(myset, 6);

  assert_str_eq(set_get(myset, 0), string[4]);
  assert_str_eq(set_get(myset, 1), string[5]);
  assert_str_eq(set_get(myset, 2), string[3]);
  assert_str_eq(set_get(myset, 3), string[1]);
  assert_str_eq(set_get(myset, 4), string[0]);
  assert_str_eq(set_get(myset, 5), string[2]);

  assert(set_remove(myset, string[0]) == true);
  assert_str_eq(set_get(myset, 0), string[4]);
  assert_str_eq(set_get(myset, 1), string[5]);
  assert_str_eq(set_get(myset, 2), string[3]);
  assert_str_eq(set_get(myset, 3), string[1]);
  assert_str_eq(set_get(myset, 4), string[2]);
  assert_str_eq(set_get(myset, 5), NULL);
  assert(set_remove(myset, string[1]) == true);
  assert_str_eq(set_get(myset, 0), string[4]);
  assert_str_eq(set_get(myset, 1), string[5]);
  assert_str_eq(set_get(myset, 2), string[3]);
  assert_str_eq(set_get(myset, 3), string[2]);
  assert_str_eq(set_get(myset, 4), NULL);
  assert(set_remove(myset, string[2]) == true);
  assert_str_eq(set_get(myset, 0), string[4]);
  assert_str_eq(set_get(myset, 1), string[5]);
  assert_str_eq(set_get(myset, 2), string[3]);
  assert_str_eq(set_get(myset, 3), NULL);
  assert(set_remove(myset, string[3]) == true);
  assert_str_eq(set_get(myset, 0), string[4]);
  assert_str_eq(set_get(myset, 1), string[5]);
  assert_str_eq(set_get(myset, 2), NULL);
  assert(set_remove(myset, string[4]) == true);
  assert_str_eq(set_get(myset, 0), string[5]);
  assert_str_eq(set_get(myset, 1), NULL);
  assert(set_remove(myset, string[5]) == true);
  assert_str_eq(set_get(myset, 0), NULL);

  set_delete(myset);
}

void test4() {
  Set myset = set();
  // size_t const SIZE = 300; // for some reason do not work in msvc
#define SIZE 300
  char *strings[SIZE];
  bool already[SIZE];
  bool already_rem[SIZE];
  size_t already_amount = 0;
  for (size_t i = 0; i < SIZE; ++i) {
    strings[i] = gen_rand_str();
    already_amount +=
        (already[i] = array_contains(strings[i], (char const **)strings, i));
    already_rem[i] = false;
    assert(set_insert(myset, strings[i]) == !already[i]);
    checkset(myset, i + 1 - already_amount);
  }
  assert(get_sub_tree_size(myset->root) == SIZE - already_amount);
  char const *sorted[SIZE];
  memcpy(sorted, strings, SIZE * sizeof(char const *));
  size_t const sorted_size = sort_and_unify(sorted, SIZE);
  {
    iterator iter = set_iter(myset);
    for (size_t i = 0; i < SIZE - already_amount; ++i) {
      char const *res = set_get(myset, i);
      char const *item = iter_next(&iter);
      assert_str_eq(sorted[i], item);
      assert_str_eq(sorted[i], res);
    }
  }

  assert(set_save(myset, "test4.bin"));

  Set myset2 = set_load("test4.bin");
  assert(myset2 != NULL);

  size_t origsize = set_get_size(myset);

  for (size_t i = 0; i < origsize; ++i) {
    assert_str_eq(set_get(myset, i), set_get(myset2, i));
  }
  assert(set_get_size(myset2) == origsize);
  set_delete(myset2);

  for (size_t i = 0; i < SIZE + 100; ++i) {
    size_t idx = rand() % SIZE;
    if (already[idx])
      continue;
    assert(set_remove(myset, strings[idx]) == !already_rem[idx]);
    array_remove(strings[idx], sorted, SIZE - already_amount);
    already_amount += !already_rem[idx];
    checkset(myset, SIZE - already_amount);
    {
      iterator iter = set_iter(myset);
      for (size_t j = 0; j < SIZE - already_amount; ++j) {
        char const *res = set_get(myset, j);
        char const *item = iter_next(&iter);
        assert_str_eq(sorted[j], item);
        assert_str_eq(sorted[j], res);
      }
    }
    already_rem[idx] = true;
  }

  for (size_t i = 0; i < SIZE; ++i)
    free(strings[i]);
  set_delete(myset);
#undef SIZE
}

void test5() {
  Set myset1 = set();
  Set myset2 = set();

  char const *string[] = {"b", "a", "c", "F", "D", "E", "G"};

  set_insert(myset1, string[1]);
  set_insert(myset1, string[0]);
  set_insert(myset1, string[2]);
  set_insert(myset2, string[3]);
  set_insert(myset2, string[5]);
  set_insert(myset2, string[4]);
  set_insert(myset2, string[6]);

  Set unionres = set_union(myset1, myset2);

  assert_str_eq(set_get(unionres, 0), string[4]);
  assert_str_eq(set_get(unionres, 1), string[5]);
  assert_str_eq(set_get(unionres, 2), string[3]);
  assert_str_eq(set_get(unionres, 3), string[6]);
  assert_str_eq(set_get(unionres, 4), string[1]);
  assert_str_eq(set_get(unionres, 5), string[0]);
  assert_str_eq(set_get(unionres, 6), string[2]);
  assert_str_eq(set_get(unionres, 7), NULL);

  set_delete(unionres);
  set_delete(myset1);
  set_delete(myset2);
}

void test6() {
  Set myset1 = set();
  Set myset2 = set();

  char const *string[] = {"b", "a", "c", "F", "D", "a", "G"};

  set_insert(myset1, string[1]);
  set_insert(myset1, string[0]);
  set_insert(myset1, string[2]);
  set_insert(myset2, string[3]);
  set_insert(myset2, string[5]);
  set_insert(myset2, string[4]);
  set_insert(myset2, string[6]);

  Set unionres = set_union(myset1, myset2);

  assert_str_eq(set_get(unionres, 0), string[4]);
  assert_str_eq(set_get(unionres, 1), string[3]);
  assert_str_eq(set_get(unionres, 2), string[6]);
  assert_str_eq(set_get(unionres, 3), string[1]);
  assert_str_eq(set_get(unionres, 4), string[0]);
  assert_str_eq(set_get(unionres, 5), string[2]);
  assert_str_eq(set_get(unionres, 6), NULL);

  set_delete(unionres);
  set_delete(myset1);
  set_delete(myset2);
}

void test7() {
  Set myset1 = set();
  Set myset2;

  char const *string[] = {"b", "a", "c", "F", "D", "a",
                          "G", "#", "f", "n", "b", "A"};
  // size_t const SIZE = sizeof(string) / sizeof(string[0]); // msvc "const is
  // // not actually a constant" stuff
#define SIZE sizeof(string) / sizeof(string[0])

  for (size_t i = 0; i < SIZE; ++i) {
    set_insert(myset1, string[i]);
  }

  assert(set_save(myset1, "test7.bin"));

  myset2 = set_load("test7.bin");
  assert(myset2 != NULL);

  size_t origsize = set_get_size(myset1);

  for (size_t i = 0; i < origsize; ++i) {
    assert_str_eq(set_get(myset1, i), set_get(myset2, i));
  }
  assert(set_get_size(myset2) == origsize);

  set_delete(myset1);
  set_delete(myset2);
#undef SIZE
}