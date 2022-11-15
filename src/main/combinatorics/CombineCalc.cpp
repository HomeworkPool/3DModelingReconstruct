/**
 * 组合计算
 */

#include <iostream>

using namespace std;

bool nextCombine(int *combine, const int n, const int k) {
    int i = k - 1;
    const int e = n - k;
    do
        combine[i]++;
    while (combine[i] > e + i && i--);
    if (combine[0] > e)
        return false;
    while (++i < k)
        combine[i] = combine[i - 1] + 1;
    return true;
}

int main() {
    int n, k;
    cout << "combine(n, k):" << endl;
    cin >> n >> k;

    if (n < k || k <= 0)
        return 0;

    int *comb = new int[k];
    for (int i = 0; i < k; i++)
        comb[i] = i;

    do {
        for (int i = 0; i < k; cout << ((++i < k) ? ',' : '\n'))
            cout << comb[i] + 1;
    } while (nextCombine(comb, n, k));

    delete[] comb;
    return 0;
}

