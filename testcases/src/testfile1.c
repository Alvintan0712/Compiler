#include <stdio.h>

const int a1=1;
const int a2[2] = {2,-3};
const int a3[1][2] = {{-4,5}};
      int b1 = 6;
      int b2[2] = {7, -8};
      int b3[1][2] = {  {9  ,10}};

const int c1  = 1, c2 = 2, c3 = 3;
const int d1[2] = {4,-5}, d2[3] = {6,7,-8};
const int e1[1][2] = {{3,1}}, e2[2][3] = {{9,10,-11},{-12,13,14}};
int v1, 
    v2 = 2, 
    v3;
int 
arr1[2], arr2[3], arr3[4] = {1,-1,+-1,-+1};
int arr4[5][6], arr5[7][8];

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

void sort(int a[], int n) {
    int i = 0 + 1 - 1;
    while (i < n - 1) {
        int j = 0, t;
        while (j < n - i - 1) {
            if (a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    i = (n - 1) * 2 / 2;
    while (i >= 0) {
        printf("%d ", a[i]);
        i = i - 1;
    }
    printf("\n");
}

int getSum(int a[], int n) {
    int i = 0, sum = 0;
    while (0 <= i && i < n) {
        sum = sum + a[i];
        i = i + 1;
    }
    return sum;
}

int isPrime(int x) {
    if (x == 0 || x == 1) {
        return 0;
    } else if (x == 2) {
        return 1;
    } 
    int i = 2;
    while (i*i <= x) {
        if (x % i == 0) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void getPrimeFactor(int x) {
    int first = 1;
    if (x == 1) {
        print(x);
        return;
    }
    while (x != 1) {
        int i = 2;
        while (1) {
            if (isPrime(i) && x % i == 0)
                break;
            i = i + 1;
        }
        
        x = x / i;
        if (first) {
            printf("%d", i);
            first = 0;
        } else {
            printf("*%d", i);
        }
    }
    printf("\n");
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

int sum(int a[]) {
    int i = 0, sum = 0;
    while(i < 5) {
        sum = sum + a[i];
        i = i + 1;
    }
    return sum;
}

int 
main() {
    printMyId();
    int x, arr[10] = {-+5,+-+3,-+-4,-8,2,+-7,-9,10,-6,+1};
    sort(arr, 10);
    x = getSum(arr, 10);
    printf("%d\n", x);
    x = getint();
    getPrimeFactor(x);

    ;
    if(0);
    if(0) {}
    while(0) {}

    x = sum(arr);
    x = sum(arr + 3);
    x = isPrime(arr[0]);

    int i = 0, p[2], q[3][4];
    while (i < x) {
        if (i == 2) {
            i = i + 2;
            continue;
        }
        else if (i == 5) break;
        i = (i + 1);
    }
    if (!0) printf("! is work\n");
    x = 8 * (7 + 6) / (2 - 3) % 2;
    print(x);
    int arrr[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    x = isPrime(arrr[2][2]);
    printMatrix(arrr, 3);
    printf("end compile\n");

    return 0;
}
