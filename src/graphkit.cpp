#include "algolib/graphkit.hpp"

#include <cstring>
#include <sstream>
#include <limits>

namespace algolib {

// Constant representing infinity for shortest paths
constexpr int INF = 1e9;

// ==================================================
// CUSTOM DISJOINT SET UNION (FOR KRUSKAL'S MST)
// ==================================================
class DisjointSet {
private:
    int* parent;
    int* rank;
    int count;

public:
    explicit DisjointSet(int n) : count(n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    ~DisjointSet() {
        delete[] parent;
        delete[] rank;
    }

    int find(int i) {
        if (parent[i] != i)
            parent[i] = find(parent[i]); // Path compression
        return parent[i];
    }

    bool unite(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI == rootJ)
            return false;

        // Union by rank
        if (rank[rootI] < rank[rootJ]) {
            parent[rootI] = rootJ;
        } else if (rank[rootI] > rank[rootJ]) {
            parent[rootJ] = rootI;
        } else {
            parent[rootJ] = rootI;
            ++rank[rootI];
        }
        return true;
    }
};


// ==================================================
// CUSTOM MIN-HEAP FOR DIJKSTRA'S ALGORITHM
// ==================================================
struct DijkstraNode {
    int vertex;
    int dist;
};

class DijkstraHeap {
private:
    DijkstraNode* heap;
    int len;
    int cap;

    void siftUp(int index) {
        while (index > 0) {
            int p = (index - 1) / 2;
            if (heap[p].dist <= heap[index].dist)
                break;
            DijkstraNode temp = heap[p];
            heap[p] = heap[index];
            heap[index] = temp;
            index = p;
        }
    }

    void siftDown(int index) {
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < len && heap[left].dist < heap[smallest].dist)
                smallest = left;
            if (right < len && heap[right].dist < heap[smallest].dist)
                smallest = right;

            if (smallest == index)
                break;

            DijkstraNode temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            index = smallest;
        }
    }

public:
    explicit DijkstraHeap(int capacity = 32)
        : len(0), cap(capacity) {
        heap = new DijkstraNode[cap];
    }

    ~DijkstraHeap() {
        delete[] heap;
    }

    bool empty() const { return len == 0; }

    void push(int vertex, int dist) {
        if (len == cap) {
            int newCap = cap * 2;
            DijkstraNode* temp = new DijkstraNode[newCap];
            for (int i = 0; i < len; ++i)
                temp[i] = heap[i];
            delete[] heap;
            heap = temp;
            cap = newCap;
        }
        heap[len] = {vertex, dist};
        siftUp(len);
        ++len;
    }

    DijkstraNode pop() {
        DijkstraNode top = heap[0];
        --len;
        if (len > 0) {
            heap[0] = heap[len];
            siftDown(0);
        }
        return top;
    }
};


// ==================================================
// GRAPH CLASS IMPLEMENTATION
// ==================================================

Graph::Graph(bool isDirected)
    : vertexCount(0), vertexCapacity(8), directed(isDirected) {
    vertexLabels = new char*[vertexCapacity];
    adjList = new EdgeNode*[vertexCapacity];
    for (int i = 0; i < vertexCapacity; ++i) {
        vertexLabels[i] = nullptr;
        adjList[i] = nullptr;
    }
}

Graph::~Graph() {
    clear();
    delete[] vertexLabels;
    delete[] adjList;
}

void Graph::clear() {
    for (int i = 0; i < vertexCount; ++i) {
        delete[] vertexLabels[i];
        vertexLabels[i] = nullptr;

        EdgeNode* cur = adjList[i];
        while (cur) {
            EdgeNode* next = cur->next;
            delete cur;
            cur = next;
        }
        adjList[i] = nullptr;
    }
    vertexCount = 0;
}

void Graph::growVertices() {
    int newCap = vertexCapacity * 2;
    char** newLabels = new char*[newCap];
    EdgeNode** newAdj = new EdgeNode*[newCap];

    for (int i = 0; i < vertexCount; ++i) {
        newLabels[i] = vertexLabels[i];
        newAdj[i] = adjList[i];
    }
    for (int i = vertexCount; i < newCap; ++i) {
        newLabels[i] = nullptr;
        newAdj[i] = nullptr;
    }

    delete[] vertexLabels;
    delete[] adjList;
    vertexLabels = newLabels;
    adjList = newAdj;
    vertexCapacity = newCap;
}

