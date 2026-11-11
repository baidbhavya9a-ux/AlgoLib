#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "algolib/data_structures.hpp"
#include "algolib/compressx.hpp"
#include "algolib/graphkit.hpp"
#include "algolib/dplab.hpp"

using namespace algolib;

int testsPassed = 0;
int testsFailed = 0;

#define TEST_ASSERT(cond, desc) \
    do { \
        if (cond) { \
            std::cout << "  [PASS] " << desc << "\n"; \
            ++testsPassed; \
        } else { \
            std::cerr << "  [FAIL] " << desc << " (Line " << __LINE__ << ")\n"; \
            ++testsFailed; \
        } \
    } while (0)

// ==================================================
// 1. DATA STRUCTURES TESTS
// ==================================================
void testDataStructures() {
    std::cout << "\n========================================\n";
    std::cout << "TEST SUITE 1: CORE DATA STRUCTURES\n";
    std::cout << "========================================\n";

    // 1.1 Dynamic Array
    {
        DynamicArray arr;
        TEST_ASSERT(arr.empty(), "Dynamic Array starts empty");
        TEST_ASSERT(arr.size() == 0, "Initial size is 0");

        bool threwOnEmpty = false;
        try { arr.remove(); } catch (...) { threwOnEmpty = true; }
        TEST_ASSERT(threwOnEmpty, "Dynamic Array throws on empty remove");

        arr.insert(10);
        arr.insert(20);
        arr.insert(30);
        TEST_ASSERT(arr.size() == 3, "Array size is 3 after 3 inserts");
        TEST_ASSERT(arr.at(0) == 10 && arr.at(1) == 20 && arr.at(2) == 30, "Array elements match inserted values");
        TEST_ASSERT(arr.search(20) == true && arr.search(99) == false, "Array search works");

        arr.insertAt(1, 15);
        TEST_ASSERT(arr.size() == 4 && arr.at(1) == 15 && arr.at(2) == 20, "Insert at index 1 works");

        arr.removeAt(1);
        TEST_ASSERT(arr.size() == 3 && arr.at(1) == 20, "Remove at index 1 works");

        arr.remove();
        TEST_ASSERT(arr.size() == 2 && arr.at(1) == 20, "Remove back works");

        // Test deep copy
        DynamicArray copy = arr;
        copy.insert(999);
        TEST_ASSERT(copy.size() == 3 && arr.size() == 2, "Dynamic Array deep copy works independently");
    }

    // 1.2 Linked List
    {
        LinkedList list;
        TEST_ASSERT(list.empty(), "Linked List starts empty");
        list.insert(100);
        list.insert(200);
        list.insertHead(50);
        TEST_ASSERT(list.size() == 3, "Linked List size is 3");
        TEST_ASSERT(list.at(0) == 50 && list.at(1) == 100 && list.at(2) == 200, "Head and tail inserts correct");
        TEST_ASSERT(list.search(100) == true && list.search(999) == false, "List search works");

        bool removed = list.remove(100);
        TEST_ASSERT(removed && list.size() == 2 && !list.search(100), "Remove middle node works");

        removed = list.remove(50);
        TEST_ASSERT(removed && list.size() == 1 && list.at(0) == 200, "Remove head node works");

        // Deep copy
        LinkedList copy = list;
        copy.insert(300);
        TEST_ASSERT(copy.size() == 2 && list.size() == 1, "Linked List deep copy works");
    }

    // 1.3 Stack
    {
        Stack s;
        TEST_ASSERT(s.empty(), "Stack starts empty");
        bool threwOnEmptyPop = false;
        try { s.pop(); } catch (...) { threwOnEmptyPop = true; }
        TEST_ASSERT(threwOnEmptyPop, "Stack pop throws underflow when empty");

        s.push(11);
        s.push(22);
        s.push(33);
        TEST_ASSERT(s.size() == 3, "Stack size is 3");
        TEST_ASSERT(s.peek() == 33, "Stack peek returns top (33)");
        TEST_ASSERT(s.pop() == 33, "Stack pop returns 33");
        TEST_ASSERT(s.pop() == 22, "Stack pop returns 22");
        TEST_ASSERT(s.size() == 1 && s.peek() == 11, "Stack size is 1 with top 11");
    }

    // 1.4 Queue
    {
        Queue q;
        TEST_ASSERT(q.empty(), "Queue starts empty");
        bool threwOnEmptyDeq = false;
        try { q.dequeue(); } catch (...) { threwOnEmptyDeq = true; }
        TEST_ASSERT(threwOnEmptyDeq, "Queue dequeue throws underflow when empty");

        q.enqueue(101);
        q.enqueue(102);
        q.enqueue(103);
        TEST_ASSERT(q.size() == 3, "Queue size is 3");
        TEST_ASSERT(q.front() == 101, "Queue front is 101");
        TEST_ASSERT(q.dequeue() == 101, "Queue FIFO dequeue is 101");
        TEST_ASSERT(q.front() == 102, "New front is 102");
    }

    // 1.5 Binary Search Tree (BST)
    {
        BST bst;
        TEST_ASSERT(bst.empty(), "BST starts empty");
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);
        bst.insert(60);
        bst.insert(80);

        TEST_ASSERT(bst.size() == 7, "BST has 7 distinct nodes");
        TEST_ASSERT(bst.search(40) == true && bst.search(999) == false, "BST search works");

        // Duplicate value support
        bst.insert(40);
        TEST_ASSERT(bst.size() == 7, "BST node count remains 7 on duplicate insert");
        TEST_ASSERT(bst.totalCount() == 8, "BST total element count is 8 with duplicate");

        // Traversal verification
        std::string in = bst.inorder();
        TEST_ASSERT(in.find("20 30 40 40 50 60 70 80") != std::string::npos, "BST Inorder is sorted with duplicates");

        // BST Deletion: duplicate count decrement
        bst.remove(40);
        TEST_ASSERT(bst.totalCount() == 7, "Removing duplicate decrements count");

        // BST Deletion: 0 children (leaf 20)
        bst.remove(20);
        TEST_ASSERT(!bst.search(20) && bst.size() == 6, "Leaf node deletion (0 children) works");

        // BST Deletion: 1 child (add 10 to 30's left then delete)
        bst.insert(25);
        bst.remove(30);
        TEST_ASSERT(!bst.search(30) && bst.search(25) && bst.search(40), "Single child node deletion works");

        // BST Deletion: 2 children (delete root 50)
        bst.remove(50);
        TEST_ASSERT(!bst.search(50) && bst.search(60) && bst.search(70), "Two-children root deletion works");
    }

    // 1.6 Min Heap
    {
        MinHeap h;
        TEST_ASSERT(h.empty(), "Min Heap starts empty");
        bool threwOnEmptyExtract = false;
        try { h.extractMin(); } catch (...) { threwOnEmptyExtract = true; }
        TEST_ASSERT(threwOnEmptyExtract, "Min Heap throws underflow when empty");

        h.insert(40);
        h.insert(10);
        h.insert(30);
        h.insert(5);
        h.insert(20);

        TEST_ASSERT(h.size() == 5, "Min Heap size is 5");
        TEST_ASSERT(h.peekMin() == 5, "Min Heap minimum is 5");
        TEST_ASSERT(h.extractMin() == 5, "extractMin returns 5");
        TEST_ASSERT(h.extractMin() == 10, "extractMin returns 10");
        TEST_ASSERT(h.extractMin() == 20, "extractMin returns 20");
        TEST_ASSERT(h.extractMin() == 30, "extractMin returns 30");
        TEST_ASSERT(h.extractMin() == 40, "extractMin returns 40");
        TEST_ASSERT(h.empty(), "Min Heap empty after extracting all");
    }
}


