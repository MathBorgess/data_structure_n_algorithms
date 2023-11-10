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
    int size = 0;

public:
    void push(Node *node)
    {
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
        size++;
    }

    Node *pop()
    {
        sentinel.prev->prev->next = &sentinel;
        Node *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        size--;
        return item;
    }

    void reduce()
    {
        Node *node = sentinel.prev;
        int num = node->number - node->prev->number;
        while (node != &sentinel && node->prev != &sentinel && num % 2 == 0)
        {
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
            num = node->number - node->prev->number;
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

    int getSize()
    {
        return size;
    }

    int getTop()
    {
        return sentinel.prev->number;
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
            stack.reduce();
            cin >> numbers;
        } while (numbers != 0);
        int top;
        int size = stack.getSize();
        if (size == 0)
        {
            top = -1;
        }
        else
        {
            top = stack.getTop();
        }
        cout << "Pilha " << i + 1 << ": " << size << " " << top << endl;
    }
    return 0;
}