int Graph::addVertex(const std::string& label) {
    int existing = getVertexIndex(label);
    if (existing != -1)
        return existing;

    if (vertexCount == vertexCapacity)
        growVertices();

    char* str = new char[label.length() + 1];
    std::strcpy(str, label.c_str());

    vertexLabels[vertexCount] = str;
    adjList[vertexCount] = nullptr;
    int newIndex = vertexCount;
    ++vertexCount;
    return newIndex;
}

int Graph::getVertexIndex(const std::string& label) const {
    for (int i = 0; i < vertexCount; ++i) {
        if (vertexLabels[i] && label == vertexLabels[i])
            return i;
    }
    return -1;
}

std::string Graph::getVertexLabel(int index) const {
    if (index >= 0 && index < vertexCount && vertexLabels[index])
        return std::string(vertexLabels[index]);
    return "";
}

bool Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount)
        return false;

    // Check if edge already exists, update weight if so
    EdgeNode* cur = adjList[u];
    while (cur) {
        if (cur->to == v) {
            cur->weight = weight;
            if (!directed) {
                // Update opposite direction as well
                EdgeNode* cur2 = adjList[v];
                while (cur2) {
                    if (cur2->to == u) {
                        cur2->weight = weight;
                        break;
                    }
                    cur2 = cur2->next;
                }
            }
            return true;
        }
        cur = cur->next;
    }

    // Add forward edge
    adjList[u] = new EdgeNode(v, weight, adjList[u]);

    // Add backward edge if undirected
    if (!directed && u != v) {
        adjList[v] = new EdgeNode(u, weight, adjList[v]);
    }

    return true;
}

bool Graph::addEdge(const std::string& fromLabel, const std::string& toLabel, int weight) {
    int u = addVertex(fromLabel);
    int v = addVertex(toLabel);
    return addEdge(u, v, weight);
}

