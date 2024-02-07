#include <iostream>

using namespace std;

template <typename T>
class IGraphStructure
{
public:
    virtual void addEdge(int from, int to, T weight) = 0;
    virtual void removeEdge(int from, int to) = 0;
    virtual T getEdge(int from, int to) = 0;
    virtual T *getAdjacent(int from) = 0;
    virtual int size() = 0;
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
struct Node
{
    int data;
    T value;
    Node *next;
    Node *prev;
};

template <typename T>
class LinkedList
{
private:
    Node<T> sentinel = {0, T(), &sentinel, &sentinel};
    int size_ = 0;

public:
    Node<T> *makeNode(int key, T item)
    {
        return new Node<T>{key, item, nullptr, nullptr};
    }

    T pop()
    {
        size_--;
        sentinel.prev->prev->next = &sentinel;
        Node<T> *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        return item->value;
    }

    T remove(int key)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->data != key)
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
        while (node != nullptr && node != &sentinel && node->data != key)
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
        T *list = new T[size_];
        Node<T> *node = sentinel.next;
        for (int i = 0; i < size_; i++)
        {
            list[node->data] = node->value;
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

    int size() { return size_; }
};

template <typename T>
class Graph
{
private:
    IGraphStructure<T> *graph;

public:
    Graph(int size, bool isMatrix, bool isDirectional)
    {
        if (isMatrix)
        {
            graph = new GraphMatrix<T>(size, isDirectional);
        }
        else
        {
            graph = new GraphList<T>(size, isDirectional);
        }
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

    int *deepSearch()
    {
        bool *visited = new bool[graph->size()];
        int *antecessor = new int[graph->size()];
        for (int i = 0; i < graph->size(); i++)
        {
            visited[i] = false;
            antecessor[i] = -1;
        }
        for (int i = 0; i < graph->size(); i++)
        {
            if (!visited[i])
            {
                dfs(i, visited, antecessor);
            }
        }
        return antecessor;
    }

    void dfs(int vertex, bool *visited, int *antecessor)
    {
        visited[vertex] = true;
        T *adjacents = graph->getAdjacent(vertex);
        for (int i = 0; i < graph->size(); i++)
        {
            if (!visited[i] && adjacents[i])
            {
                antecessor[i] = vertex;
                dfs(i, visited, antecessor);
            }
        }
    }

    int *barrowSearch()
    {
        bool *visited = new bool[graph->size()];
        int *antecessor = new int[graph->size()];
        LinkedList<int> *queue = new LinkedList<int>();
        for (int i = 0; i < graph->size(); i++)
        {
            visited[i] = false;
            antecessor[i] = -1;
        }
        for (int i = 0; i < graph->size(); i++)
        {
            if (!visited[i])
            {
                queue->push(i, i);
                visited[i] = true;
                while (queue->size() > 0)
                {
                    int vertex = queue->pop();
                    T *adjacents = graph->getAdjacent(vertex);
                    for (int i = 0; i < graph->size(); i++)
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

int main()
{
    Graph<int> graph(5, true, true);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);
    graph.addEdge(3, 4, 1);
    int *antecessor = graph.barrowSearch();
    for (int i = 0; i < 5; i++)
    {
        cout << antecessor[i] << endl;
    }

    return 0;
}