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
class Heap
{
private:
    int size_;
    int maxSize;
    T *heap_;

public:
    Heap(int maxSize) : maxSize(maxSize), size_(-1)
    {
        heap_ = new T[maxSize];
    }

    T critical()
    {
        T temp = heap_[0];
        heap_[0] = heap_[size_];
        heap_[size_] = temp;
        size_--;
        minHeapify();
        return temp;
    }

    int parent(int index)
    {
        return (index - index % 2) / 2;
    }

    void minHeapify(int index = 0)
    {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int smallest = index;
        if (leftChild <= size_ && heap_[leftChild]->value < heap_[index]->value)
        {
            smallest = leftChild;
        }
        if (rightChild <= size_ && heap_[rightChild]->value < heap_[smallest]->value)
        {
            smallest = rightChild;
        }
        if (smallest != index)
        {
            T temp = heap_[index];
            heap_[index] = heap_[smallest];
            heap_[smallest] = temp;
            minHeapify(smallest);
        }
    }

    void insert(T item)
    {
        size_++;
        heap_[size_] = item;
        int index = size_;
        while (index > 0 && heap_[parent(index)]->value > heap_[index]->value)
        {
            T temp = heap_[index];
            heap_[index] = heap_[parent(index)];
            heap_[parent(index)] = temp;
            index = parent(index);
        }
    }

    void remove(int index)
    {
        T temp = heap_[index];
        heap_[index] = heap_[size_];
        heap_[size_] = temp;
        size_--;
        minHeapify(index);
    }

    void buildMinHeap()
    {
        int middle = parent(maxSize - 1);
        for (int i = middle; i >= 0; i--)
        {
            minHeapify(i);
        }
    }

    T search(int key)
    {
        for (int i = 0; i < maxSize; i++)
        {
            if (heap_[i]->key == key)
            {
                return heap_[i];
            }
        }
        return NULL;
    }

    int size() const { return size_ + 1; }

    T *heap() const { return heap_; }

    ~Heap()
    {
        delete[] heap_;
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
    bool isDisjointSet;

    void makeSet()
    {
        int size = graph->size();
        parent = new int[size];
        rank = new int[size];
        memset(rank, -1, size);
        for (int i = 0; i < size; i++)
        {
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

    void disunion_(int vertex)
    {
        parent[vertex] = vertex;
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

    void relax(int startVertex, int endVertex, T *distance, int *antecessor, Heap<Node<T> *> *heap)
    {
        T weight = graph->getEdge(startVertex, endVertex);
        if (distance[endVertex] > distance[startVertex] + weight)
        {
            distance[endVertex] = distance[startVertex] + weight;
            heap->insert(new Node<T>{endVertex, distance[endVertex], nullptr, nullptr});
            antecessor[endVertex] = startVertex;
        }
    }

public:
    Graph(int size, bool isMatrix, bool isDirectional, bool disjointSet = false)
    {
        if (isMatrix)
        {
            graph = new GraphMatrix<T>(size, isDirectional);
        }
        else
        {
            graph = new GraphList<T>(size, isDirectional);
        }
        isDisjointSet = disjointSet;
        if (isDisjointSet)
        {
            makeSet();
        }
    }

    ~Graph()
    {
        delete graph;
        if (isDisjointSet)
        {
            delete rank;
            delete parent;
        }
    }

    void addEdge(int from, int to, T weight)
    {
        graph->addEdge(from, to, weight);
        if (isDisjointSet)
        {
            union_(from, to);
        }
    }

    void removeEdge(int from, int to)
    {
        graph->removeEdge(from, to);
        if (isDisjointSet)
        {
            disunion_(from, to);
        }
    }

    T getEdge(int from, int to)
    {
        return graph->getEdge(from, to);
    }

    T *getAdjacent(int from)
    {
        return graph->getAdjacent(from);
    }

    int find(int vertex)
    {
        return find_(vertex);
    }

    int *deepSearch(int init = 0)
    {
        int size = graph->size();
        bool *visited = new bool[size];
        int *antecessor = new int[size];
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
                dfs(i, visited, antecessor);
            }
        }
        delete[] visited;
        return antecessor;
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
        delete queue;
        delete[] visited;
        return antecessor;
    }

    int *dijkstra(int init = 0)
    {
        int size = graph->size();
        int *antecessor = new int[size];
        T *distance = new T[size];
        bool *visited = new bool[size];
        Heap<Node<T> *> *heap = new Heap<Node<T> *>(size * size * size);
        int loopState = 0;
        for (int i = init; loopState < size; i = (i + 1) % size, loopState++)
        {
            antecessor[i] = -1;
            visited[i] = false;
            distance[i] = INT_MAX;
        }
        distance[init] = 0;
        heap->insert(new Node<T>{init, 0, nullptr, nullptr});
        while (heap->size() > 0)
        {
            int node = heap->critical()->key;

            visited[node] = true;
            T *adjacents = graph->getAdjacent(node);
            for (int j = 0; j < size; j++)
            {
                if (adjacents[j])
                {
                    relax(node, j, distance, antecessor, heap);
                }
            }
        }
        delete heap;
        delete[] visited;
        delete[] distance;

        return antecessor;
    }

    Graph<T> *prim(bool isMatrix = true, bool isDirectional = false)
    {
        struct heapNode
        {
            int startVertex;
            int endVertex;
            T value;
        };

        int size = graph->size();
        int visitedCount = 0;
        int vertex = 0;

        Graph<T> *mst = new Graph<T>(size, isMatrix, isDirectional, isDisjointSet);
        bool *visited = new bool[size]{false};
        heapNode *node;
        Heap<heapNode *> *heap = new Heap<heapNode *>(size * size);

        do
        {
            visited[vertex] = true;
            visitedCount++;
            T *adjacents = graph->getAdjacent(vertex);
            for (int i = 0; i < size; i++)
            {
                if (adjacents[i] && !visited[i])
                {
                    heap->insert(new heapNode{vertex, i, adjacents[i]});
                }
            }
            bool flag = true;
            do
            {
                node = heap->critical();
                if (!visited[node->endVertex])
                {
                    mst->addEdge(node->startVertex, node->endVertex, node->value);
                    vertex = node->endVertex;
                    flag = false;
                }
            } while (heap->size() > 0 && flag);
        } while (visitedCount < size);

        delete heap;
        delete[] visited;

        return mst;
    }
};

int main()
{
    int size = 7;
    Graph<int> graph(size, false, false);
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 6, 2);
    graph.addEdge(6, 5, 2);
    graph.addEdge(6, 1, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(3, 5, 2);
    graph.addEdge(2, 5, 5);
    graph.addEdge(2, 4, 1);
    graph.addEdge(3, 4, 1);
    graph.addEdge(0, 5, 10);

    int *antecessor = graph.barrowSearch(1);
    cout << "Barrow Search: ";
    for (int i = 0; i < size; i++)
    {
        cout << antecessor[i] << " ";
    }
    delete[] antecessor;
    delete graph;
    return 0;
}
