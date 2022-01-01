#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PARENT(i) (((i) + 1) / 2 - 1)
#define LEFT_CHILD(i) ((i)*2 + 1)
#define RIGHT_CHILD(i) ((i)*2 + 2)
#define ROOT (0)
#define NOT_ROOT(i) ((i) != ROOT)

struct heap {
  int *array;
  int array_len;
  int heap_size;
};

static struct heap *empty_heap(int);
static struct heap *make_heap(int *, int);
static void keep_heap_at(struct heap *, int);
static bool check_heap(struct heap *, int);
static int pop(struct heap *);
static void push(struct heap *, int);
static void change(struct heap *, int, int);
static void heap_sort(int *, int);

static struct heap *empty_heap(int len) {
  struct heap *hp = (struct heap *)malloc(sizeof(struct heap));
  hp->array = (int *)malloc(len * sizeof(int));
  hp->array_len = len;
  hp->heap_size = 0;

  return hp;
}

static struct heap *make_heap(int *array, int len) {
  struct heap *hp = (struct heap *)malloc(sizeof(struct heap));
  hp->array = array;
  hp->array_len = hp->heap_size = len;

  for (int i = len - 1; i >= ROOT; i--) {
    keep_heap_at(hp, i);
  }

  assert(check_heap(hp, ROOT));
  return hp;
}

static void keep_heap_at(struct heap *hp, int i) {
  int left = LEFT_CHILD(i);
  int right = RIGHT_CHILD(i);
  int min = i;

  if (left < hp->heap_size && hp->array[left] < hp->array[min]) {
    min = left;
  }

  if (right < hp->heap_size && hp->array[right] < hp->array[min]) {
    min = right;
  }

  if (min != i) {
    int tmp = hp->array[i];
    hp->array[i] = hp->array[min];
    hp->array[min] = tmp;
    keep_heap_at(hp, min);
  }
  assert(check_heap(hp, i));
}

static bool check_heap(struct heap *hp, int i) {
  int left = LEFT_CHILD(i);
  int right = RIGHT_CHILD(i);
  int heap_size = hp->heap_size;
  int val_i = hp->array[i];

  bool ans = (left >= heap_size ||
              (hp->array[left] >= val_i && check_heap(hp, left))) &&
             (right >= heap_size ||
              (hp->array[right] >= val_i && check_heap(hp, right)));

  return ans;
}

static int pop(struct heap *hp) {
  // remove root && keep heap
  int ans = hp->array[ROOT];
  hp->array[ROOT] = hp->array[hp->heap_size - 1];
  keep_heap_at(hp, ROOT);

  assert(check_heap(hp, ROOT));
  return ans;
}

static void push(struct heap *hp, int val) {
  // push and keep heap
  if (hp->array_len == hp->heap_size) {
    return;
  }

  hp->heap_size++;
  hp->array[hp->heap_size - 1] = INT_MAX;
  change(hp, hp->heap_size - 1, val);

  assert(check_heap(hp, ROOT));
}

static void change(struct heap *hp, int i, int val) {
  int old_val = hp->array[i];
  hp->array[i] = val;

  if (old_val < val) {
    // increase, keep heap at i
    keep_heap_at(hp, i);
    return;
  }

  // decrease or same, go to top
  while (NOT_ROOT(i) && hp->array[PARENT(i)] > hp->array[i]) {
    int tmp = hp->array[i];
    hp->array[i] = hp->array[PARENT(i)];
    hp->array[PARENT(i)] = tmp;
    i = PARENT(i);
  }

  assert(check_heap(hp, ROOT));
}

static void heap_sort(int *array, int len) {
  struct heap *hp = make_heap(array, len);
  while (hp->heap_size) {
    int tmp = hp->array[0];
    hp->array[0] = hp->array[hp->heap_size - 1];
    hp->array[hp->heap_size - 1] = tmp;
    hp->heap_size--;

    keep_heap_at(hp, 0);
  }
  free(hp);
}

int main() {
  int arr[] = {7, 2, 4, 5, 3, 1, 0, 9, 6, 8};
  int arr_len = 10;
  struct heap *hp = empty_heap(10);
  for (int i = 0; i < 10; i++) {
    push(hp, arr[i]);
  }
  pop(hp);
  pop(hp);
  change(hp, 5, 100);
  change(hp, 3, -10);
  change(hp, 0, -1232);

  heap_sort(arr, arr_len);
  for (int i = 0; i < arr_len; i++) {
    printf("%d%c", arr[i], i == arr_len - 1 ? '\n' : ' ');
  }

  return 0;
}