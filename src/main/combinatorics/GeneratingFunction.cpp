/**
 * 母函数
 */

#include<stdio.h>
#include<string.h>

int main() {
    int t, i, j, k, a[27] = {0};
    int b[51], c[51];
    scanf("%d", &t);//输入测试用例数目
    while (t--) {
        for (i = 1; i <= 26; i++) scanf("%d", &a[i]);
        memset(b, 0, sizeof(b));
        memset(c, 0, sizeof(c));
        b[0] = 1;//一个不取算一种情况
        for (i = 1; i <= 26; i++) {
            for (j = 0; j <= 50; j++)
                for (k = 0; k <= a[i] && j + k * i <= 50; k++) c[j + k * i] += b[j];
            memcpy(b, c, sizeof(c));
            memset(c, 0, sizeof(c));
        }
        int num = 0;
        for (i = 1; i <= 50; i++) num += b[i];
        printf("%d\n", num);
    }
    return 0;
}
