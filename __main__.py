class Heap:
    def __init__(self, maxsize: int, array: list = []):
        self.maxsize = maxsize
        self.size = 0
        if not list:
            self.heap = [0]*(maxsize)
        else:
            self.size = maxsize
            self.heap = array
            self.buildMinHeap()

    def r_child(self, i: int):
        return 2*(i+1)

    def l_child(self, i: int):
        return 2*(i+1)-1

    def parent(self, i: int):
        return abs((i-1))//2

    def getHeap(self):
        return self.heap

    def getsize(self):
        return self.size

    def swap(self, tpos: int, spos: int):
        self.heap[tpos], self.heap[spos] = self.heap[spos], self.heap[tpos]

    def minHeapify(self, i: int):
        l = self.l_child(i)
        r = self.r_child(i)
        minelem = i
        if l < self.size and self.heap[l][2] < self.heap[minelem][2]:
            minelem = l

        if r < self.size and self.heap[r][2] < self.heap[minelem][2]:
            minelem = r

        if minelem != i:
            self.swap(i, minelem)
            self.minHeapify(minelem)

    def buildMinHeap(self):
        for ind in range(self.maxsize//2-1, -1, -1):
            self.minHeapify(ind)

    def minHeapsort(self, buildheap: bool = False):
        if buildheap:
            self.buildMinHeap()
        temp = self.size
        for ind in range(self.size-1, 0, -1):
            self.swap(ind, 0)
            self.size -= 1
            self.minHeapify(0)
        self.size = temp

    def extractmin(self):
        if self.size < 1:
            return -1
        minelem = self.heap[0]
        self.size -= 1
        self.heap[0] = self.heap[self.size]
        self.heap[self.size] = minelem
        self.minHeapify(0)
        return minelem

    def insert(self, elem, max_min: bool = True):
        if self.size >= self.maxsize:
            return -1

        self.heap[self.size] = elem
        curr = self.size
        self.size += 1
        if max_min:
            while self.heap[curr][2] > self.heap[self.parent(curr)][2]:
                self.swap(curr, self.parent(curr))
                curr = self.parent(curr)
            return curr

        else:
            while self.heap[curr][2] < self.heap[self.parent(curr)][2]:
                self.swap(curr, self.parent(curr))
                curr = self.parent(curr)
            return curr


def find(parent, i):
    if parent[i] != i:
        parent[i] = find(parent, parent[i])
    return parent[i]

# Heap -> [edge: [v1, v2, weight]]


def kruskal(edges: int, parent: list, graph: list):
    cost = 0
    ew = Heap(edges, graph)

    for _ in range(edges):
        u, v, w = ew.extractmin()
        upar = find(parent, u)
        vpar = find(parent, v)
        if upar != vpar:
            parent[vpar] = upar
            cost += w

    return cost


def main():
    ports, lines = input().split()
    ports, lines = int(ports), int(lines)
    network = [[0, 0, 0]]*lines
    parent = [0]*ports
    for i in range(ports):
        parent[i] = i

    for i in range(lines):
        v1, v2, w = input().split()
        network[i] = [int(v1), int(v2), int(w)]

    custo = kruskal(lines, parent, network)
    print(custo)


main()
