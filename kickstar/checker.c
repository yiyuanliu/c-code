#include <assert.h>
#include <stdio.h>

unsigned int tmp_table[100000 + 1];
unsigned int table[100000 + 1];
int table_len = 0;

int main() {
  for (int i = 2; i <= 100000; i++) {
    if (tmp_table[i])
      continue;
    int j = 2;
    while (1) {
      int tmp = i * j;
      if (i * j > 100000)
        break;
      tmp_table[tmp] = 1;

      j++;
    }
  }

  for (int i = 2; i <= 100000; i += 1) {
    if (tmp_table[i])
      continue;
    table[table_len++] = i;
  }

  // table size: table len,
  // table start with 2;
  int t;
  scanf("%d", &t);
  for (int i = 1; i <= t; i++) {
    int l, r;
    scanf("%d %d", &l, &r);
    int sum = 0;
    while (l <= r) {
      // check number l
      int n2 = 0;
      int v = l;
      while ((l & 1) == 0) {
        l = l >> 1;
        n2++;
      }

      int old_sum = sum;
      switch (n2) {
      case 0:
        // l is zhisu or 1
        if (l <= 100000) {
          sum += tmp_table[l] == 0;
        } else {
          int find = 0;
          int findv = 0;
          for (int i = 0; i < table_len; i++) {
            if (l % table[i] == 0) {
              find = 1;
              findv = table[i];
              break;
            }
          }
          sum += find == 0;
        }
        break;
      case 1:
        sum++; // always right
        break;
      case 2:
        // l is zhisu or 1
        if (l <= 100000) {
          sum += tmp_table[l] == 0;
        } else {
          int find = 0;
          for (int i = 0; i < table_len; i++) {
            if (l % table[i] == 0) {
              find = 1;
              break;
            }
          }

          sum += find == 0;
        }
        break;
      case 3:
        sum += (l == 1) ? 1 : 0;
        break;
      default:
        // no
        break;
      }

      l = v + 1;
    }

    printf("Case #%d: %d\n", i, sum);
  }
}