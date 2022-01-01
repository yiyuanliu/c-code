#include <stdio.h>

int main() {
  int t;
  scanf("%d", &t);
  for (int case_i = 1; case_i <= t; case_i++) {
    int d, s;
    scanf("%d %d", &d, &s);
    printf("Case #%d: ", case_i);
    if (s == 1) {
      int c, e;
      scanf("%d %d", &c, &e);

      for (int i = 0; i < d; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        double code = (double)a / c;
        double eat = (1 - code) * e;
        if (a <= c && eat - b >= -0.00001) {
          printf("Y");
        } else {
          printf("N");
        }
      }

    } else if (s == 2) {
      int c1, e1, c2, e2;
      scanf("%d %d", &c1, &e1);
      scanf("%d %d", &c2, &e2);

      for (int i = 0; i < d; i++) {
        int a, b;
        scanf("%d %d", &a, &b);

        double a1 = a, b1 = b;
        if (a1 >= c1) {
          a1 = a1 - c1;
        } else {
          double per = a1 / c1;
          a1 = 0;
          b1 = b1 - (1 - per) * e1;
        }
        if (a1 >= c2) {
          a1 = a1 - c2;
        } else {
          double per = a1 / c2;
          a1 = 0;
          b1 = b1 - (1 - per) * e2;
        }

        double a2 = a, b2 = b;
        if (a2 >= c2) {
          a2 = a2 - c2;
        } else {
          double per = a2 / c2;
          a2 = 0;
          b2 = b2 - (1 - per) * e2;
        }
        if (a2 >= c1) {
          a2 = a2 - c1;
        } else {
          double per = a2 / c1;
          a2 = 0;
          b2 = b2 - (1 - per) * e1;
        }

        int ok1 = a1 <= 0 && b1 <= 0;
        int ok2 = a2 <= 0 && b2 <= 0;
        if (ok1 || ok2)
          printf("Y");
        else
          printf("N");
      }
    }
    printf("\n");
  }
}