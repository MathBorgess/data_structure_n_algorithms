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
            return 0;
        }
        return node->value;
    }

    T *list()
    {
        T *list = new T[maxSize_ ? maxSize_ : size_]{0};
        Node<T> *node = sentinel.next;
        for (int i = 0; (i < size_ && node != &sentinel); i++)
        {
            list[i] = node->key;
            node = node->next;
        }
        return list;
    }

    void print()
    {
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_ && node != &sentinel; i++)
        {
            cout << node->key << " ";
            node = node->next;
        }
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

    void enqueue(int key, T weight)
    {
        Node<T> *node = makeNode(key, weight);
        node->prev = &sentinel;
        node->next = sentinel.next;
        sentinel.next->prev = node;
        sentinel.next = node;
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
    virtual LinkedList<T> getAdjacentList(int from) = 0;
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
        for (int i = 0; i < size; i++)
        {
            adjacencyList[i].updateMaxSize(size);
        }
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
        adjacencyList[from].enqueue(to, weight);
        if (!directional_)
        {
            adjacencyList[to].enqueue(from, weight);
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
    LinkedList<T> getAdjacentList(int from)
    {
        return adjacencyList[from];
    }

    int size() { return size_; }
};

template <typename T>
class Graph
{
private:
    IGraphStructure<T> *graph;

    void dfs(int vertex, bool *visited)
    {
        visited[vertex] = true;
        T *adjacents = graph->getAdjacent(vertex);
        int size = graph->size();
        for (int i = 0; i < size; i++)
        {
            if (adjacents[i] >= 0 && !visited[adjacents[i]])
            {
                cout << adjacents[i] << " ";
                dfs(adjacents[i], visited);
            }
        }
    }

public:
    Graph(int size)
    {
        graph = new GraphList<T>(size, false);
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

    LinkedList<T> getAdjacentList(int from)
    {
        return graph->getAdjacentList(from);
    }

    void deepSearch()
    {
        int size = graph->size();
        bool *visited = new bool[size]{false};
        for (int i = 0; i < size; i++)
        {
            if (!visited[i])
            {
                dfs(i, visited);
            }
        }
    }
};

int main()
{
    int size, from, to;
    int keepGoing = 1;
    cin >> size;
    Graph<int> graph(size);
    LinkedList<int> adjacents;
    while (keepGoing)
    {
        cin >> from >> to >> keepGoing;
        graph.addEdge(from, to, 1);
    }
    for (int i = 0; i < size; i++)
    {
        adjacents = graph.getAdjacentList(i);

        cout << i << ": ";

        if (adjacents.size())
        {
            adjacents.print();
        }
        else
        {
            cout << "Lista Vazia";
        }
        cout << endl;
    }
    cout << endl;
    cout << "0 ";
    graph.deepSearch();

    return 0;
};
