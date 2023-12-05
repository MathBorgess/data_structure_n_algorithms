#include <iostream>

using namespace std;

struct Node
{
    int key;
};

template <typename T>
class Heap
{
private:
    int size_;
    int maxSize;
    T *heap_;

public:
    Heap(int maxSize) : maxSize(maxSize), size_(-1)
    {
        heap_ = new T[maxSize];
    }

    Heap(int maxSize_, T *itemArray) : maxSize(maxSize_)
    {
        heap_ = itemArray;
        size_ = maxSize;
        buildMaxHeap();
    }

    int parent(int index)
    {
        return (index - index % 2) / 2;
    }

    void maxHeapify(int index = 0)
    {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int largest = index;
        if (leftChild < size_ && heap_[leftChild]->key > heap_[index]->key)
        {
            largest = leftChild;
        }
        if (rightChild < size_ && heap_[rightChild]->key > heap_[largest]->key)
        {
            largest = rightChild;
        }
        if (largest != index)
        {
            T temp = heap_[index];
            heap_[index] = heap_[largest];
            heap_[largest] = temp;
            maxHeapify(largest);
        }
    }

    void buildMaxHeap()
    {
        int middle = parent(maxSize - 1);
        for (int i = middle; i >= 0; i--)
        {
            maxHeapify(i);
        }
    }

    void heapSort()
    {
        int tempSize = size_;
        buildMaxHeap();
        for (int i = size_; i > 1; i--)
        {
            T temp = heap_[0];
            heap_[0] = heap_[i];
            heap_[i] = temp;
            size_--;
            maxHeapify();
        }
        size_ = tempSize;
    }

    void insert(T item)
    {
        size_++;
        heap_[size_] = item;
        int index = size_;
        while (index > 0 && heap_[parent(index)]->key < heap_[index]->key)
        {
            T temp = heap_[index];
            heap_[index] = heap_[parent(index)];
            heap_[parent(index)] = temp;
            index = parent(index);
        }
    }

    void remove(int index)
    {
        T temp = heap_[index];
        heap_[index] = heap_[size_];
        heap_[size_] = temp;
        size_--;
        maxHeapify(index);
    }
    int size() const { return size_ + 1; }

    T *heap() const { return heap_; }
};

int main()
{
    Heap<Node *> heap2(5);
    heap2.insert(new Node{1});
    heap2.insert(new Node{8});
    heap2.insert(new Node{5});
    for (int i = 0; i < heap2.size(); i++)
    {
        cout << heap2.heap()[i]->key << endl;
    }
    cout << endl;
    heap2.heapSort();
    for (int i = 0; i < heap2.size(); i++)
    {
        cout << heap2.heap()[i]->key << endl;
    }

    return 0;
}