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

    T shift()
    {
        sentinel.next->next->prev = &sentinel;
        Node<T> *item = sentinel.next;
        sentinel.next = sentinel.next->next;
        size_--;
        return item->key;
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
            return NULL;
        }
        return node->value;
    }

    T *list()
    {
        T *list = new T[maxSize_ ? maxSize_ : size_]{0};
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_; i++)
        {
            list[node->key] = node->value;
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
    virtual int size() = 0;
    virtual T **matrix() = 0;
};

template <typename T>
class GraphMatrix : public IGraphStructure<T>
{
private:
    int size_;
    bool directional_;
    T **adjacencyMatrix;

    void fillAdjancets()
    {
        for (int i = 0; i < size_; i++)
        {
            adjacencyMatrix[i] = new T[size_];
            for (int j = 0; j < size_; j++)
            {
                adjacencyMatrix[i][j] = T();
            }
        }
    }

public:
    GraphMatrix(int size, bool directional)
    {
        size_ = size;
        directional_ = directional;
        adjacencyMatrix = new T *[size];
        fillAdjancets();
    }

    T **matrix() { return adjacencyMatrix; }

    int size() { return size_; }

    void addEdge(int from, int to, T weight)
    {
        adjacencyMatrix[from][to] = weight;
        if (!directional_)
        {
            adjacencyMatrix[to][from] = weight;
        }
    }

    void removeEdge(int from, int to)
    {
        adjacencyMatrix[from][to] = T();
        if (!directional_)
        {
            adjacencyMatrix[to][from] = T();
        }
    }

    T getEdge(int from, int to)
    {
        return adjacencyMatrix[from][to];
    }

    T *getAdjacent(int from)
    {
        return adjacencyMatrix[from];
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
        graph = new GraphMatrix<T>(size, true);
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

    int *barrowSearch(int init = 0)
    {
        int size = graph->size();
        bool *visited = new bool[size];
        int *antecessor = new int[size];
        LinkedList<int> *queue = new LinkedList<int>();
        for (int i = 0; i < size; i++)
        {
            visited[i] = false;
            antecessor[i] = -1;
        }
        int loopState = 0;
        for (int i = init; loopState < size; i = (i + 1) % size, loopState++)
        {
            if (!visited[i])
            {
                queue->push(i, i);
                visited[i] = true;
                while (queue->size() > 0)
                {
                    int vertex = queue->shift();
                    T *adjacents = graph->getAdjacent(vertex);
                    for (int i = 0; i < size; i++)
                    {
                        if (!visited[i] && adjacents[i])
                        {
                            visited[i] = true;
                            antecessor[i] = vertex;
                            queue->push(i, i);
                        }
                    }
                }
            }
        }
        return antecessor;
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

                int k = i;
                int l = j - 1;
                if (l >= 0)
                {
                    graph.addEdge(flat(col, k, l), flat(col, i, j), 1);
                }
                l = j + 1;
                if (l < col)
                {
                    graph.addEdge(flat(col, k, l), flat(col, i, j), 1);
                }
                k = i - 1;
                l = j;
                if (k >= 0)
                {
                    graph.addEdge(flat(col, k, l), flat(col, i, j), 1);
                }
                k = i + 1;
                if (k < row)
                {
                    graph.addEdge(flat(col, k, l), flat(col, i, j), 1);
                }
            }
        }
    }

    int *antecessor = graph.barrowSearch(initVertex);
    int pathSize = 0;
    int vertex = endVertex;
    while (vertex != -1 && vertex != initVertex)
    {
        vertex = antecessor[vertex];
        pathSize++;
    }
    if (vertex == initVertex)
    {
        cout << pathSize << endl;
    }
    else
    {
        cout << "Labirinto Impossivel" << endl;
    }

    return 0;
};
