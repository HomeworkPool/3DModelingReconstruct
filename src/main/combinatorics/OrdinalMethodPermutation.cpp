/*
	序数法生成全排列
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//注意:为了更好理解，a数组的实际长度为n，p数组的实际长度为n+1，a[0]和p[0]弃之不用

//生成a1,a2,a3……
void generate_a(int *a, int N, int n) {
    int i;
    for (i = 1; N > 0; i++) {
        a[i] = N % (i + 1);
        N = N / (i + 1);
    }
    while (i <= n - 1) {
        a[i++] = 0;
    }
}

//由a生成p
void generate_p(int *p, int *a, int n) {
    int i, j;
    //先将p置为0

    memset(p, 0, (n + 1) * sizeof(int));

    for (i = n - 1; i >= 1; i--) {
        //让j下标指向一个元素，这个元素为从p的右（下标最大）边向左（下标小）边，第一个还没有被占用的元素开始数，数a[i]个数位
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
        //然后将这个数位上的数置为i+1
        p[j] = i + 1;
    }
    //最后一个元素位置填上1
    for (i = 1; i <= n; i++) {
        if (p[i] == 0) {
            p[i] = 1;
        }
    }
}

//数组打印函数
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
    printf("请输入n的值：");
    scanf("%d", &n);
    a = (int *) malloc(n * sizeof(int));
    p = (int *) malloc((n + 1) * sizeof(int));

    //求n的阶乘
    for (i = n, N = 1; i > 1; i--) {
        N *= i;
    }
    //循环求出p数组
    for (i = 0; i < N; i++) {
        generate_a(a, i, n);
        print("a", a, n, 1);
        generate_p(p, a, n);
        print("p", p, n + 1, 0);
        printf("\n");
    }
    scanf("%d", &n);
    free(a);
    free(p);
}
