#include "input.h"
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
void save_set(Set sets[], size_t *current);
void load_set(Set sets[], size_t *current);

int main(int argc, char const *argv[]) {
  Set sets[NSETS];
  size_t current = 0;
  for (size_t i = 0; i < NSETS; ++i) {
    sets[i] = set();
  }
  void (*const actions[15])(Set[], size_t *) = {
      add_element, remove_element,   erase_element,  clear_set,
      get_element, index_of_element, print_set_size, print_set,
      union_sets,  intersect_sets,   subtract_set,   change_current,
      save_set,    load_set,         exit_app};
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
           "[13] save_set\n"
           "[14] load_set\n"
           "[15] exit_app\n"
           "> ");
    size_t choice = read_unumber(1, sizeof(actions) / sizeof(actions[0]), NULL);

    actions[choice - 1](sets, &current);
  }

  return 0;
}

#include <assert.h>
#define unimplemented() assert(false && "Unimplemented!");

void add_element(Set sets[], size_t *current) {
  printf("Input element to insert:\n> ");
  auto line = read_line();
  puts(set_insert(sets[*current], line) ? green "Successfully inserted!\n" reset
                                        : red "Failed to insert!\n" reset);
  free(line);
}
void remove_element(Set sets[], size_t *current) {
  printf("Input element to remove:\n> ");
  auto line = read_line();
  puts(set_remove(sets[*current], line) ? green "Successfully removed!\n" reset
                                        : red "Failed to remove!\n" reset);
  free(line);
}
void erase_element(Set sets[], size_t *current) {
  auto set_size = set_get_size(sets[*current]);
  printf("Input element index to remove [0; %zu):\n> ", set_size);
  bool exited = false;
  auto idx = read_unumber(0, set_size - 1, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  puts(set_erase(sets[*current], idx) ? green "Successfully removed!\n" reset
                                      : red "Failed to remove!\n" reset);
}
void clear_set(Set sets[], size_t *current) {
  set_clear(sets[*current]);
  puts(green "Successfully cleared set!\n" reset);
}
void get_element(Set sets[], size_t *current) {
  auto set_size = set_get_size(sets[*current]);
  printf("Input element index to get it's value [0; %zu):\n> ", set_size);
  bool exited = false;
  auto idx = read_unumber(0, set_size - 1, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  auto res = set_get(sets[*current], idx);
  if (res == NULL) {
    puts(red "Failed to get element!\n" reset);
    return;
  }
  printf(green "Requested element: < " yellow "%s" green " >\n\n" reset, res);
}
void index_of_element(Set sets[], size_t *current) {
  printf("Input element to get it's index:\n> ");
  auto line = read_line();
  auto idx = set_index_of(sets[*current], line);
  if (idx == -1) {
    puts(red "Failed to get elements index!\n" reset);
    free(line);
    return;
  }
  printf(green "Index of requested element is: %ld\n\n" reset, idx);
  free(line);
}
void print_set_size(Set sets[], size_t *current) {
  printf(green "Current set size: %zu\n\n" reset, set_get_size(sets[*current]));
}
void print_set(Set sets[], size_t *current) {
  foreach (item, sets[*current]) {
    printf("< " yellow "%s" reset " >\n", item);
  }
  puts("\n--------------");
}
void union_sets(Set sets[], size_t *current) {
  printf("Input set's index to union with [0; %zu):\n> ", NSETS);
  bool exited = false;
  auto idx2 = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  printf("Input set's index to put result in (previously contained values will "
         "be lost) [0; %zu):\n> ",
         NSETS);
  exited = false;
  auto idxres = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  auto res = set_union(sets[*current], sets[idx2]);
  set_delete(sets[idxres]);
  sets[idxres] = res;
  puts(green "Unioned successfully!\n" reset);
}
void intersect_sets(Set sets[], size_t *current) {
  printf("Input set's index to intersect with [0; %zu):\n> ", NSETS);
  bool exited = false;
  auto idx2 = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  printf("Input set's index to put result in (previously contained values will "
         "be lost) [0; %zu):\n> ",
         NSETS);
  exited = false;
  auto idxres = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  auto res = set_intersect(sets[*current], sets[idx2]);
  set_delete(sets[idxres]);
  sets[idxres] = res;
  puts(green "Intersected successfully!\n" reset);
}
void subtract_set(Set sets[], size_t *current) {
  printf("Input set's index to subtract with [0; %zu):\n> ", NSETS);
  bool exited = false;
  auto idx2 = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  printf("Input set's index to put result in (previously contained values will "
         "be lost) [0; %zu):\n> ",
         NSETS);
  exited = false;
  auto idxres = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  auto res = set_difference(sets[*current], sets[idx2]);
  set_delete(sets[idxres]);
  sets[idxres] = res;
  puts(green "Subtracted successfully!\n" reset);
}
void change_current(Set sets[], size_t *current) {
  printf("Input set's index to select [0; %zu):\n> ", NSETS);
  bool exited = false;
  auto idx2 = read_unumber(0, NSETS, &exited);
  if (exited) {
    puts(yellow "Operation cancelled!\n" reset);
    return;
  }
  if (idx2 == *current) {
    puts(red "Set already selected!\n" reset);
    return;
  }
  *current = idx2;
  puts(green "Current set successfully changed!\n" reset);
}

void save_set(Set sets[], size_t *current) {
  printf("Input file name to save in:\n> ");
  auto line = read_line();
  auto res = set_save(sets[*current], line);
  if (!res) {
    puts(red "Failed to save set!\n" reset);
    free(line);
    return;
  }
  printf(green "Set successfully saved in: \"" yellow "%s" green "\"\n\n" reset,
         line);
  free(line);
}
void load_set(Set sets[], size_t *current) { 
  printf("Input file name to load from:\n> ");
  auto line = read_line();
  auto res = set_load(line);
  if (!res) {
    puts(red "Failed to load set!\n" reset);
    free(line);
    return;
  }
  set_delete(sets[*current]);
  sets[*current] = res;
  printf(green "Set successfully loaded from: \"" yellow "%s" green "\"\n\n" reset,
         line);
  free(line);
 }
void exit_app(Set sets[], size_t *current) {
  puts(yellow "Bye!");
  exit(0);
}