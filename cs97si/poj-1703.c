#include <stdio.h>
#include <stdlib.h>

#define N   100000

long father[2*N+10];

long stack[N];
// may too deep
long find(int n)
{
    int p = 0;
    int root = n;
    while (father[root] != 0) {
        stack[p++] = root;
        root = father[root];
    }

    while (p) {
        int i = stack[--p];
        father[i] = root;
    }

    return root;
}

void uni(int x, int y)
{
    x = find(x);
    y = find(y);
    if (x < y) {
        father[y] = x;
    } else if (x > y) {
        father[x] = y;
    }
}

int main()
{
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        int n, m;
        scanf("%d %d", &n, &m);
        for (int i = 1; i <= n; i++) {
            father[i] = 0;
            father[i+N] = 0;
        }

        for (int i = 0; i < m; i++) {
            int a, b;
            char op;
            scanf("\n%c %d %d", &op, &a, &b);
            if (op == 'A') {
                int fa_a = find(a);
                int fa_b = find(b);
                if (fa_a == fa_b) {
                    printf("In the same gang.\n");
                } else if (fa_a == find(b + N) || fa_b == find(a + N)) {
                    printf("In different gangs.\n");
                } else {
                    printf("Not sure yet.\n");
                }
            } else if (op == 'D') {
                uni(a, b+N);
                uni(b, a+N);
            }
        }
    }
}