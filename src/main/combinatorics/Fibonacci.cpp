/**
 * 斐波那契数列
 */

#include <stdio.h>
#include <stdlib.h>

int fibonacci(int i) {
    if (i <= 2)
        return i == 0 ? 0 : 1;//满足递归结束的条件
    return fibonacci(i - 1) + fibonacci(i - 2);
}

int main(void) {
    int i;

    for (i = 1; i <= 12; i++)
        printf("经过%d次之后的数量：%d\n", i, fibonacci(i));
    system("pause");
    return 0;
}


