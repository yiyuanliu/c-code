#include <stdio.h>
#include <stdlib.h>

int cmp(const void *v1, const void *v2) {
  const int *a = (const int *)v1;
  const int *b = (const int *)v2;

  return (a[1] * b[0]) - (a[0] * b[1]);
}

int search(int *arr, int val, int len) {
  int lo = 0;
  int hi = len - 1;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (arr[mid] > val)
      hi = mid - 1;
    else
      lo = mid + 1;
  }

  return hi;
}

int main() {
  int t;
  scanf("%d", &t);
  for (int case_i = 1; case_i <= t; case_i++) {
    int d, s;
    scanf("%d %d", &d, &s);
    int arr[s][2];

    for (int i = 0; i < s; i++) {
      scanf("%d %d", &arr[i][0], &arr[i][1]);
    }

    qsort(arr, s, sizeof(int[2]), cmp);
    printf("Case #%d: ", case_i);

    int code_arr[s];
    int eat_arr[s];
    code_arr[0] = arr[0][0];
    for (int i = 1; i < s; i++) {
      code_arr[i] = code_arr[i - 1] + arr[i][0];
    }
    eat_arr[s - 1] = arr[s - 1][1];
    for (int i = s - 2; i >= 0; i--) {
      eat_arr[i] = eat_arr[i + 1] + arr[i][1];
    }

    for (int day = 0; day < d; day++) {
      int a, b;
      scanf("%d %d", &a, &b);
      if (a > code_arr[s - 1] || b > eat_arr[0]) {
        printf("N");
        continue;
      }

      int i = search(code_arr, a, s);
      double code = (i == -1) ? 0 : code_arr[i];
      code = a - code;
      double eat = 0;
      if (i + 1 < s) {
        double per = code / arr[i + 1][0];
        eat = (1 - per) * arr[i + 1][1];
      }
      // printf("%f %f %d %d\n", per, eat, i, s);
      if (i + 2 < s) {
        eat += eat_arr[i + 2];
      }

      if (eat >= b) {
        printf("Y");
      } else {
        printf("N");
      }
    }

    printf("\n");
  }
}
