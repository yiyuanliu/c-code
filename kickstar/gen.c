#include <stdio.h>
#include <stdlib.h>

int main()
{
	int t = 20;
	printf("%d\n", t*2);
	for (int i = 0; i < t; i++) {
		printf("10 1\n");
		printf("%d %d\n", rand() % 100, rand() % 100);

		for (int j = 0; j < 10; j++) {
			printf("%d %d\n", rand() % 100, rand() % 100);
		}
	}

	for (int i = 0; i < t; i++) {
		printf("10 2\n");
		printf("%d %d\n", rand() % 100, rand() % 100);
		printf("%d %d\n", rand() % 100, rand() % 100);

		for (int j = 0; j < 10; j++) {
			printf("%d %d\n", rand() % 200, rand() % 200);
		}
	}
}