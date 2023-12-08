#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node *next;
    Node *prev;
};

template <typename T>
class LinkedList
{
private:
    Node<T> sentinel = {T(), &sentinel, &sentinel};
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

    T remove(T item)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data.key != item.key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return T();
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

    void push(T item)
    {
        Node<T> *node = makeNode(item);
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
        size_++;
    }

    T pop()
    {
        size_--;
        sentinel.prev->prev->next = &sentinel;
        Node<T> *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        return item->data;
    }

    T shift()
    {
        sentinel.next->next->prev = &sentinel;
        Node<T> *item = sentinel.next;
        sentinel.next = sentinel.next->next;
        size_--;
        return item->data;
    }

    T remove(T item)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data != item)
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

    int size() const { return size_; }
};
