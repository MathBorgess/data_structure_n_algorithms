#include <iostream>
using namespace std;

struct Node
{
    int number;
    Node *next;
    Node *prev;
};

class MathStack
{
private:
    Node sentinel = {0, &sentinel, &sentinel};

public:
    void push(Node *node)
    {
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
    }

    Node *pop()
    {
        sentinel.prev->prev->next = &sentinel;
        Node *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        return item;
    }

    void reduce()
    {
        Node *node = sentinel.prev;
        while (node != &sentinel && node->prev != &sentinel && ((node->number % 2) == (node->prev->number % 2)))
        {
            int num = node->number - node->prev->number;
            if (num < 0)
            {
                num *= -1;
            }
            pop();
            pop();
            if (num != 0)
            {
                Node *newNode = new Node{num, nullptr, nullptr};
                push(newNode);
            }
            node = sentinel.prev;
        }
    }
    void search()
    {
        Node *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            std::cout << node->number << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }
};

int main()
{
    int rounds;
    cin >> rounds;
    for (int i = 0; i < rounds; i++)
    {
        MathStack stack;
        int numbers;
        cin >> numbers;
        do
        {
            Node *node = new Node{numbers, nullptr, nullptr};
            stack.push(node);
            cout << "search" << endl;
            stack.search();
            cout << "reduce" << endl;
            stack.reduce();
            cout << "search" << endl;
            stack.search();
            cin >> numbers;
        } while (numbers != 0);
        stack.search();
    }
    return 0;
}