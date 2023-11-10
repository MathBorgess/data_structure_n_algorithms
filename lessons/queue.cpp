#include <iostream>

using namespace std;

struct Ship
{
    float height;
    char side;
    int travels;
};

struct Node
{
    float height;
    Node *next;
    Node *prev;
};

class CarShipSideQueue
{
private:
    Node sentinel = {0, &sentinel, &sentinel};
    int size_ = 0;

public:
    void enqueue(Node *node)
    {
        node->prev = &sentinel;
        node->next = sentinel.next;
        sentinel.next->prev = node;
        sentinel.next = node;
        size_++;
    }

    Node *dequeue()
    {
        sentinel.prev->prev->next = &sentinel;
        Node *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        size_--;
        return item;
    }

    void travel(float *shipHeight)
    {
        Node *node = sentinel.prev;
        float countHeight = 0;
        while (node != &sentinel && node->height / 100 + countHeight <= *shipHeight)
        {
            countHeight += node->height / 100;
            node = node->prev;
            delete dequeue();
        }
    }

    void search()
    {
        Node *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            std::cout << node->height << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    int size() const { return size_; }
};

int main()
{
    Ship ship = {0, 'L', 0};
    int rounds;
    cin >> rounds;
    for (int i = 0; i < rounds; i++)
    {
        int cars;
        cin >> ship.height;
        ship.travels = 0;
        ship.side = 'L';
        CarShipSideQueue leftQueue;
        CarShipSideQueue rightQueue;
        cin >> cars;
        for (int j = 0; j < cars; j++)
        {
            float carHeight;
            string carSide;
            cin >> carHeight;
            cin >> carSide;
            if (carSide == "esquerdo")
            {
                Node *node = new Node{carHeight, nullptr, nullptr};
                leftQueue.enqueue(node);
            }
            else if (carSide == "direito")
            {
                Node *node = new Node{carHeight, nullptr, nullptr};
                rightQueue.enqueue(node);
            }
        }
        while (leftQueue.size() > 0 || rightQueue.size() > 0)
        {
            if (ship.side == 'L')
            {
                leftQueue.travel(&ship.height);
                ship.side = 'R';
                ship.travels++;
            }
            else if (ship.side == 'R')
            {
                rightQueue.travel(&ship.height);
                ship.side = 'L';
                ship.travels++;
            }
        }
        cout << "Caso " << i + 1 << ": " << ship.travels << endl;
    }
    return 0;
}