bool Graph::removeEdge(int u, int v) {
    if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount)
        return false;

    bool removed = false;
    EdgeNode* cur = adjList[u];
    EdgeNode* prev = nullptr;

    while (cur) {
        if (cur->to == v) {
            if (prev)
                prev->next = cur->next;
            else
                adjList[u] = cur->next;
            delete cur;
            removed = true;
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    if (!directed && u != v) {
        cur = adjList[v];
        prev = nullptr;
        while (cur) {
            if (cur->to == u) {
                if (prev)
                    prev->next = cur->next;
                else
                    adjList[v] = cur->next;
                delete cur;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    return removed;
}

int Graph::getDegree(int u) const {
    if (u < 0 || u >= vertexCount)
        return 0;
    int deg = 0;
    EdgeNode* cur = adjList[u];
    while (cur) {
        ++deg;
        cur = cur->next;
    }
    return deg;
}

bool Graph::hasEdge(int u, int v) const {
    if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount)
        return false;
    EdgeNode* cur = adjList[u];
    while (cur) {
        if (cur->to == v)
            return true;
        cur = cur->next;
    }
    return false;
}

int Graph::getEdgeWeight(int u, int v) const {
    if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount)
        return -1;
    EdgeNode* cur = adjList[u];
    while (cur) {
        if (cur->to == v)
            return cur->weight;
        cur = cur->next;
    }
    return -1;
}

int Graph::getTotalEdgeCount() const {
    int total = 0;
    for (int i = 0; i < vertexCount; ++i) {
        EdgeNode* cur = adjList[i];
        while (cur) {
            if (directed || i <= cur->to)
                ++total;
            cur = cur->next;
        }
    }
    return total;
}

int Graph::getAllEdges(GraphEdge* output, int maxEdges) const {
    int count = 0;
    for (int i = 0; i < vertexCount; ++i) {
        EdgeNode* cur = adjList[i];
        while (cur) {
            if (directed || i <= cur->to) {
                if (count < maxEdges) {
                    output[count] = {i, cur->to, cur->weight};
                }
                ++count;
            }
            cur = cur->next;
        }
    }
    return count;
}


// ==================================================
// CUSTOM QUICKSORT FOR GRAPH EDGES
// ==================================================

int Graph::partitionEdges(GraphEdge* edges, int low, int high) {
    int pivot = edges[high].weight;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (edges[j].weight <= pivot) {
            ++i;
            GraphEdge temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
        }
    }
    GraphEdge temp = edges[i + 1];
    edges[i + 1] = edges[high];
    edges[high] = temp;
    return i + 1;
}

void Graph::customSortEdges(GraphEdge* edges, int low, int high) {
    if (low < high) {
        int pi = partitionEdges(edges, low, high);
        customSortEdges(edges, low, pi - 1);
        customSortEdges(edges, pi + 1, high);
    }
}


// ==================================================
// BREADTH-FIRST SEARCH (BFS)
// ==================================================

BFSResult Graph::bfs(int startVertex, bool visitAllComponents) const {
    BFSResult result;
    result.visitedOrder = nullptr;
    result.visitedCount = 0;
    result.componentCount = 0;

    if (vertexCount == 0) {
        result.summary = "Graph is empty.";
        return result;
    }

    if (startVertex < 0 || startVertex >= vertexCount)
        startVertex = 0;

    bool* visited = new bool[vertexCount];
    for (int i = 0; i < vertexCount; ++i)
        visited[i] = false;

    int* order = new int[vertexCount];
    int orderIdx = 0;

    // Custom FIFO queue for BFS
    int* queue = new int[vertexCount];
    int qFront = 0;
    int qRear = 0;

    int components = 0;
    std::ostringstream ss;
    ss << "=== BREADTH FIRST SEARCH (BFS) ===\n";
    ss << "Starting Vertex: " << getVertexLabel(startVertex) << "\n\n";

    // Helper lambda to run BFS from a root
    auto runBFSComponent = [&](int root) {
        ++components;
        ss << "Component " << components << ": ";

        visited[root] = true;
        queue[qRear++] = root;

        while (qFront < qRear) {
            int u = queue[qFront++];
            order[orderIdx++] = u;
            ss << getVertexLabel(u) << " ";

            // Traverse adjacent neighbors
            EdgeNode* cur = adjList[u];
            while (cur) {
                int v = cur->to;
                if (!visited[v]) {
                    visited[v] = true;
                    queue[qRear++] = v;
                }
                cur = cur->next;
            }
        }
        ss << "\n";
    };

    // 1. Traverse starting component
    runBFSComponent(startVertex);

    // 2. Traverse any remaining disconnected components
    if (visitAllComponents) {
        for (int i = 0; i < vertexCount; ++i) {
            if (!visited[i]) {
                runBFSComponent(i);
            }
        }
    }

    ss << "\nTotal Visited Vertices: " << orderIdx << " / " << vertexCount << "\n";
    ss << "Total Connected Components: " << components << "\n";
    if (components > 1) {
        ss << "NOTE: Graph is disconnected with " << components << " separate components.\n";
    } else {
        ss << "STATUS: Graph component is fully connected.\n";
    }

    delete[] visited;
    delete[] queue;

    result.visitedOrder = order;
    result.visitedCount = orderIdx;
    result.componentCount = components;
    result.summary = ss.str();
    return result;
}


// ==================================================
// DEPTH-FIRST SEARCH (DFS)
// ==================================================

DFSResult Graph::dfs(int startVertex, bool visitAllComponents) const {
    DFSResult result;
    result.visitedOrder = nullptr;
    result.visitedCount = 0;
    result.componentCount = 0;

    if (vertexCount == 0) {
        result.summary = "Graph is empty.";
        return result;
    }

    if (startVertex < 0 || startVertex >= vertexCount)
        startVertex = 0;

    bool* visited = new bool[vertexCount];
    for (int i = 0; i < vertexCount; ++i)
        visited[i] = false;

    int* order = new int[vertexCount];
    int orderIdx = 0;

    int components = 0;
    std::ostringstream ss;
    ss << "=== DEPTH FIRST SEARCH (DFS) ===\n";
    ss << "Starting Vertex: " << getVertexLabel(startVertex) << "\n\n";

    // Custom LIFO Stack for iterative DFS
    int* stack = new int[vertexCount * 4];
    int top = 0;

    auto runDFSComponent = [&](int root) {
        ++components;
        ss << "Component " << components << ": ";

        stack[top++] = root;

        while (top > 0) {
            int u = stack[--top];

            if (!visited[u]) {
                visited[u] = true;
                order[orderIdx++] = u;
                ss << getVertexLabel(u) << " ";

                // Push neighbors onto stack (push in reverse order to visit in forward order)
                EdgeNode* cur = adjList[u];
                while (cur) {
                    if (!visited[cur->to]) {
                        stack[top++] = cur->to;
                    }
                    cur = cur->next;
                }
            }
        }
        ss << "\n";
    };

    // 1. Traverse starting component
    runDFSComponent(startVertex);

    // 2. Traverse any remaining disconnected components
    if (visitAllComponents) {
        for (int i = 0; i < vertexCount; ++i) {
            if (!visited[i]) {
                runDFSComponent(i);
            }
        }
    }

    ss << "\nTotal Visited Vertices: " << orderIdx << " / " << vertexCount << "\n";
    ss << "Total Connected Components: " << components << "\n";
    if (components > 1) {
        ss << "NOTE: Graph is disconnected with " << components << " separate components.\n";
    } else {
        ss << "STATUS: Graph component is fully connected.\n";
    }

    delete[] visited;
    delete[] stack;

    result.visitedOrder = order;
    result.visitedCount = orderIdx;
    result.componentCount = components;
    result.summary = ss.str();
    return result;
}


// ==================================================
// DIJKSTRA'S SHORTEST PATH
// ==================================================

DijkstraResult Graph::dijkstra(int startVertex, int targetVertex) const {
    DijkstraResult result;
    result.distances = nullptr;
    result.parent = nullptr;
    result.path = nullptr;
    result.vertexCount = vertexCount;
    result.startVertex = startVertex;
    result.targetVertex = targetVertex;
    result.pathLength = 0;
    result.pathExists = false;

    if (vertexCount == 0 || startVertex < 0 || startVertex >= vertexCount) {
        result.summary = "Invalid start vertex or empty graph.";
        return result;
    }

    int* dist = new int[vertexCount];
    int* prev = new int[vertexCount];
    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }

    dist[startVertex] = 0;

    DijkstraHeap pq(vertexCount * 2);
    pq.push(startVertex, 0);

    while (!pq.empty()) {
        DijkstraNode top = pq.pop();
        int u = top.vertex;

        if (visited[u])
            continue;
        visited[u] = true;

        if (targetVertex != -1 && u == targetVertex)
            break; // Found destination

        EdgeNode* edge = adjList[u];
        while (edge) {
            int v = edge->to;
            int weight = edge->weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v, dist[v]);
            }
            edge = edge->next;
        }
    }

    std::ostringstream ss;
    ss << "=== DIJKSTRA'S SHORTEST PATH ===\n";
    ss << "Source Vertex: " << getVertexLabel(startVertex) << "\n\n";

    if (targetVertex >= 0 && targetVertex < vertexCount) {
        ss << "Target Vertex: " << getVertexLabel(targetVertex) << "\n";
        if (dist[targetVertex] >= INF) {
            ss << "STATUS: Target is UNREACHABLE (Disconnected component).\n";
            result.pathExists = false;
        } else {
            ss << "Shortest Distance: " << dist[targetVertex] << "\n";
            ss << "Shortest Path: ";

            // Reconstruct path
            int* pathArr = new int[vertexCount];
            int pathLen = 0;
            int curr = targetVertex;
            while (curr != -1) {
                pathArr[pathLen++] = curr;
                curr = prev[curr];
            }

            // Reverse path to be source -> target
            for (int i = 0; i < pathLen / 2; ++i) {
                int temp = pathArr[i];
                pathArr[i] = pathArr[pathLen - 1 - i];
                pathArr[pathLen - 1 - i] = temp;
            }

            for (int i = 0; i < pathLen; ++i) {
                ss << getVertexLabel(pathArr[i]);
                if (i + 1 < pathLen)
                    ss << " ➔ ";
            }
            ss << "\n";

            result.path = pathArr;
            result.pathLength = pathLen;
            result.pathExists = true;
        }
    } else {
        // Table of all shortest paths from source
        ss << "Shortest Paths from " << getVertexLabel(startVertex) << " to all reachable vertices:\n";
        ss << std::string(50, '-') << "\n";
        ss << "Target Vertex       Distance       Path\n";
        ss << std::string(50, '-') << "\n";

        for (int i = 0; i < vertexCount; ++i) {
            ss << getVertexLabel(i);
            int pad = 20 - static_cast<int>(getVertexLabel(i).length());
            for (int k = 0; k < pad; ++k) ss << " ";

            if (dist[i] >= INF) {
                ss << "INF (Unreachable)\n";
            } else {
                ss << dist[i];
                int distPad = 15 - static_cast<int>(std::to_string(dist[i]).length());
                for (int k = 0; k < distPad; ++k) ss << " ";

                // Print path
                int* tempP = new int[vertexCount];
                int pLen = 0;
                int c = i;
                while (c != -1) {
                    tempP[pLen++] = c;
                    c = prev[c];
                }
                for (int p = pLen - 1; p >= 0; --p) {
                    ss << getVertexLabel(tempP[p]);
                    if (p > 0) ss << "->";
                }
                ss << "\n";
                delete[] tempP;
            }
        }
    }

    delete[] visited;

    result.distances = dist;
    result.parent = prev;
    result.summary = ss.str();
    return result;
}


