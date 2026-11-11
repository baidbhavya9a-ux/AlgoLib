#ifndef ALGOLIB_GRAPHKIT_HPP
#define ALGOLIB_GRAPHKIT_HPP

#include <string>
#include <cstddef>

namespace algolib {

// Graph Edge representation for weighted graphs
struct GraphEdge {
    int u;
    int v;
    int weight;
};

// Results of Breadth-First Search traversal
struct BFSResult {
    int* visitedOrder;
    int visitedCount;
    int componentCount;
    std::string summary;
};

// Results of Depth-First Search traversal
struct DFSResult {
    int* visitedOrder;
    int visitedCount;
    int componentCount;
    std::string summary;
};

// Results of Dijkstra's Shortest Path algorithm
struct DijkstraResult {
    int* distances;
    int* parent;
    int vertexCount;
    int startVertex;
    int targetVertex;
    int* path;
    int pathLength;
    bool pathExists;
    std::string summary;
};

// Results of Kruskal's Minimum Spanning Tree algorithm
struct KruskalResult {
    GraphEdge* mstEdges;
    int mstEdgeCount;
    int totalWeight;
    bool isConnected;
    int componentCount;
    std::string summary;
};

// Custom Weighted Graph class without STL containers
class Graph {
private:
    struct EdgeNode {
        int to;
        int weight;
        EdgeNode* next;
        EdgeNode(int t, int w, EdgeNode* n = nullptr) : to(t), weight(w), next(n) {}
    };

    int vertexCount;
    int vertexCapacity;
    char** vertexLabels;
    EdgeNode** adjList;
    bool directed;

    void growVertices();
    static void customSortEdges(GraphEdge* edges, int low, int high);
    static int partitionEdges(GraphEdge* edges, int low, int high);

public:
    explicit Graph(bool isDirected = false);
    ~Graph();

    // Prevent accidental shallow copies
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    void clear();

    int addVertex(const std::string& label);
    int getVertexIndex(const std::string& label) const;
    std::string getVertexLabel(int index) const;
    int getVertexCount() const { return vertexCount; }
    bool isDirected() const { return directed; }

    bool addEdge(int u, int v, int weight = 1);
    bool addEdge(const std::string& fromLabel, const std::string& toLabel, int weight = 1);
    bool removeEdge(int u, int v);

    // Adjacency inspection
    int getDegree(int u) const;
    bool hasEdge(int u, int v) const;
    int getEdgeWeight(int u, int v) const;

    // Traversal & path algorithms
    BFSResult bfs(int startVertex = 0, bool visitAllComponents = true) const;
    DFSResult dfs(int startVertex = 0, bool visitAllComponents = true) const;
    DijkstraResult dijkstra(int startVertex, int targetVertex = -1) const;
    KruskalResult kruskalMST() const;

    // Export edges for visualization
    int getAllEdges(GraphEdge* output, int maxEdges) const;
    int getTotalEdgeCount() const;

    // Load sample educational graph presets
    void loadSampleGraph();
    void loadDisconnectedSample();
};

} // namespace algolib

#endif // ALGOLIB_GRAPHKIT_HPP

