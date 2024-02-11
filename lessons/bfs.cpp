#include <iostream>

using namespace std;

template <typename T>
struct Node
{
    int key;
    T value;
    Node *next;
    Node *prev;
};

template <typename T>
class LinkedList
{
private:
    Node<T> sentinel = {0, T(), nullptr, nullptr};
    int size_ = 0;
    int maxSize_;

public:
    LinkedList()
    {
        sentinel.next = &sentinel;
        sentinel.prev = &sentinel;
    }

    void updateMaxSize(int maxSize)
    {
        maxSize_ = maxSize;
    }

    Node<T> *makeNode(int key, T item)
    {
        return new Node<T>{key, item, nullptr, nullptr};
    }

    pair<T, int> shift()
    {
        sentinel.next->next->prev = &sentinel;
        Node<T> *item = sentinel.next;
        sentinel.next = sentinel.next->next;
        size_--;
        return make_pair(item->key, item->value);
    }

    T remove(int key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->key != key)
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
        return node->value;
    }

    T search(int key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->key != key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return 0;
        }
        return node->value;
    }

    void enqueueList(LinkedList<T> *list, bool *visited, T weight)
    {
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_; i++)
        {
            if (!visited[node->key])
            {
                list->push(node->key, weight);
                visited[node->key] = true;
            }
            node = node->next;
        }
    }

    int *list()
    {
        T *list = new T[size_ + 1]{-1};
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_; i++)
        {
            list[i] = node->key;
            node = node->next;
        }
        return list;
    }

    void push(int key, T weight)
    {
        Node<T> *node = makeNode(key, weight);
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
        size_++;
    }

    int size() const { return size_; }
};

template <typename T>
class IGraphStructure
{
public:
    virtual void addEdge(int from, int to, T weight) = 0;
    virtual void removeEdge(int from, int to) = 0;
    virtual T getEdge(int from, int to) = 0;
    virtual T *getAdjacent(int from) = 0;
    virtual void enqueueAdjancents(int from, T weight, LinkedList<T> *queue, bool *visited) = 0;
    virtual int size() = 0;
    virtual T **matrix() = 0;
};

template <typename T>
class GraphList : public IGraphStructure<T>
{
private:
    int size_;
    bool directional_;
    LinkedList<T> *adjacencyList;

public:
    GraphList(int size, bool directional)
    {
        size_ = size;
        directional_ = directional;
        adjacencyList = new LinkedList<T>[size];
    }

    T **matrix()
    {
        T **matrix = new T *[size_];
        for (int i = 0; i < size_; i++)
        {
            matrix[i] = adjacencyList[i].list();
        }
        return matrix;
    }

    void addEdge(int from, int to, T weight)
    {
        adjacencyList[from].push(to, weight);
        if (!directional_)
        {
            adjacencyList[to].push(from, weight);
        }
    }

    void removeEdge(int from, int to)
    {
        adjacencyList[from].remove(to);
        if (!directional_)
        {
            adjacencyList[to].remove(from);
        }
    }

    T getEdge(int from, int to)
    {
        return adjacencyList[from].search(to);
    }

    T *getAdjacent(int from)
    {
        return adjacencyList[from].list();
    }

    void enqueueAdjancents(int from, T weight, LinkedList<T> *queue, bool *visited)
    {
        adjacencyList[from].enqueueList(queue, visited, weight);
    }

    int size()
    {
        return size_;
    }
};

template <typename T>
class Graph
{
private:
    IGraphStructure<T> *graph;

public:
    Graph(int size)
    {
        graph = new GraphList<T>(size, true);
    }

    void addEdge(int from, int to, T weight)
    {
        graph->addEdge(from, to, weight);
    }

    void removeEdge(int from, int to)
    {
        graph->removeEdge(from, to);
    }

    T getEdge(int from, int to)
    {
        return graph->getEdge(from, to);
    }

    T *getAdjacent(int from)
    {
        return graph->getAdjacent(from);
    }

    int barrowSearch(int init = 0, int end = -1)
    {
        int size = graph->size();
        bool *visited = new bool[size]{false};
        LinkedList<int> *queue = new LinkedList<int>();
        queue->push(init, 0);
        visited[init] = true;
        pair<int, int> node = make_pair(-2, -1);
        while (queue->size() > 0 && node.first != end)
        {
            node = queue->shift();
            graph->enqueueAdjancents(node.first, node.second + 1, queue, visited);
        }
        if (node.first != end)
        {
            return -1;
        }
        return node.second;
    }
};

int flat(int colTotal, int col, int row)
{
    return col * colTotal + row;
}

int main()
{
    int row, col, value, initVertex, endVertex;
    cin >> row >> col;
    Graph<int> graph(row * col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cin >> value;
            if (value != 1)
            {
                if (value == 2)
                {
                    initVertex = flat(col, i, j);
                }
                else if (value == 3)
                {
                    endVertex = flat(col, i, j);
                }

                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, 1, -1};
                for (int k = 0; k < 4; k++)
                {
                    int X = j + dx[k];
                    int Y = i + dy[k];

                    if (X > -1 && X < col && Y > -1 && Y < row)
                    {
                        graph.addEdge(flat(col, Y, X), flat(col, i, j), 1);
                    }
                }
            }
        }
    }

    int pathLen = graph.barrowSearch(initVertex, endVertex);
    if (pathLen != -1)
    {
        cout << pathLen << endl;
    }
    else
    {
        cout << "Labirinto Impossivel" << endl;
    }

    return 0;
};
