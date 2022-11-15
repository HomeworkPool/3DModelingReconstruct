/**
 * 字典序法全排列
 */

#include <iostream>

using namespace std;

void swap(int arr[], int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void reverse(int arr[], int first, int last) {
    while (first != last && first != --last) {
        swap(arr, first, last);
        first++;
    }
}


bool isChanged(int arr[], int N) {
    if (N == 0 || N == 1)
        return false;

    int last = N;
    int ii = last - 1;
    int i = ii - 1;

    while (true) {
        if (arr[i] < arr[ii])
        {
            int j = last - 1;
            while (!(arr[i] < arr[j]))
                j--;
            swap(arr, i, j);//交换
            //i位置后不包含元素逆序
            reverse(arr, ii, last);
            return true;
        }

        if (i == 0) {
            reverse(arr, i, last);
            return false;
        }

        i--;
        ii--;
    }
}

int main(int argc, char const *argv[]) {
    int size;
    cin >> size;

    int* arr = (int*) alloca(size * sizeof(int));
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;

    while (isChanged(arr, size)) {
        for (int i = 0; i < size; i++)
            cout << arr[i] << " ";
        cout << endl;
    }
    return 0;
}
