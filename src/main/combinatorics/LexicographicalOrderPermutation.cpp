#include <iostream>
#include <string.h>
#include <stdio.h>

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

//true has changed
//false has not changed
bool is_changed(int arr[], int N) {
    if (N == 0 || N == 1)
        return false;
    int last = N;
    int ii = last - 1;
    int i = ii - 1;
    while (true) {
        if (arr[i] < arr[ii])//找到一个正序列
        {
            int j = last - 1;
            while (!(arr[i] < arr[j]))//再从右向左找第一个比arr[i]大的
                j--;
            swap(arr, i, j);//交换
            //i位置后不包含元素逆序
            reverse(arr, ii, last);//将第一次找见的正序数的大者从她后面开始全部倒序
            return true;
        }
        //已经是递减序列，无下一个序列
        if (i == 0) {
            //全部倒序恢复最初状态
            reverse(arr, i, last);
            return false;
        }
        i--;
        ii--;
    }
}

int main(int argc, char const *argv[]) {
    /* code */
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(int);
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
    while (is_changed(arr, size)) {
        for (int i = 0; i < size; i++)
            cout << arr[i] << " ";
        cout << endl;
    }
    return 0;
}
