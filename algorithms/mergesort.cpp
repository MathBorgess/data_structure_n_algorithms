#include <iostream>
#include <chrono>

using namespace std;

template <typename T>
class MergeSort
{
public:
    static T *merge(T arr_1[], T arr_2[], int size1, int size2)
    {
        int totalSize = size1 + size2;
        T *arr = new T[totalSize]{};
        int i = 0, j = 0, k = 0;
        while (i < size1 && j < size2)
        {
            if (arr_1[i] < arr_2[j])
            {
                arr[k++] = arr_1[i++];
            }
            else
            {
                arr[k++] = arr_2[j++];
            }
        }
        while (i < size1)
        {
            arr[k++] = arr_1[i++];
        }
        while (j < size2)
        {
            arr[k++] = arr_2[j++];
        }
        return arr;
    }

    static void mergeSort(T arr[], int size)
    {
        for (int i = 1; i < size; i *= 2)
        {
            for (int j = 0; j < size; j += 2 * i)
            {
                int initIndex = j;
                int endIndex = j + 2 * i - 1;
                int midIndex = ((initIndex + endIndex) - ((initIndex + endIndex) % 2)) / 2;
                if (endIndex >= size)
                {
                    endIndex = size - 1;
                }
                if (midIndex >= size)
                {
                    midIndex = size - 1;
                }
                T *mergedArr = merge(&arr[initIndex], &arr[midIndex + 1], midIndex - initIndex + 1, endIndex - midIndex);
                for (int k = initIndex; k <= endIndex; k++)
                {
                    arr[k] = mergedArr[k - initIndex];
                }
                delete[] mergedArr;
            }
        }
    }
};

int main()
{

    int bigSize = 10000000;
    int *bigArr = new int[bigSize];
    for (int i = bigSize - 1; i >= 0; i--)
    {
        bigArr[bigSize - i - 1] = i;
    }
    cout << endl;
    cout << "Sorting " << bigSize << " elements..." << endl;

    auto start_time = chrono::high_resolution_clock::now();

    MergeSort<int>::mergeSort(bigArr, bigSize);

    auto end_time = chrono::high_resolution_clock::now();

    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Execution time: " << duration_ms << " milliseconds" << endl;
    return 0;
}