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
    bool isMaxHeap;

public:
    Heap(int maxSize, bool isMaxHeap) : maxSize(maxSize), size_(-1), isMaxHeap(isMaxHeap)
    {
        heap_ = new T[maxSize];
    }

    Heap(int maxSize_, T *itemArray, bool isMaxHeap = true) : maxSize(maxSize_), isMaxHeap(isMaxHeap)
    {
        heap_ = itemArray;
        size_ = maxSize;
        if (isMaxHeap)
            buildMaxHeap();
        else
            buildMinHeap();
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

    void minHeapify(int index = 0)
    {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int smallest = index;
        if (leftChild < size_ && heap_[leftChild]->key < heap_[index]->key)
        {
            smallest = leftChild;
        }
        if (rightChild < size_ && heap_[rightChild]->key < heap_[smallest]->key)
        {
            smallest = rightChild;
        }
        if (smallest != index)
        {
            T temp = heap_[index];
            heap_[index] = heap_[smallest];
            heap_[smallest] = temp;
            minHeapify(smallest);
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

    void buildMinHeap()
    {
        int middle = parent(maxSize - 1);
        for (int i = middle; i >= 0; i--)
        {
            minHeapify(i);
        }
    }

    void ascHeapSort()
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

    void descHeapSort()
    {
        int tempSize = size_;
        buildMinHeap();
        for (int i = size_; i > 1; i--)
        {
            T temp = heap_[0];
            heap_[0] = heap_[i];
            heap_[i] = temp;
            size_--;
            minHeapify();
        }
        size_ = tempSize;
    }

    void insert(T item, int minOrMax = 1)
    {
        size_++;
        heap_[size_] = item;
        int index = size_;
        while (index > 0 && isMaxHeap ? heap_[parent(index)]->key < heap_[index]->key : heap_[parent(index)]->key > heap_[index]->key)
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
        isMaxHeap ? maxHeapify(index) : minHeapify(index);
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
    heap2.insert(new Node{9});
    heap2.insert(new Node{0});
    heap2.insert(new Node{12});
    heap2.insert(new Node{13});
    // why that is inserting even when the vector is full?
    for (int i = 0; i < heap2.size(); i++)
    {
        cout << heap2.heap()[i]->key << endl;
    }
    cout << endl;
    heap2.descHeapSort();
    for (int i = 0; i < heap2.size(); i++)
    {
        cout << heap2.heap()[i]->key << endl;
    }

    return 0;
}