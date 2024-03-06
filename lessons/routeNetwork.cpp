#include <iostream>
#define INT_MAX 2147483647
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

    void update(int key, T value)
    {
        Node<T> *node = sentinel.next;
        while (node != nullptr && node != &sentinel && node->key != key)
        {
            node = node->next;
        }
        if (node == nullptr || node == &sentinel)
        {
            return;
        }
        node->value = value;
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
    virtual void updateEdge(int from, int to, T weight) = 0;
    virtual T *getAdjacent(int from) = 0;
    virtual int size() = 0;
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

    void updateEdge(int from, int to, T weight)
    {
        adjacencyList[from].update(to, weight);
        if (!directional_)
        {
            adjacencyList[to].update(from, weight);
        }
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
    int *rank, *representative;

    void makeSet()
    {
        int size = graph->size();
        representative = new int[size];
        rank = new int[size];
        for (int i = 0; i < size; i++)
        {
            rank[i] = -1;
            representative[i] = i;
        }
    }

    int find_(int vertex)
    {
        if (representative[vertex] != vertex)
        {
            representative[vertex] = find_(representative[vertex]);
        }

        return representative[vertex];
    }

    void union_(int vertexFrom, int vertexTo)
    {
        int vertexFromRepresentative = find_(vertexFrom);
        int vertexToRepresentative = find_(vertexTo);

        if (vertexFromRepresentative == vertexToRepresentative)
            return;

        if (rank[vertexFromRepresentative] < rank[vertexToRepresentative])
        {
            representative[vertexFromRepresentative] = vertexToRepresentative;
        }
        else if (rank[vertexFromRepresentative] > rank[vertexToRepresentative])
        {
            representative[vertexToRepresentative] = vertexFromRepresentative;
        }
        else
        {
            representative[vertexToRepresentative] = vertexFromRepresentative;
            rank[vertexFromRepresentative] = rank[vertexFromRepresentative] + 1;
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
        delete representative;
    }

    int find(int vertex)
    {
        return find_(vertex);
    }

    void addEdge(int from, int to, T weight)
    {
        graph->addEdge(from, to, weight);
        union_(from, to);
    }

    void updateEdge(int from, int to, T weight)
    {
        graph->updateEdge(from, to, weight);
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

    T primCount()
    {
        struct antecessorStruct
        {
            int antecessor;
            T weight;
        };
        int size = graph->size();

        antecessorStruct **antecessor = new antecessorStruct *[size];
        Heap<Node<T> *> *heap = new Heap<Node<T> *>(size * size * size);

        for (int i = 0; i < size; i++)
        {
            antecessor[i] = new antecessorStruct{-1, INT_MAX};
        }
        antecessor[0]->weight = 0;
        heap->insert(new Node<T>{0, 0, nullptr, nullptr});

        while (heap->size() > 0)
        {
            int node = heap->critical()->key;
            cout << node << endl;

            T *adjacents = graph->getAdjacent(node);
            for (int j = 0; j < size; j++)
            {
                if (adjacents[j])
                {
                    T weight = graph->getEdge(node, j);
                    if (antecessor[j]->weight > weight)
                    {
                        heap->insert(new Node<T>{j, weight, nullptr, nullptr});
                        antecessor[j]->antecessor = node;
                        antecessor[j]->weight = weight;
                    }
                }
            }
            delete[] adjacents;
            cout << endl;
        }
        T count = 0;
        for (int i = 0; i < size; i++)
        {
            if (antecessor[i]->antecessor != -1)
            {
                cout << antecessor[i]->antecessor << " to " << i << " " << antecessor[i]->weight << endl;
                count += antecessor[i]->weight;
            }
        }

        delete[] antecessor;
        delete heap;

        return count;
    }

    T kruskal()
    {
        struct HeapNode
        {
            int from;
            int to;
            T value;
        };

        int totalWeight = 0;
        int size = graph->size();

        Graph<T> *mst = new Graph<T>(size, false);
        Heap<HeapNode *> *heap = new Heap<HeapNode *>(size * size * size);

        for (int i = 0; i < size; i++)
        {
            T *adjacents = graph->getAdjacent(i);

            for (int j = 0; j < size; j++)
            {
                if (adjacents[j])
                {
                    heap->insert(new HeapNode{i, j, adjacents[j]});
                }
            }

            delete[] adjacents;
        }
        while (heap->size() > 0)
        {
            auto value = heap->critical();

            int from = value->from;
            int to = value->to;
            if (mst->find(from) != mst->find(to))
            {
                mst->addEdge(from, to, value->value);
                totalWeight += value->value;
            }

            delete value;
        }
        delete heap;
        delete mst;

        return totalWeight;
    }
};

int main()
{
    int size, edges;
    cin >> size >> edges;
    Graph<int> *graph = new Graph<int>(size, true);
    for (int i = 0; i < edges; i++)
    {
        int from, to, weight;
        cin >> from >> to >> weight;
        if (from != to)
        {
            int prevWeight = graph->getEdge(from, to);
            if (prevWeight)
                graph->updateEdge(from, to, weight);
            else
                graph->addEdge(from, to, weight);
        }
    }
    cout << graph->kruskal() << endl;

    delete graph;
    return 0;
}
