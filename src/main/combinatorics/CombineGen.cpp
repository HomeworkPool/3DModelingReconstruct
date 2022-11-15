
#include<iostream>

#define N 50

using namespace std;
int temp[N];

void combine(int n, int m, int lev) {
    int i;
    if (m == 0) {
        for (i = 0; i < lev; ++i)
            cout << temp[i] << " ";
        cout << endl;
    } else {
        for (i = 0; i <= n - m; ++i) {
            temp[lev++] = n - i;
            combine(n - i - 1, m - 1, lev);
            lev--;
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    int i;

    int a[N];
    for (i = 0; i < n; ++i) {
        a[i] = i + 1;
    }

    combine(n, m, 0);

    return 0;
}
