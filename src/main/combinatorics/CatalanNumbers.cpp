/**
 * 卡特兰数
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int ans[100];

void mutil(int x) {
    int yushu = 0;
    for (int i = 0; i < 100; i++) {
        yushu += ans[i] * x;
        ans[i] = yushu % 10000;
        yushu /= 10000;
    }
}

void div(int x) {
    int t = 0;
    for (int i = 99; i >= 0; i--) {
        t += ans[i];
        ans[i] = t / x;
        t = t % x * 10000;
    }
}

int main(int argc, char *argv[]) {
    int n;
    while (scanf("%d", &n) != EOF) {
        memset(ans, 0, sizeof(ans));
        ans[0] = 1;
        for (int i = 1; i <= n; i++) {
            mutil(n + i);
            div(i);
        }
        div(n + 1);
        int j = 99;
        while (ans[j] == 0) j--;
        printf("%d", ans[j]);
        j--;
        for (int i = j; i >= 0; i--)
            printf("%04d", ans[i]);
        putchar(10);
    }
    return 0;
}

