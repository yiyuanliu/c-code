#include <assert.h>
#include <stdio.h>

int stack[1000000];

int main() {
  int t;
  int sp;
  scanf("%d", &t);
  while (t--) {
    sp = 0;
    while (1) {
      int n = getchar() - '0';
      if (n == 5)
        break;
      if (sp != 0 && stack[sp] + n == 10)
        sp--;
      else
        stack[sp++] = n;
    }
    getchar(); // bypass '\n'
    int area = sp >= 2 ? (sp - 2) : 0;
    if (area % 2)
      printf("%d.%d\n", area / 2, 5);
    else
      printf("%d\n", area / 2);
  }

  return 0;
}