// ==================================================
// KRUSKAL'S MINIMUM SPANNING TREE (MST)
// ==================================================

KruskalResult Graph::kruskalMST() const {
    KruskalResult result;
    result.mstEdges = nullptr;
    result.mstEdgeCount = 0;
    result.totalWeight = 0;
    result.isConnected = false;
    result.componentCount = 0;

    if (vertexCount == 0) {
        result.summary = "Graph is empty.";
        return result;
    }

    int totalEdges = getTotalEdgeCount();
    if (totalEdges == 0) {
        result.summary = "Graph has no edges.";
        return result;
    }

    GraphEdge* allEdges = new GraphEdge[totalEdges];
    getAllEdges(allEdges, totalEdges);

    // Sort edges by ascending weight using custom quicksort (No STL)
    customSortEdges(allEdges, 0, totalEdges - 1);

    DisjointSet dsu(vertexCount);
    GraphEdge* mst = new GraphEdge[vertexCount];
    int mstCount = 0;
    int totalCost = 0;

    for (int i = 0; i < totalEdges; ++i) {
        int u = allEdges[i].u;
        int v = allEdges[i].v;
        int w = allEdges[i].weight;

        if (dsu.unite(u, v)) {
            mst[mstCount++] = {u, v, w};
            totalCost += w;
            if (mstCount == vertexCount - 1)
                break;
        }
    }

    delete[] allEdges;

    // Count components in DSU to determine if graph is connected
    int compCount = 0;
    for (int i = 0; i < vertexCount; ++i) {
        if (dsu.find(i) == i)
            ++compCount;
    }

    bool connected = (compCount == 1 && vertexCount > 0);

    std::ostringstream ss;
    ss << "=== KRUSKAL'S MINIMUM SPANNING TREE ===\n\n";

    if (connected) {
        ss << "STATUS: Complete Minimum Spanning Tree constructed.\n";
        ss << "Total Vertices (V): " << vertexCount << "\n";
        ss << "MST Edges (V - 1): " << mstCount << "\n";
        ss << "Total Minimum Spanning Tree Weight: " << totalCost << "\n\n";
    } else {
        ss << "STATUS: Graph is DISCONNECTED! Result is a Minimum Spanning Forest.\n";
        ss << "Number of Disconnected Components: " << compCount << "\n";
        ss << "Total Edges in Forest: " << mstCount << "\n";
        ss << "Total Spanning Forest Weight: " << totalCost << "\n\n";
    }

    ss << "Selected MST Edges:\n";
    ss << std::string(45, '-') << "\n";
    ss << "Edge #      From ➔ To             Weight\n";
    ss << std::string(45, '-') << "\n";

    for (int i = 0; i < mstCount; ++i) {
        std::string edgeStr = getVertexLabel(mst[i].u) + " ➔ " + getVertexLabel(mst[i].v);
        ss << "  " << (i + 1) << "         " << edgeStr;
        int pad = 22 - static_cast<int>(edgeStr.length());
        for (int k = 0; k < pad; ++k) ss << " ";
        ss << mst[i].weight << "\n";
    }

    result.mstEdges = mst;
    result.mstEdgeCount = mstCount;
    result.totalWeight = totalCost;
    result.isConnected = connected;
    result.componentCount = compCount;
    result.summary = ss.str();
    return result;
}