// ==================================================
// 2. COMPRESSX HUFFMAN CODING TESTS
// ==================================================
void testCompressX() {
    std::cout << "\n========================================\n";
    std::cout << "TEST SUITE 2: COMPRESSX HUFFMAN ENGINE\n";
    std::cout << "========================================\n";

    // 2.1 Text file compression and decompression roundtrip
    {
        std::string originalFile = "test_sample.txt";
        std::string compressedFile = "test_sample.txt.cmpx";
        std::string decompressedFile = "test_sample_restored.txt";

        // Create sample text file
        std::ofstream out(originalFile, std::ios::binary);
        out << "The quick brown fox jumps over the lazy dog! 1234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz\n";
        out << "Huffman coding is an optimal prefix code used commonly for lossless data compression.\n";
        out.close();

        CompressionStats compStats = CompressX::compressFile(originalFile, compressedFile);
        TEST_ASSERT(compStats.success, "Text file compressed successfully");
        TEST_ASSERT(compStats.compressedBytes > 0, "Compressed archive non-empty");

        CompressionStats decompStats = CompressX::decompressFile(compressedFile, decompressedFile);
        TEST_ASSERT(decompStats.success, "Text file decompressed successfully");
        TEST_ASSERT(decompStats.originalBytes == compStats.originalBytes, "Decompressed size matches original size");

        bool match = CompressX::verifyFilesMatch(originalFile, decompressedFile);
        TEST_ASSERT(match, "VERIFIED: Bit-for-bit exact match on text file round-trip");

        std::remove(originalFile.c_str());
        std::remove(compressedFile.c_str());
        std::remove(decompressedFile.c_str());
    }

    // 2.2 Binary file compression and decompression roundtrip
    {
        std::string originalBin = "test_binary.dat";
        std::string compressedBin = "test_binary.dat.cmpx";
        std::string decompressedBin = "test_binary_restored.dat";

        // Create random byte buffer including all byte values 0..255
        std::ofstream out(originalBin, std::ios::binary);
        for (int i = 0; i < 2048; ++i) {
            uint8_t b = static_cast<uint8_t>((i * 37 + 13) % 256);
            out.put(static_cast<char>(b));
        }
        out.close();

        CompressionStats compStats = CompressX::compressFile(originalBin, compressedBin);
        TEST_ASSERT(compStats.success, "Binary file compressed successfully");

        CompressionStats decompStats = CompressX::decompressFile(compressedBin, decompressedBin);
        TEST_ASSERT(decompStats.success, "Binary file decompressed successfully");

        bool match = CompressX::verifyFilesMatch(originalBin, decompressedBin);
        TEST_ASSERT(match, "VERIFIED: Bit-for-bit exact match on binary file round-trip");

        std::remove(originalBin.c_str());
        std::remove(compressedBin.c_str());
        std::remove(decompressedBin.c_str());
    }

    // 2.3 Empty file handling
    {
        std::string emptyOrig = "test_empty.txt";
        std::string emptyComp = "test_empty.txt.cmpx";
        std::string emptyDecomp = "test_empty_restored.txt";

        std::ofstream out(emptyOrig, std::ios::binary);
        out.close();

        CompressionStats compStats = CompressX::compressFile(emptyOrig, emptyComp);
        TEST_ASSERT(compStats.success, "Empty file compression handled safely");

        CompressionStats decompStats = CompressX::decompressFile(emptyComp, emptyDecomp);
        TEST_ASSERT(decompStats.success, "Empty file decompression handled safely");

        bool match = CompressX::verifyFilesMatch(emptyOrig, emptyDecomp);
        TEST_ASSERT(match, "VERIFIED: Empty file bit-for-bit match");

        std::remove(emptyOrig.c_str());
        std::remove(emptyComp.c_str());
        std::remove(emptyDecomp.c_str());
    }
}


