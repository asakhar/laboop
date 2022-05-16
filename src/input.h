#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef unsigned long size_t;
typedef long ssize_t;

#define green "\033[0;32m"
#define red "\033[0;31m"
#define reset "\033[0m"
#define yellow "\033[0;33m"

size_t read_unumber(size_t from, size_t to, bool *exited);

ssize_t read_inumber(ssize_t from, ssize_t to, bool *exited);

char *read_line();

#endif  // INPUT_H
