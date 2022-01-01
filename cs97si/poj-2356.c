#include <stdio.h>

int stack[10000 + 1][3];
int sp;
int rec[10000 + 1];

int main() {
  int N;
  scanf("%d", &N);

  sp = 1;
  for (int i = 0; i < N; i++) {
    int v;
    scanf("%d", &v);
    int old_sp = sp;
    for (int j = 0; j < old_sp; j++) {
      int r = (v + stack[j][0]) % N;
      if (r == 0) {
        // find ans
        int num = 1;
        int pre = j;
        while (pre != 0) {
          num++;
          pre = stack[pre][2];
        }
        printf("%d\n", num);
        printf("%d\n", v);
        pre = j;
        while (pre != 0) {
          printf("%d\n", stack[pre][1]);
          pre = stack[pre][2];
        }
        return 0;
      } else if (rec[r] == 0) {
        // find first time
        rec[r] = 1;
        stack[sp][0] = r;
        stack[sp][1] = v;
        stack[sp][2] = j;
        sp++;
      }
    }
  }

  printf("0\n");
  return 0;
}