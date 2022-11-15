/**
 * 序数法生成全排列
 */

#include<cstdio>
#include<cstdlib>
#include<cstring>

void generateA(int *a, int N, int n) {
    int i;
    for (i = 1; N > 0; i++) {
        a[i] = N % (i + 1);
        N = N / (i + 1);
    }
    while (i <= n - 1) {
        a[i++] = 0;
    }
}


void generateP(int *p, int *a, int n) {
    int i, j;

    memset(p, 0, (n + 1) * sizeof(int));

    for (i = n - 1; i >= 1; i--) {

        j = n;
        while (1) {
            if (p[j] == 0) {
                a[i]--;
                if (a[i] < 0) {
                    break;
                }
            }
            j--;
        }

        p[j] = i + 1;
    }

    for (i = 1; i <= n; i++) {
        if (p[i] == 0) {
            p[i] = 1;
        }
    }
}


void print(char *s, int *p, int n, int reverse) {
    int i;
    printf("%s", s);
    if (!reverse) {
        for (i = 1; i < n; i++) {
            printf(" %d", p[i]);
        }
    } else {
        for (i = n - 1; i >= 1; i--) {
            printf(" %d", p[i]);
        }
    }
    printf("\n");
}

int main() {
    int i, n, N, *a, *p;
    printf("Enter value of n: ");
    scanf("%d", &n);
    a = (int *) malloc(n * sizeof(int));
    p = (int *) malloc((n + 1) * sizeof(int));

    //求n的阶乘
    for (i = n, N = 1; i > 1; i--) {
        N *= i;
    }
    //循环求出p数组
    for (i = 0; i < N; i++) {
        generateA(a, i, n);
        print("A: ", a, n, 1);
        generateP(p, a, n);
        print("P: ", p, n + 1, 0);
        printf("\n");
    }
    scanf("%d", &n);
    free(a);
    free(p);
}
