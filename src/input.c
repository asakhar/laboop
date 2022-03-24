#include "input.h"

#include <stdio.h>
#define auto __auto_type

size_t read_unumber(size_t from, size_t to, bool *exited) {
  while (1) {
    size_t choice;
    auto res = scanf("%zu", &choice);
    size_t missed = 0;
    int c = 0;
    while ((c = getc(stdin)) != '\n') {
      if (++missed == 1 && exited && (c == 'e' || c == 'E')) {
        if (getc(stdin) == '\n') {
          *exited = true;
          return 0;
        }
        ++missed;
      }
    }
    if (res != 0 && missed == 0 && choice >= from && choice <= to) {
      return choice;
    }
    fprintf(stderr, "\n"red"Invalid option choosen. Try again!"reset"\n> ");
  }
}
ssize_t read_inumber(ssize_t from, ssize_t to, bool *exited) {
  while (1) {
    ssize_t choice;
    auto res = scanf("%ld", &choice);
    size_t missed = 0;
    int c = 0;
    while ((c = getc(stdin)) != '\n') {
      if (++missed == 1 && exited && (c == 'e' || c == 'E')) {
        if (getc(stdin) == '\n') {
          *exited = true;
          return 0;
        }
        ++missed;
      }
    }
    if (res != 0 && missed == 0 && choice >= from && choice <= to) {
      return choice;
    }
    fprintf(stderr, "\n"red"Invalid option choosen. Try again!"reset"\n> ");
  }
}
char *read_line() {
  size_t size = 0;
  char *buffer = NULL;
  size = getline(&buffer, &size, stdin);
  if(buffer[size-1] == '\n')
    buffer[size-1] = '\0';
  return buffer;
}
