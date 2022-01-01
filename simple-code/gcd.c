#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int gcd_num(int a, int b) {
  while (b) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a;
}

void gcd(int a, int b) {
  int old_a = a, old_b = b;
  int xa = 1, ya = 0;
  int xb = 0, yb = 1;
  while (b) {
    int r = a % b;
    int q = a / b;
    int xr = xa - q * xb;
    int yr = ya - q * yb;
    a = b;
    xa = xb;
    ya = yb;
    b = r;
    xb = xr;
    yb = yr;
  }

  printf("%d, %d, %d\n", xa, ya, xa * old_a + ya * old_b);
  assert(gcd_num(a, b) == xa * old_a + ya * old_b);
}

int main() {
  while (true) {
    int a, b;
    scanf("%d %d", &a, &b);
    gcd(a, b);
  }
}