#include <stdio.h>

int not_prime[1000000 + 1];
int primes[1000000];

int main() {
  // just need to make a prime num table
  not_prime[1] = not_prime[2] = 1;
  for (int i = 2; i < 1000000; i++) {
    for (int j = 2; j <= i; j++) {
      int k = i * j;
      if (k <= 1000000)
        not_prime[k] = 1;
      else
        break;
    }
  }

  int num = 0;
  for (int i = 1; i < 1000000; i++) {
    if (!not_prime[i]) {
      primes[num++] = i;
    }
  }

  while (1) {
    int n;
    scanf("%d", &n);
    if (n == 0)
      return 0;
    for (int i = 0; i < num; i++) {
      int a = primes[i];
      if (a * 2 > n)
        printf("Goldbach's conjecture is wrong.\n");
      else if (!not_prime[n - a]) {
        printf("%d = %d + %d\n", n, a, n - a);
        break;
      }
    }
  }

  return 0;
}