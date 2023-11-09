#include <iostream>

struct Node
{
    int data;
    Node *next;
    Node *prev;
};

class LinkedList
{
private:
    Node sentinel = {0, nullptr, nullptr};

public:
    void enqueue(Node *node)
    {
        node->prev = &sentinel;
        node->next = sentinel.next;
        if (sentinel.next != nullptr)
        {
            sentinel.next->prev = node;
        }
        if (sentinel.prev == nullptr)
        {
            sentinel.prev = node;
        }
        sentinel.next = node;
    }

    Node *dequeue()
    {
        sentinel.prev->prev->next = &sentinel;
        Node *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        return item;
    }

    void search()
    {
        Node *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            std::cout << node->data << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }
};

int main()
{
    LinkedList list;

    // Creating nodes
    Node node1 = {1, nullptr, nullptr};
    Node node2 = {2, nullptr, nullptr};
    Node node3 = {3, nullptr, nullptr};

    // Enqueuing nodes into the linked list
    list.enqueue(&node1);
    list.enqueue(&node2);
    list.enqueue(&node3);

    // Printing the linked list
    std::cout << "Linked List: ";
    list.search();
    list.dequeue();

    return 0;
}