// ==================================================
// EDUCATIONAL SAMPLE GRAPH PRESETS
// ==================================================

void Graph::loadSampleGraph() {
    clear();
    // Classic 6-vertex weighted connected graph
    addVertex("A"); // 0
    addVertex("B"); // 1
    addVertex("C"); // 2
    addVertex("D"); // 3
    addVertex("E"); // 4
    addVertex("F"); // 5

    addEdge(0, 1, 4); // A - B (4)
    addEdge(0, 2, 2); // A - C (2)
    addEdge(1, 2, 1); // B - C (1)
    addEdge(1, 3, 5); // B - D (5)
    addEdge(2, 3, 8); // C - D (8)
    addEdge(2, 4, 10); // C - E (10)
    addEdge(3, 4, 2); // D - E (2)
    addEdge(3, 5, 6); // D - F (6)
    addEdge(4, 5, 3); // E - F (3)
}

void Graph::loadDisconnectedSample() {
    clear();
    // Component 1: A, B, C, D
    addVertex("A");
    addVertex("B");
    addVertex("C");
    addVertex("D");
    // Component 2: X, Y, Z
    addVertex("X");
    addVertex("Y");
    addVertex("Z");

    addEdge(0, 1, 3); // A - B
    addEdge(1, 2, 4); // B - C
    addEdge(2, 3, 2); // C - D
    addEdge(3, 0, 5); // D - A

    addEdge(4, 5, 7); // X - Y
    addEdge(5, 6, 6); // Y - Z
    addEdge(6, 4, 8); // Z - X
}

} // namespace algolib
