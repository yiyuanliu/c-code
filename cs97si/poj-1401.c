#include <stdio.h>

// return number of zero at the end of n!
int z(long long n) {
    int ans = 0;
    long long base = 5;
    while (n >= base) {
        ans += n / base;
        base = base * 5;
    }

    return ans;
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) {
        long long n;
        scanf("%lld", &n);
        printf("%d\n", z(n));
    }
    return 0;
}