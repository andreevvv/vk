#include <limits.h>
#include <stdio.h>

/* struct for binary search */
struct vector_int {
  int    *base;
  size_t nitems;
};
typedef struct vector_int vector_int_t;

#define INVALID_INDEX (-1)

int bsearch_1st_big_value (vector_int_t *value, size_t left, size_t right, int x) {
  while (left != right) {
    size_t middle = left + (right - left) / 2;
    if (value->base[middle] <= x) {
      left = middle + 1;
    } else {
      right = middle;
    }
  }
  if (value->base[right] > x) {
    return right;
  }
  return INVALID_INDEX;
}

/* binary search implementation */
int bsearch (vector_int_t *value, int x, ...) {
  if (!value) {
    return INVALID_INDEX;
  }

  size_t left = 0;
  size_t middle = 0;
  size_t right = value->nitems;
  if (0 >= value->nitems) {
    return INVALID_INDEX;
  }

  while (left < right) {
    middle = left + (right - left) / 2;
    if (value->base[middle] < x) {
      left = middle + 1;
    } else if (value->base[middle] > x) {
      right = middle;
    } else {
      return bsearch_1st_big_value( value, middle, right, x);
    }
  }
  return INVALID_INDEX;
}

/* wrap for unit tasting */
int unittest (int *values, int nitems, int x) {
    vector_int_t v;
    v.base = values;
    v.nitems = nitems;
    return bsearch( &v, x);
}

/* main function impemenation */
int main (int argc, char **argv) {
  int values[] = { INT_MIN, -1000, -100, -1, -1, 0, 1, 2, 2, 10, 100, 100, 1000, INT_MAX };
  size_t ntitems = sizeof(values) / sizeof(values[0]);
  printf("result: %d\n", unittest( &values[0], ntitems, INT_MIN));
  printf("result: %d\n", unittest( &values[0], ntitems, -1000));
  printf("result: %d\n", unittest( &values[0], ntitems, -2));
  printf("result: %d\n", unittest( &values[0], ntitems, 0));
  printf("result: %d\n", unittest( &values[0], ntitems, 5));
  printf("result: %d\n", unittest( &values[0], ntitems, 100));
  printf("result: %d\n", unittest( &values[0], ntitems, 1000));
  printf("result: %d\n", unittest( &values[0], ntitems, INT_MAX));
  return 0;
}
