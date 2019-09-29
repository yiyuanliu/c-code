#include <stdio.h>

typedef long long int64;

int64 rec[100+1];

int main() {
    for (int i = 0; i < 100000000; i++) {
        int b = i * i;
    }

    return 0;
}