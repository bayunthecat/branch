#include "arrays.h"
#include <stdlib.h>

int **NewTwoDimArray(int rows, int cols, int init) {
  int **arr = malloc(rows * sizeof(int *));
  for (int i = 0; i < rows; i++) {
    arr[i] = malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++) {
      arr[i][j] = init;
    }
  }
  return arr;
}

void FreeTwoDimArray(int **arr, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    free(arr[i]);
  }
  free(arr);
}
