#include <stdio.h>
#include <stdlib.h>

inline static void swap(int *arr, int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

#define partation(a, b) two_pointer_partation(a, b)

static void sort(int *arr, int len);
static int simple_partation(int *arr, int len);
static int two_pointer_partation(int *arr, int len);

static void sort(int *arr, int len) {
  if (len <= 1) {
    return;
  }

  int mid = partation(arr, len);
  sort(arr, mid);
  sort(arr + mid + 1, len - mid - 1);
}

static int simple_partation(int *arr, int len) {
  int i = 1;
  int j = 1;
  int x = arr[0];

  while (j < len) {
    if (arr[j] < x) {
      swap(arr, i, j);
      i++;
    }
    j++;
  }
  swap(arr, 0, i - 1);

  return i - 1;
}

static int two_pointer_partation(int *arr, int len) {
  int p = 0;
  int q = len;
  int x = arr[0];
  while (p < q) {
    while (p < q && arr[--q] >= x)
      ;
    arr[p] = arr[q];
    while (p < q && arr[++p] <= x)
      ;
    arr[q] = arr[p];
  }
  arr[p] = x;
  return p;
}

int main() {
  int arr[] = {324,  24, 435,  34,  4534, 234322, 43534,
               2334, 2,  2134, 423, 4,    32432};
  int arr_len = sizeof(arr) / sizeof(int);
  sort(arr, arr_len);
  for (int i = 0; i < arr_len; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}