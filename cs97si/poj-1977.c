#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double sol(double R, int n)
{
    return sin(M_PI / n) * R / (1 + sin(M_PI / n));
}

int main()
{
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        double R;
        int n;
        scanf("%lf %d", &R, &n);
        printf("Scenario #%d:\n%.3f\n", i, sol(R, n));
        if (i != num)
            printf("\n");
    }

    return 0;
}