// ==================================================
// 3. GRAPHKIT TESTS
// ==================================================
void testGraphKit() {
    std::cout << "\n========================================\n";
    std::cout << "TEST SUITE 3: GRAPHKIT ALGORITHMS\n";
    std::cout << "========================================\n";

    // 3.1 Connected Graph (BFS, DFS, Dijkstra, Kruskal)
    {
        Graph g(false); // Undirected
        g.loadSampleGraph(); // 6 vertices A..F, 9 edges
        TEST_ASSERT(g.getVertexCount() == 6, "Sample graph has 6 vertices");
        TEST_ASSERT(g.getTotalEdgeCount() == 9, "Sample graph has 9 undirected edges");

        // BFS Traversal
        BFSResult bfsRes = g.bfs(0);
        TEST_ASSERT(bfsRes.visitedCount == 6, "BFS visited all 6 vertices");
        TEST_ASSERT(bfsRes.componentCount == 1, "BFS detected 1 connected component");
        delete[] bfsRes.visitedOrder;

        // DFS Traversal
        DFSResult dfsRes = g.dfs(0);
        TEST_ASSERT(dfsRes.visitedCount == 6, "DFS visited all 6 vertices");
        TEST_ASSERT(dfsRes.componentCount == 1, "DFS detected 1 connected component");
        delete[] dfsRes.visitedOrder;

        // Dijkstra's Shortest Path
        // A=0, B=1, C=2, D=3, E=4, F=5
        // Edges: A-B(4), A-C(2), B-C(1), B-D(5), C-D(8), C-E(10), D-E(2), D-F(6), E-F(3)
        // Shortest Path A -> F: A -> C (2) -> B (2+1=3) -> D (3+5=8) -> E (8+2=10) -> F (10+3=13)
        // Wait: A -> C (2), B-C (1) so A-B is min(4, 2+1=3). A->D: min(4+5=9, 3+5=8). A->E: 8+2=10. A->F: 10+3=13.
        DijkstraResult dijk = g.dijkstra(0, 5);
        TEST_ASSERT(dijk.pathExists, "Dijkstra found path from A to F");
        TEST_ASSERT(dijk.distances[5] == 13, "Dijkstra shortest distance A->F is 13");
        delete[] dijk.distances;
        delete[] dijk.parent;
        delete[] dijk.path;

        // Kruskal's MST
        KruskalResult mst = g.kruskalMST();
        TEST_ASSERT(mst.isConnected, "Kruskal confirms connected MST");
        TEST_ASSERT(mst.mstEdgeCount == 5, "MST has V-1 = 5 edges");
        // Edges in MST: B-C(1), D-E(2), A-C(2), E-F(3), B-D(5) => 1+2+2+3+5 = 13
        TEST_ASSERT(mst.totalWeight == 13, "MST total weight is 13");
        delete[] mst.mstEdges;
    }

    // 3.2 Disconnected Graph Handling
    {
        Graph g(false);
        g.loadDisconnectedSample(); // Component 1 (A,B,C,D), Component 2 (X,Y,Z)
        TEST_ASSERT(g.getVertexCount() == 7, "Disconnected graph has 7 vertices");

        BFSResult bfsRes = g.bfs(0, true);
        TEST_ASSERT(bfsRes.visitedCount == 7, "BFS with visitAllComponents visits all components");
        TEST_ASSERT(bfsRes.componentCount == 2, "BFS detects 2 disconnected components");
        delete[] bfsRes.visitedOrder;

        // Dijkstra across disconnected components (A -> X)
        DijkstraResult dijk = g.dijkstra(0, 4); // A=0, X=4
        TEST_ASSERT(!dijk.pathExists, "Dijkstra correctly detects unreachable disconnected target");
        delete[] dijk.distances;
        delete[] dijk.parent;

        // Kruskal on disconnected graph
        KruskalResult mst = g.kruskalMST();
        TEST_ASSERT(!mst.isConnected, "Kruskal correctly detects disconnected graph (Forest)");
        TEST_ASSERT(mst.componentCount == 2, "Kruskal reports 2 spanning components");
        delete[] mst.mstEdges;
    }
}


