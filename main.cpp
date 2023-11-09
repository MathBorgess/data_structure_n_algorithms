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
    Node sentinel = {0, &sentinel, &sentinel};

public:
    void enqueue(Node *node)
    {
        node->prev = &sentinel;
        node->next = sentinel.next;
        sentinel.next->prev = node;
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
    list.search();
    list.dequeue();
    list.search();

    return 0;
}
