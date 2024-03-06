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

struct Reachable
{
    int value;
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

    void barrow(LinkedList<T> *queue, bool *visited, Reachable *reachable)
    {
        Node<T> *node = sentinel.next;

        for (int i = 0; i < size_; i++)
        {
            if (!visited[node->key])
            {
                reachable->value = reachable->value + 1;
                visited[node->key] = true;
                queue->push(node->key, node->value);
            }
            node = node->next;
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
    virtual void barrowInList(int vertex, LinkedList<T> *queue, bool *visited, Reachable *reachable) = 0;
    virtual T **matrix() = 0;
    virtual ~IGraphStructure() {}
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

    void barrowInList(int vertex, LinkedList<T> *queue, bool *visited, Reachable *reachable)
    {
        adjacencyList[vertex].barrow(queue, visited, reachable);
    }

    int size() { return size_; }
};

template <typename T>
class Graph
{
private:
    IGraphStructure<T> *graph;
    int *rank, *parent, *componentConex;

    void makeSet()
    {
        int size = graph->size();
        parent = new int[size];
        rank = new int[size];
        componentConex = new int[size];
        for (int i = 0; i < size; i++)
        {
            rank[i] = -1;
            componentConex[i] = 1;
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
            componentConex[yset] += componentConex[xset];
        }
        else if (rank[xset] > rank[yset])
        {
            parent[yset] = xset;
            componentConex[xset] += componentConex[yset];
        }
        else
        {
            parent[yset] = xset;
            componentConex[xset] += componentConex[yset];
            rank[xset] = rank[xset] + 1;
        }
    }

public:
    Graph(int size, bool isDirectional)
    {
        graph = new GraphList<T>(size, isDirectional);
        makeSet();
    }
    ~Graph()
    {
        delete graph;
        delete rank;
        delete parent;
        delete componentConex;
    }

    void addEdge(int from, int to, T weight)
    {
        graph->addEdge(from, to, weight);
        union_(from, to);
    }

    int conexCount(int vertex)
    {
        return componentConex[find_(vertex)];
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

    int barrowSearch(int init = 0)
    {
        int size = graph->size();
        bool *visited = new bool[size]{false};
        Reachable *reachables = new Reachable{0};
        LinkedList<int> *queue = new LinkedList<int>();
        queue->push(init, init);
        visited[init] = true;
        while (queue->size() > 0)
        {
            int vertex = queue->shift();
            graph->barrowInList(vertex, queue, visited, reachables);
        }
        delete queue;
        delete[] visited;
        return reachables->value + 1;
    }
};

int main()
{
    int vertexes, edges;
    cin >> vertexes >> edges;
    Graph<int> *graph = new Graph<int>(vertexes, false);
    for (int i = 0; i < edges; i++)
    {
        int from, to;
        cin >> from >> to;
        graph->addEdge(from - 1, to - 1, 1);
    }
    for (int i = 0; i < vertexes; i++)
    {
        if (i)
        {
            cout << " ";
        }
        cout << graph->conexCount(i);
    }
    cout << endl;

    delete graph;

    return 0;
}
