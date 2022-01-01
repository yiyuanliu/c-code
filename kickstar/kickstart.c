#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS_COUNT(value) __builtin_popcount(value)
#define TO_ONE(value) (BITS_COUNT(value) % 2)

int array[100000 + 1];

void sol(int i) {
  int n, q;
  scanf("%d %d", &n, &q);
  for (int i = 0; i < n; i++) {
    int tmp;
    scanf("%d", &tmp);
    array[i] = TO_ONE(tmp);
  }

  int total = 0;
  int first, last;
  int one_count = 0;
  first = last = -1;
  for (int i = 0; i < n; i++) {
    total = total ^ array[i];
    if (array[i]) {
      first = first == -1 ? i : first;
      last = i;
      one_count++;
    }
  }

  printf("Case #%d:", i);

  for (int i = 0; i < q; i++) {
    int p, v;
    scanf("%d %d", &p, &v);
    v = TO_ONE(v);
    if (v != array[p]) {
      total = total ^ 1;
      array[p] = v;
      if (v == 0) {
        one_count--;
        if (one_count == 0)
          first = last = -1;
        // new is zero, if p was first or last, need rescan;
        if (p == first) {
          // find new first
          first = -1;
          for (int i = 0; i <= last; i++) {
            if (array[i]) {
              first = i;
              break;
            }
          }
        }
        if (p == last) {
          // find new last
          last = -1;
          if (first != -1) {
            for (int i = p; i >= first; i--) {
              if (array[i]) {
                last = i;
                break;
              }
            }
          }
        }
      } else {
        one_count++;
        // new is one
        if (first == -1 || first > p)
          first = p;
        if (last == -1 || last < p)
          last = p;
      }
    }

    int ans = 0;
    if (total == 0) {
      ans = n;
    } else {
      if (first != -1) {
        // remove first 1
        ans = n - first - 1;
      }
      if (last != -1) {
        // remove last 1
        ans = last > ans ? last : ans;
      }
    }
    printf(" %d", ans);
  }
  printf("\n");
}

int main() {
  int t = 0;
  scanf("%d", &t);
  for (int i = 0; i < t; i++) {
    sol(i + 1);
  }
}