// ==================================================
// 4. DP LAB TESTS
// ==================================================
void testDPLab() {
    std::cout << "\n========================================\n";
    std::cout << "TEST SUITE 4: DYNAMIC PROGRAMMING LAB\n";
    std::cout << "========================================\n";

    // 4.1 Fibonacci
    {
        FibResult f10 = DPLab::computeFibonacci(10);
        TEST_ASSERT(f10.result == 55, "Fibonacci F(10) == 55");
        DPLab::freeFibResult(f10);

        FibResult f0 = DPLab::computeFibonacci(0);
        TEST_ASSERT(f0.result == 0, "Fibonacci F(0) == 0");
        DPLab::freeFibResult(f0);

        FibResult f1 = DPLab::computeFibonacci(1);
        TEST_ASSERT(f1.result == 1, "Fibonacci F(1) == 1");
        DPLab::freeFibResult(f1);

        FibResult f20 = DPLab::computeFibonacci(20);
        TEST_ASSERT(f20.result == 6765, "Fibonacci F(20) == 6765");
        DPLab::freeFibResult(f20);
    }

    // 4.2 0/1 Knapsack
    {
        int weights[] = {2, 3, 4, 5};
        int values[]  = {3, 4, 5, 6};
        int capacity = 5;
        KnapsackResult ks = DPLab::solveKnapsack(capacity, weights, values, 4);
        // Max value: items with wt 2 and 3 have values 3 and 4 -> total val 7, wt 5
        TEST_ASSERT(ks.maxValue == 7, "0/1 Knapsack optimal value is 7");
        TEST_ASSERT(ks.totalWeightUsed == 5, "0/1 Knapsack total weight used is 5");
        TEST_ASSERT(ks.selectedCount == 2, "0/1 Knapsack selected exactly 2 items");
        DPLab::freeKnapsackResult(ks);
    }

    // 4.3 Longest Common Subsequence (LCS)
    {
        std::string s1 = "ABCBDAB";
        std::string s2 = "BDCAB";
        LCSResult lcs = DPLab::solveLCS(s1, s2);
        TEST_ASSERT(lcs.lcsLength == 4, "LCS length is 4 (e.g. BDAB or BCAB)");
        TEST_ASSERT(lcs.lcsString.length() == 4, "Reconstructed LCS string length is 4");
        DPLab::freeLCSResult(lcs);
    }

    // 4.4 Coin Change
    {
        int coins[] = {1, 2, 5};
        CoinChangeResult cc1 = DPLab::solveCoinChange(11, coins, 3);
        TEST_ASSERT(cc1.isPossible, "Coin change 11 with [1,2,5] is possible");
        TEST_ASSERT(cc1.minCoins == 3, "Minimum coins for 11 is 3 (5 + 5 + 1)");
        DPLab::freeCoinChangeResult(cc1);

        int coins2[] = {2};
        CoinChangeResult cc2 = DPLab::solveCoinChange(3, coins2, 1);
        TEST_ASSERT(!cc2.isPossible, "Coin change 3 with [2] is impossible");
        TEST_ASSERT(cc2.minCoins == -1, "Returns -1 for impossible amount");
        DPLab::freeCoinChangeResult(cc2);
    }
}


// ==================================================
// MAIN TEST RUNNER
// ==================================================
int main() {
    std::cout << "========================================================\n";
    std::cout << "AlgoLib - Comprehensive Automated ADSA Test Runner\n";
    std::cout << "========================================================\n";

    testDataStructures();
    testCompressX();
    testGraphKit();
    testDPLab();

    std::cout << "\n========================================================\n";
    std::cout << "TEST EXECUTION SUMMARY\n";
    std::cout << "========================================================\n";
    std::cout << "Total Assertions Passed: " << testsPassed << "\n";
    std::cout << "Total Assertions Failed: " << testsFailed << "\n";

    if (testsFailed == 0) {
        std::cout << "\n>>> ALL TESTS PASSED SUCCESSFULLY! <<<\n\n";
        return 0;
    } else {
        std::cerr << "\n>>> SOME TESTS FAILED! <<<\n\n";
        return 1;
    }
}
