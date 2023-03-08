#include <bits/stdc++.h>

using namespace std;

void printArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void sortByDigit(int *arr, int n, int r, int p)
{
    // Get the current base
    int base = 1;
    while (p > 0)
    {
        p--;
        base *= r;
    }

    int count[r];
    int tmp[n];
    int m = 0;

    // Initial count value
    for (int i = 0; i < r; i++)
        count[i] = 0;

    // Count each digits
    for (int i = 0; i < n; i++)
        count[(arr[i] / base) % r]++;

    // Change value in count from appear times to accumulate appear times
    // If don't use this part, it will be unstable
    for (int i = 0, psum = 0, sum = 0; i < r; i++)
    {
        psum = sum;
        sum += count[i];
        count[i] = psum;
    }

    // Duplicate value from origin array and choose by counting index
    for (int i = 0; i < n; i++)
        tmp[count[(arr[i] / base) % r]++] = arr[i];

    for (int i = 0; i < n; i++)
        arr[i] = tmp[i];
}

void radixSort(int *arr, int n, int r, int d)
{
    // Take counting sort as subroutine
    for (int i = 0; i < d; i++)
        sortByDigit(arr, n, r, i);
}

int main()
{
    int arr[5] = {4, 2, 8};
    radixSort(arr, 3, 10, 1);
    printArr(arr, 3);
    return 0;
}