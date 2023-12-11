#include <iostream>
using namespace std;

struct Item
{
    string key;
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

    int search(string key)
    {
        int searchIndex = 1;
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data->key != key)
        {
            searchIndex++;
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return 0;
        }
        return searchIndex;
    }

    bool remove(string key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data->key != key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return false;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        size_--;
        delete node;
        return true;
    }

    int size() const { return size_; }
};

template <typename T>
class OpenedHashTable
{
private:
    int size_;
    int maxSize;
    LinkedList<T> *hashTable_;
    int hash(string key)
    {
        int keyValue = 0;
        for (int i = 0; i < key.length(); i++)
        {
            keyValue += int(key[i]) * (i + 1);
        }
        return (keyValue * 17) % maxSize;
    }

public:
    OpenedHashTable(int maxSize) : maxSize(maxSize), size_(0)
    {
        hashTable_ = new LinkedList<T>[maxSize] {};
    }

    void insert(T item, bool rehashing = false)
    {
        int index = hash(item->key);
        hashTable_[index].enqueue(item);
        size_++;
    }

    pair<int, int> search(string key)
    {
        int index = hash(key);
        pair<int, int> result = {0, index};
        if (hashTable_[index].size() == 0)
        {
            return result;
        }
        result.first = hashTable_[index].search(key);
        return result;
    }

    int remove(string key)
    {
        int index = hash(key);
        if (hashTable_[index].size() == 0)
        {
            return 0;
        }
        size_--;
        return hashTable_[index].remove(key);
    }
};

int main()
{
    int maxSize;
    int inputsCount;
    cin >> maxSize >> inputsCount;
    OpenedHashTable<Item *> *hashTable = new OpenedHashTable<Item *>(maxSize);
    string command, key;
    for (int i = 0; i < inputsCount; i++)
    {
        cin >> command >> key;
        hashTable->insert(new Item{key});
    }
    cin >> inputsCount;
    for (int i = 0; i < inputsCount; i++)
    {
        cin >> command >> key;
        if (command == "GET")
        {
            pair<int, int> result = hashTable->search(key);
            if (result.first == 0)
            {
                cout << "404 - NOT FOUND" << endl;
            }
            else
            {
                cout << result.second << " " << result.first << endl;
            }
        }
        else if (command == "DELETE")
        {
            bool isThere = hashTable->remove(key);
            if (isThere)
            {
                cout << "DELETADO" << endl;
            }
        }
        else if (command == "POST")
        {
            hashTable->insert(new Item{key});
        }
    }

    return 0;
}