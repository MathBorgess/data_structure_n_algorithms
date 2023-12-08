#include <iostream>
#include <chrono>

using namespace std;

template <typename T>
class MergeSort
{
public:
    static void merge(T aux[], T arr_1[], T arr_2[], int size1, int size2)
    {
        int totalSize = size1 + size2;
        int i = 0, j = 0, k = 0;
        while (i < size1 && j < size2)
        {
            if (arr_1[i] < arr_2[j])
            {
                aux[k++] = arr_1[i++];
            }
            else
            {
                aux[k++] = arr_2[j++];
            }
        }
        while (i < size1)
        {
            aux[k++] = arr_1[i++];
        }
        while (j < size2)
        {
            aux[k++] = arr_2[j++];
        }
    }

    static void mergeSort(T arr[], int size)
    {
        T *aux = new T[size];
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
                merge(aux, &arr[initIndex], &arr[midIndex + 1], midIndex - initIndex + 1, endIndex - midIndex);
                for (int k = initIndex; k <= endIndex; k++)
                {
                    arr[k] = aux[k - initIndex];
                }
            }
        }
        delete[] aux;
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