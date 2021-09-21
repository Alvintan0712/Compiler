#include <stdio.h>

const int a1 = 1;
int b1 = 6;

const int c1  = 1, c2 = 2, c3 = 3;
int v1, v2 = 2, v3;

int getint() {
    int n;
    scanf("%d", &n);
    return n;
}

int main() {

    printf("79066012\n");

    int x;
    const int ccc = 1, cccc = 2;
    int i = 0;

    {};{{}};;{{{}}};;;

    x = getint();
    x = x + (x + (1 + 2*(3 + 4) / 5 + 6 % 3 + 7 - (8 + 9)));
    while (i < x) {
        if (i == 2) {
            i = i + 2;
            continue;
        }
        else if (i == 5) break;
        i = i + 1;
    }
    printf("i = %d, x = %d\n", i, x);

    x = getint();
    i = 0;
    while (i < x) i = i + 1;
    printf("%d\n", i);

    i = 0;
    while (i < 6) {
        printf("%d\n", i);
        i = i + 1;
    }

    printf("end compile\n");

    return 0;
}
