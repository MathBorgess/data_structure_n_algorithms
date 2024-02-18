#include <iostream>

using namespace std;

struct Item
{
    int key;
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

    ~LinkedList()
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            Node<T> *temp = node;
            node = node->next;
            delete temp;
        }
        delete node;
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

    T remove(int key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data->key != key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return nullptr;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        size_--;
        return node->data;
    }

    T search(int key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data.key != key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
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

    T *list()
    {
        T *list = new T[size_];
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_; i++)
        {
            list[i] = node->data;
            node = node->next;
        }
        return list;
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
    int (*hash)(int, int);
    int (*updateMaxSize)(int);

public:
    int size() const { return maxSize; }

    LinkedList<T> *table() const { return hashTable_; }

    OpenedHashTable(int maxSize, float maxLoadFactor, int (*updateMaxSize)(int), int (*hash)(int, int))
        : maxSize(maxSize), maxLoadFactor(maxLoadFactor), size_(0), updateMaxSize(updateMaxSize), hash(hash)
    {
        hashTable_ = new LinkedList<T>[maxSize] {};
    }
    ~OpenedHashTable()
    {
        delete[] hashTable_;
    }

    void insert(T item, bool rehashing = false)
    {
        if (size_ >= maxSize * maxLoadFactor && !rehashing)
        {
            size_ = 0;
            rehash();
        }
        int index = hash(item->key);
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

    T search(int key)
    {
        int index = hash(key);
        if (hashTable_[index].size() == 0)
        {
            return T();
        }
        return hashTable_[index].search(key);
    }

    T remove(int key)
    {
        int index = hash(key);
        if (hashTable_[index].size() == 0)
        {
            return T();
        }
        size_--;
        return hashTable_[index].remove(key);
    }
};

int updateMaxSize(int maxSize)
{
    return maxSize * 2;
}

int hash(int key, int maxSize)
{
    return key % maxSize;
}

int main()
{
    OpenedHashTable<Item *> *hashTable = new OpenedHashTable<Item *>(2, 0.5, &updateMaxSize, &hash);
    hashTable->insert(new Item{1});
    hashTable->insert(new Item{2});
    hashTable->insert(new Item{11});
    hashTable->insert(new Item{15});
    hashTable->insert(new Item{17});
    hashTable->insert(new Item{18});
    hashTable->insert(new Item{20});
    // what might I do with equal keys?
    for (int i = 0; i < hashTable->size(); i++)
    {
        if (hashTable->table()[i].size() > 0)
        {
            Item **list = hashTable->table()[i].list();
            for (int j = 0; j < hashTable->table()[i].size(); j++)
            {
                cout << list[j]->key << endl;
            }
        }
    }

    return 0;
}