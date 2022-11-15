/**
 * 卡特兰数
 */

#include <cstdio>
#include <cstring>

using namespace std;

int ans[128];

void mul(int x) {
    int rem = 0;
    for (int i = 0; i < 100; i++) {
        rem += ans[i] * x;
        ans[i] = rem % 10000;
        rem /= 10000;
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
            mul(n + i);
            div(i);
        }

        div(n + 1);
        int j = 99;
        while (ans[j] == 0) j--;
        printf("%d", ans[j]);
        j--;

        for (int i = j; i >= 0; i--)
            printf("%08d ", ans[i]);
        putchar(' ');
    }
    return 0;
}

