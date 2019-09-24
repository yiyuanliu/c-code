# Mathematics

Sum of powers:
1. sum_of_n = n*(n+1)/2;
2. sum_of_n2 = n*(n+1)*(2n+1)/6;
3. sum_of_n3 = sum_of_n ^ 2;


Fast Exp:
```
// a ^ n
int exp(a, n)
{
    int ans = 1;
    int base = a;
    while (n) {
        if (n % 2) ans = ans * base;
        base = a * a;
        n = n / 2;
    }

    return ans;
}
```

GCD:
1. gcd(a, b) = gcd(a, b - a); gcd(a, 0) = a;
```
int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int gcd(int a, int b)
{
    while (b)   {int r = a % b; a = b; b = r;}
    return a;
}
```
2. gcd(a, b) is smallest positive number in {ax + by}
```
// return gcd as ax + by
void gcd2(int a, int b)
{
    int old_a = a, old_b = b;
    int xa = 1, ya = 0;     // a = 1a + 0b;
    int xb = 0, yb = 1;     // b = 0a + 1b;
    while (b) {
        int r = a % b;
        int q = a / b;
        int xr = xa - q * xb;
        int yr = ya - q * yb;
        a = b; xa = xb; ya = yb;
        b = r; xb = xr; yb = yr;
    }

    printf("%d, %d, %d\n", xa, ya, xa * old_a + ya * old_b);
}
```

Mod:
1. x == y (mod n)
2. x*y == 0 (mod n), y == x^-1 (mod n)
3. x^-1 exists <=> gcd(x, n) = 1, (smallest ax + bn is 1); 
    we can use gcd(x, n) to find x^-1, see gcd2
4. Chianese Remainder:
    1. give a, b, m, n, gcd(m, n) = 1, find x == a(mod m) && x == b(mod n)
    2. gcd(m, n) = 1, easy to find m^-1(mod n) and n^-1(mod m)
    3. ann^-1 + bmm^-1; when mod m, we want a, so multiple m with nn^-1

Use fast exponentiation to compute the matrix power

Geometry:
1. float is not accurate! can use int in some special situation.
2. use double instead of float
3. equality test, not a == b, abs(a - b) < EPS
4. area of triangle, A, B, C, 2S = |(B-A)x(C-A)|
    - cross product:(x1,y1) x (x2,y2) = x1y2 - x2y1.(Very important)
5. area of polygon:
    - convert into traingles, add theme all