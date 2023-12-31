#include <iostream>

using namespace std;

struct Item
{
    string key;
    int row;
    int priority;
    int inscriptionNumber;
};

template <typename T>
struct Node
{
    T data;
    Node<T> *next;
    Node<T> *prev;
};

template <typename T>
class LinkedList
{
private:
    Node<T> sentinel = {nullptr, &sentinel, &sentinel};
    int size_ = 0;

public:
    Node<T> *makeNode(T item)
    {
        return new Node<T>{item, nullptr, nullptr};
    }

    void enqueue(T item)
    {
        Node<T> *node = makeNode(item);
        node->prev = &sentinel;
        node->next = sentinel.next;
        sentinel.next->prev = node;
        sentinel.next = node;
        size_++;
    }

    T remove(string key, int inscriptionNumber)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data->key != key && node->data->inscriptionNumber != inscriptionNumber)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel || node->data->key != key || node->data->inscriptionNumber != inscriptionNumber)
        {
            return nullptr;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        size_--;
        return node->data;
    }

    T search(string key, int inscriptionNumber)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data->key != key && node->data->inscriptionNumber != inscriptionNumber)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel || node->data->key != key || node->data->inscriptionNumber != inscriptionNumber)
        {
            return nullptr;
        }
        return node->data;
    }

    T shift()
    {
        size_--;
        sentinel.next->next->prev = &sentinel;
        Node<T> *item = sentinel.next;
        sentinel.next = sentinel.next->next;
        return item->data;
    }

    int size() const { return size_; }
};

template <typename T>
class OpenedHashTable
{
private:
    int size_;
    int maxSize;
    float maxLoadFactor;
    LinkedList<T> *hashTable_;
    int (*hash)(string, int);
    int (*updateMaxSize)(int);

public:
    OpenedHashTable(int maxSize, float maxLoadFactor, int (*updateMaxSize)(int), int (*hash)(string, int))
        : maxSize(maxSize), maxLoadFactor(maxLoadFactor), size_(0), updateMaxSize(updateMaxSize), hash(hash)
    {
        hashTable_ = new LinkedList<T>[maxSize] {};
    }

    void insert(T item, bool rehashing = false)
    {
        if (size_ >= maxSize * maxLoadFactor && !rehashing)
        {
            size_ = 0;
            rehash();
        }
        int index = hash(item->key, maxSize);
        hashTable_[index].enqueue(item);
        size_++;
    }

    void rehash()
    {
        LinkedList<T> *oldHashTable = hashTable_;
        int newSize = updateMaxSize(maxSize);
        hashTable_ = new LinkedList<T>[newSize] {};
        for (int i = 0; i < maxSize; i++)
        {
            if (oldHashTable[i].size() > 0)
            {
                int oldTableSize = oldHashTable[i].size();
                for (int j = 0; j < oldTableSize; j++)
                {
                    insert(oldHashTable[i].shift(), true);
                }
            }
        }
        delete[] oldHashTable;
        maxSize = newSize;
    }

    T search(string key, int inscriptionNumber)
    {
        int index = hash(key, maxSize);
        if (hashTable_[index].size() == 0)
        {
            return nullptr;
        }
        return hashTable_[index].search(key, inscriptionNumber);
    }

    T remove(string key, int inscriptionNumber)
    {
        int index = hash(key, maxSize);
        if (hashTable_[index].size() == 0)
        {
            return nullptr;
        }
        size_--;
        return hashTable_[index].remove(key, inscriptionNumber);
    }
};

