#include "sort_and_unify.h"
#include <string.h>

typedef char const *lpstr;

static void swap(lpstr *a, lpstr *b) {
  lpstr t = *a;
  *a = *b;
  *b = t;
}
long partition(lpstr array[], long low, long high) {

  // select the rightmost element as pivot
  lpstr pivot = array[high];

  // pointer for greater element
  long i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (long j = low; j < high; j++) {
    if (strcmp(array[j], pivot) <= 0) {

      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;

      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);

  // return the partition point
  return (i + 1);
}
void quickSort(lpstr array[], long low, long high) {
  if (low < high) {

    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    long pi = partition(array, low, high);

    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);

    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}
size_t sort_and_unify(char const **array, size_t size) {
  quickSort(array, 0, size - 1);
  size_t i = 1;
  for (size_t j = 1; j < size; ++j) {
    if (strcmp(array[i - 1], array[j]) != 0) {
      array[i] = array[j];
      ++i;
    }
  }
  return i;
}
