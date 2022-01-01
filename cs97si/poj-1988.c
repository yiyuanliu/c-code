#include <stdio.h>
#include <stdlib.h>

#define N 30000

int father[N + 1];
int height[N + 1];
int num[N + 1];

int find(int n) {
  if (father[n] == 0)
    return n;
  int old_fa = father[n];
  int fa = find(old_fa);
  height[n] += height[old_fa];
  father[n] = fa;

  return fa;
}

// put x on y
void uni(int x, int y) {
  int x_root = find(x);
  int y_root = find(y);

  father[x_root] = y_root;
  height[x_root] = num[y_root] + 1;
  num[y_root] += num[x_root] + 1;
}

int main() {
  int p;
  scanf("%d\n", &p);
  for (int i = 0; i < p; i++) {
    char op;
    scanf("\n%c", &op);
    if (op == 'C') {
      int x;
      scanf("%d", &x);
      find(x);
      printf("%d\n", height[x]);
    } else {
      int x, y;
      scanf("%d %d", &x, &y);
      uni(x, y);
    }
  }
  return 0;
}