template <typename T>
class Heap
{
private:
    int size_;
    int maxSize;
    bool isMaxHeap;
    T *heap_;

public:
    Heap(int maxSize, bool isMax) : maxSize(maxSize), size_(-1), isMaxHeap(isMax)
    {
        heap_ = new T[maxSize];
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
        if (leftChild < size_ && heap_[leftChild]->priority > heap_[index]->priority)
        {
            largest = leftChild;
        }
        if (rightChild < size_ && heap_[rightChild]->priority > heap_[largest]->priority)
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
        if (leftChild < size_ && heap_[leftChild]->priority < heap_[index]->priority)
        {
            smallest = leftChild;
        }
        if (rightChild < size_ && heap_[rightChild]->priority < heap_[smallest]->priority)
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

    void insert(T item)
    {
        size_++;
        heap_[size_] = item;
        int index = size_;
        while (index > 0 && isMaxHeap ? heap_[parent(index)]->priority < heap_[index]->priority : heap_[parent(index)]->priority > heap_[index]->priority)
        {
            T temp = heap_[index];
            heap_[index] = heap_[parent(index)];
            heap_[parent(index)] = temp;
            index = parent(index);
        }
    }

    void remove(string key, int inscriptionNumber)
    {
        int index = 0;
        while (index <= size_ && (heap_[index]->key != key || heap_[index]->inscriptionNumber != inscriptionNumber))
        {
            index++;
        }
        if (index > size_)
        {
            return;
        }
        T temp = heap_[index];
        heap_[index] = heap_[size_];
        heap_[size_] = temp;
        size_--;
        isMaxHeap ? maxHeapify(index) : minHeapify(index);
    }
    int size() const { return size_ + 1; }

    T critical() const { return heap_[0]; }

    T updateCritial()
    {
        T temp = heap_[0];
        heap_[0] = heap_[size_];
        heap_[size_] = nullptr;
        size_--;
        if (isMaxHeap)
        {
            maxHeapify();
        }
        else
        {
            minHeapify();
        };
        return temp;
    }

    T *heap() const { return heap_; }
};

int updateMaxSize(int maxSize)
{
    return maxSize * 2 + 1;
}

int hashFuntion(string key, int maxSize)
{
    int keyValue = 0;
    for (int i = 0; i < key.length(); i++)
    {
        keyValue += int(key[i]) * (i + 1);
    }
    return (keyValue * 17) % maxSize;
}

int main()
{
    int rows, seats, commands, priority;
    int cadNumber = 0;
    string command, key;
    cin >> rows >> seats >> commands;
    Heap<Item *> heap(rows * seats, true);
    Heap<Item *> seatedHeap(rows * seats, false);
    OpenedHashTable<Item *> hashTable(2 * rows * seats, 0.8, &updateMaxSize, &hashFuntion);
    int *filledSeatsRows = new int[rows]{0};

    for (int i = 0; i < commands; i++)
    {
        cin >> command >> key >> priority;

        if (command == "CAD")
        {
            cadNumber++;
            Item *item = new Item{key, -1, priority, cadNumber};
            for (int j = 0; j < rows; j++)
            {
                if (filledSeatsRows[j] < seats)
                {
                    item->row = j;
                    filledSeatsRows[j]++;
                    seatedHeap.insert(item);
                    break;
                }
            }

            if (item->row == -1 && seatedHeap.size() > 0 && seatedHeap.critical()->priority < item->priority)
            {
                Item *item2 = seatedHeap.updateCritial();
                item->row = item2->row;
                seatedHeap.insert(item);
                item2->row = -1;
                heap.insert(item2);
            }

            if (item->row == -1)
            {
                cout << item->key << " (" << cadNumber << ") "
                     << "nao foi alocado(a) em nenhuma fileira" << endl;
                heap.insert(item);
            }
            else
            {
                cout << item->key << " (" << cadNumber << ") "
                     << "foi alocado(a) na fileira " << item->row + 1 << endl;
            }
            hashTable.insert(item);
        }
        else if (command == "REM")
        {
            Item *item = hashTable.remove(key, priority);
            if (item == nullptr)
            {
                cout << "Inexistente" << endl;
            }
            else if (item->row == -1)
            {
                heap.remove(item->key, item->inscriptionNumber);
                delete item;
                cout << "Removido(a)" << endl;
            }
            else
            {
                if (heap.size() != 0)
                {
                    Item *item2 = heap.updateCritial();
                    item2->row = item->row;
                    seatedHeap.remove(item->key, item->inscriptionNumber);
                    delete item;
                    seatedHeap.insert(item2);
                }
                else
                {
                    seatedHeap.remove(item->key, item->inscriptionNumber);
                    filledSeatsRows[item->row]--;
                    delete item;
                }
                cout << "Removido(a)" << endl;
            }
        }
        else if (command == "VER")
        {
            Item *item = hashTable.search(key, priority);

            if (item != nullptr)
            {
                if (item->row == -1)
                {
                    cout << "Sem assento" << endl;
                }
                else
                {
                    cout << "Sentado(a) na fileira " << item->row + 1 << endl;
                }
            }
            else
            {
                cout << "Inexistente" << endl;
            }
        }
    }
    return 0;
}
