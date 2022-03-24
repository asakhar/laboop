#include "set.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t const NSETS = 10;

void add_element(Set sets[], size_t *current);
void remove_element(Set sets[], size_t *current);
void erase_element(Set sets[], size_t *current);
void clear_set(Set sets[], size_t *current);
void get_element(Set sets[], size_t *current);
void index_of_element(Set sets[], size_t *current);
void print_set_size(Set sets[], size_t *current);
void print_set(Set sets[], size_t *current);
void union_sets(Set sets[], size_t *current);
void intersect_sets(Set sets[], size_t *current);
void subtract_set(Set sets[], size_t *current);
void change_current(Set sets[], size_t *current);
void exit_app(Set sets[], size_t *current);

int main(int argc, char const *argv[]) {
  Set sets[NSETS];
  size_t current = 0;
  for (size_t i = 0; i < NSETS; ++i) {
    sets[i] = set();
  }
  void (*const actions[13])(Set[], size_t *) = {
      add_element, remove_element,   erase_element,  clear_set,
      get_element, index_of_element, print_set_size, print_set,
      union_sets,  intersect_sets,   subtract_set,   change_current,
      exit_app};
  while (1) {
    printf("Options:\n"
           "[ 1] add_element\n"
           "[ 2] remove_element\n"
           "[ 3] erase_element\n"
           "[ 4] clear_set\n"
           "[ 5] get_element\n"
           "[ 6] index_of_element\n"
           "[ 7] print_set_size\n"
           "[ 8] print_set\n"
           "[ 9] union_sets\n"
           "[10] intersect_sets\n"
           "[11] subtract_set\n"
           "[12] change_current\n"
           "[13] exit_app\n"
           "> ");
    size_t choice = 0;
    auto res = scanf("%zu", &choice);
    size_t missed = 0;
    while (getc(stdin) != '\n')
      ++missed;
    if (res == 0 || missed != 0 || choice < 1 || choice > 13) {
      fprintf(stderr, "\nInvalid action choosen. Try again!\n");
      continue;
    }
    actions[choice - 1](sets, &current);
  }

  return 0;
}

#include <assert.h>
#define unimplemented() assert(false && "Unimplemented!");

void add_element(Set sets[], size_t *current) { unimplemented(); }
void remove_element(Set sets[], size_t *current) { unimplemented(); }
void erase_element(Set sets[], size_t *current) { unimplemented(); }
void clear_set(Set sets[], size_t *current) { unimplemented(); }
void get_element(Set sets[], size_t *current) { unimplemented(); }
void index_of_element(Set sets[], size_t *current) { unimplemented(); }
void print_set_size(Set sets[], size_t *current) { unimplemented(); }
void print_set(Set sets[], size_t *current) { unimplemented(); }
void union_sets(Set sets[], size_t *current) { unimplemented(); }
void intersect_sets(Set sets[], size_t *current) { unimplemented(); }
void subtract_set(Set sets[], size_t *current) { unimplemented(); }
void change_current(Set sets[], size_t *current) { unimplemented(); }
void exit_app(Set sets[], size_t *current) { exit(0); }