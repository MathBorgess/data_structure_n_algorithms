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

    ~LinkedList()
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            Node<T> *temp = node;
            node = node->next;
            delete temp;
        }
    }
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
    virtual ~IGraphStructure() {}
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

    ~GraphMatrix()
    {
        for (int i = 0; i < size_; i++)
        {
            delete[] adjacencyMatrix[i];
        }
        delete[] adjacencyMatrix;
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
    ~GraphList()
    {
        delete[] adjacencyList;
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

    int size() { return size_; }
};

template <typename T>
class Graph
{
private:
    IGraphStructure<T> *graph;
    int *rank, *parent;

    void makeSet()
    {
        int size = graph->size();
        parent = new int[size];
        rank = new int[size];
        for (int i = 0; i < size; i++)
        {
            rank[i] = -1;
            parent[i] = i;
        }
    }

    int find_(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find_(parent[x]);
        }

        return parent[x];
    }

    void union_(int x, int y)
    {
        int xset = find_(x);
        int yset = find_(y);

        if (xset == yset)
            return;

        if (rank[xset] < rank[yset])
        {
            parent[xset] = yset;
        }
        else if (rank[xset] > rank[yset])
        {
            parent[yset] = xset;
        }
        else
        {
            parent[yset] = xset;
            rank[xset] = rank[xset] + 1;
        }
    }

    void dfs(int vertex, bool *visited, int *antecessor)
    {
        visited[vertex] = true;
        T *adjacents = graph->getAdjacent(vertex);
        int size = graph->size();
        for (int i = 0; i < size; i++)
        {
            if (!visited[i] && adjacents[i])
            {
                antecessor[i] = vertex;
                dfs(i, visited, antecessor);
            }
        }
        delete[] adjacents;
    }

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
        makeSet();
    }
    ~Graph()
    {
        delete graph;
        delete rank;
        delete parent;
    }

    void addEdge(int from, int to, T weight)
    {
        graph->addEdge(from, to, weight);
        union_(from, to);
    }

    int find(int vertex)
    {
        return find_(vertex);
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
        queue->push(init, init);
        visited[init] = true;
        while (queue->size() > 0)
        {
            int vertex = queue->shift();
            T *adjacents = graph->getAdjacent(vertex);
            for (int i = 0; i < size; i++)
            {
                if (!visited[i] && adjacents[i])
                {
                    visited[i] = true;
                    queue->push(i, i);
                    if (i == end)
                    {
                        delete queue;
                        delete[] visited;
                        return 1;
                    }
                }
            }
        }
        delete queue;
        delete[] visited;
        return 0;
    }
};

int main()
{
    Graph<int> *graph;
    int quantity, size, removedWalls, consults, wall, from, to;
    cin >> quantity;
    for (int i = 0; i < quantity; i++)
    {
        cin >> size >> removedWalls >> consults;
        graph = new Graph<int>(size * size, false, false);
        for (int j = 0; j < removedWalls; j++)
        {
            cin >> wall;
            int row = 0;
            while (wall >= size * 2 - 1)
            {
                wall -= size * 2 - 1;
                row++;
            }
            if (wall < size - 1)
            {
                from = row * size + wall;
                to = row * size + wall + 1;
            }
            else
            {
                wall -= size - 1;
                from = row * size + wall;
                to = (row + 1) * size + wall;
            }

            graph->addEdge(from, to, 1);
        }
        for (int j = 0; j < consults; j++)
        {
            cin >> from >> to;
            bool expr = (graph->find(from) == graph->find(to));
            cout << i << "." << j << " " << (expr ? 1 : 0) << endl;
        }
        cout << endl;
        delete graph;
    }
    return 0;
}
