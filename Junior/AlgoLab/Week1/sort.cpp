#include <bits/stdc++.h>

using namespace std;

void printArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int *sort(int *arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        int j = i, val = arr[j];

        // arr[0->i-1] are sorted, arr[i->n-1] are unsorted
        while (j > 0 && !(arr[j - 1] >= val))
        {
            arr[j] = arr[j - 1];
            j--;
            printArr(arr, n);
        }
        arr[j] = val;
        printArr(arr, n);
    }
    return arr;
}

int main()
{
    int arr[5] = {3, 5, 2, 1, 4};
    sort(arr, 5);
    printArr(arr, 5);
    return 0;
}