#include <stdio.h>

int v1, v2 = 2, v3;
int arr1[2], arr2[3], arr3[4] = {1,-1,+-1,-+1};
int arr4[1][2] = {{3,4}}, arr5[7][8];

int b1 = 6;
int b2[2] = {7,-8};
int b3[1][2] = {{9,10}};

const int a1=1;
const int a2[2] = {2,-3};
const int a3[1][2] = {{-4,5}};

const int c1  = 1, c2 = 2, c3 = 3;
const int d1[2] = {4,-5}, d2[3] = {6,7,-8};
const int e1[1][2] = {{3,1}}, e2[2][3] = {{9,10,-11},{-12,13,14}};

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
    printMyId();

    int x, arr[10] = {-+5,+-+3,-+-4,-8,2,+-7,-9,10,-6,+1};
    const int cc = 1;
    const int ccc = 1, cccc = 2;
    x = getint();
    ;
    {}
    int i;
    b3[0][0] = 0;
    while (b3[0][0] < x) {
        if (b3[0][0] == 2) {
            b3[0][0] = b3[0][0] + 2;
            continue;
        }
        else if (b3[0][0] == 5) break;
        b3[0][0] = b3[0][0] + 1;
    }
    print(b3[0][0]);

    if (!0 || 1) printf("! is work\n");
    x = 8 * (7 + 6) / (2 - 3) % 2;
    printf("%d %d\n", x, e1[0][1]);

    int arrr[3][3];
    i = 0;
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
    arr2[0] = 0;
    while (arr2[0] < x) arr2[0] = arr2[0] + 1;
    print(arr2[0]);
    printf("end compile\n");

    return 0;
}
