#include <stdio.h>

int main() {
  int N;
  scanf("%d", &N);
  while (N--) {
    int n;
    scanf("%d", &n);
    int min = (n & (n - 1)) + 1;
    int max = (n | (n - 1));
    printf("%d %d\n", min, max);
  }

  return 0;
}