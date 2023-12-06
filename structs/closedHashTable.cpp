#include <iostream>

using namespace std;

struct Item
{
    int key;
};

template <typename T>
class ClosedHashTable
{
private:
    int size_;
    int maxSize;
    float maxLoadFactor;
    T *hashTable_;
    bool *flag;
    int hash(int key, int steps = 0)
    {
        return (key + steps) % maxSize;
    }
    int (*updateMaxSize)(int);

public:
    int size() const { return maxSize; }

    T *table() const { return hashTable_; }

    ClosedHashTable(int maxSize, float maxLoadFactor, int (*updateMaxSize)(int)) : maxSize(maxSize), maxLoadFactor(maxLoadFactor), size_(0), updateMaxSize(updateMaxSize)
    {
        hashTable_ = new T[maxSize]{};
        flag = new bool[maxSize]{0};
    }

    void insert(T item, bool rehashing = false)
    {
        if (size_ >= maxSize * maxLoadFactor && !rehashing)
        {
            size_ = 0;
            rehash();
        }
        int steps = 0;
        int index = hash(item->key, steps);
        while (((hashTable_[index] != nullptr && hashTable_[index]->key != item->key) || flag[index] == true))
        {
            steps++;
            index = hash(item->key, steps);
        }
        hashTable_[index] = item;
        flag[index] = true;
        size_++;
    }

    void rehash()
    {
        T *oldHashTable = hashTable_;
        bool *oldFlag = flag;
        int newSize = updateMaxSize(maxSize);
        hashTable_ = new T[newSize]{};
        flag = new bool[newSize]{0};
        for (int i = 0; i < maxSize; i++)
        {
            if (oldHashTable[i] != nullptr)
            {
                insert(oldHashTable[i], true);
            }
        }
        delete[] oldHashTable;
        delete[] oldFlag;
        maxSize = newSize;
    }

    T search(int key)
    {
        int steps = 0;
        int index = hash(key, steps);
        while ((hashTable_[index] == nullptr || hashTable_[index]->key != key) && flag[index] == true)
        {
            steps++;
            index = hash(key, steps);
        }
        if (flag[index] != true)
        {
            return nullptr;
        }
        return hashTable_[index];
    }

    T remove(int key)
    {
        int steps = 0;
        int index = hash(key, steps);
        while ((hashTable_[index] == nullptr || hashTable_[index]->key != key) && flag[index] == true)
        {
            steps++;
            index = hash(key, steps);
        }
        if (flag[index] != true)
        {
            return nullptr;
        }
        T item = hashTable_[index];
        hashTable_[index] = nullptr;
        size_--;
        return item;
    }
};

int updateMaxSize(int maxSize)
{
    return maxSize * 2;
}

int main()
{
    ClosedHashTable<Item *> *hashTable = new ClosedHashTable<Item *>(2, 0.5, &updateMaxSize);
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
        if (hashTable->table()[i] != nullptr)
            cout << hashTable->table()[i]->key << endl;
        else
            cout << "empty" << endl;
    }

    return 0;
}