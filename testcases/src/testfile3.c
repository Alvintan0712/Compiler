#include <stdio.h>

int getint() {
    int n;
    scanf("%d", &n);
    return n;
}

void printMyId() {
    printf("79066012\n");
}

void print(int x) {
    printf("x = %d\n", x);
}

void printMatrix(int a[][3], int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < 3) {
            printf("%d ", a[i][j]);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

int getMult(int a[], int n) {
    int i = 0, res = 1;
    while (i < n) {
        res = res * a[i];
        i = i + 1;
    }
    return res;
}

int main() {
    const int a1 = 1;
    const int a2[2] = {2,-3};
    const int a3[2][2] = {{-4,5},{6,7}};
    int b1 = 6;
    int b2[2] = {7, -8};
    int b3[2][2] = {{9,10},{11,12}};

    const int c1  = 1, c2 = 2, c3 = 3;
    const int d1[2] = {4,-5}, d2[3] = {6,7,-8};
    const int e1[2][2] = {{3,1},{-1,-3}}, e2[2][3] = {{9,10,-11},{-12,13,14}};
    int v1, v2 = 2, v3;
    int arr1[2], arr2[3], arr3[4] = {1,-1,+-1,-+1};
    int arr4[5][6], arr5[7][8];

    printMyId();

    int x, arr[10] = {-+5,+-+3,-+-4,-8,2,+-7,-9,10,-6,+1};
    int i = 0;

    {;}{{;;}}{{{;;;}}}

    x = getint();
    x = x * (7 + 6) / (2 - (x % 1)) + x;
    while (i < x) {
        if (i == 2) {
            i = i + 2;
            continue;
        }
        else if (i == 5) break;
        i = i + 1;
    }
    print(i); print(x);

    i = 0;
    int arrr[3][3];
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            arrr[i][j] = getint();
            j = j + 1;
        }
        i = i + 1;
    }
    printMatrix(arrr, 3);

    int res = getMult(arr, 10);
    print(res);

    x = getint();
    i = 0;
    while (i < x) i = i + 1;

    i = 0; res = 1;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            res = res * arrr[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    print(res);

    printf("end compile\n");

    return 0;
}
