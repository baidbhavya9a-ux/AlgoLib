# AlgoLib — Advanced Data Structures & Algorithms (ADSA) Toolkit

[![C++17](https://img.shields.io/badge/Standard-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Qt 6.12](https://img.shields.io/badge/GUI-Qt%20Widgets%206.12.0-41CD52.svg)](https://www.qt.io/)
[![Build System](https://img.shields.io/badge/Build-CMake%20%7C%20Ninja-064F8C.svg)](https://cmake.org/)
[![Compiler](https://img.shields.io/badge/Compiler-MinGW%20GCC%2013.1.0%20(64--bit)-purple.svg)](https://www.mingw-w64.org/)
[![Tests](https://img.shields.io/badge/Tests-91%20Passed%20%7C%200%20Failed-brightgreen.svg)]()

---

## 1. Project Introduction

**AlgoLib** is a comprehensive, standalone educational software suite designed for the **Advanced Data Structures and Algorithms (ADSA)** assessment. It combines rigorous, low-level manual C++ implementations of fundamental computer science structures and algorithms with an intuitive, modern Qt Widgets desktop interface.

In strict adherence to ADSA pedagogical rules, **none of the core data structures or algorithms utilize the C++ Standard Template Library (STL) containers** (such as `std::vector`, `std::list`, `std::map`, `std::set`, or `std::priority_queue`). Every structure is engineered from first principles utilizing raw pointers, dynamically allocated contiguous arrays, linked node graphs, manual memory management (`new[]`/`delete[]`), and custom bitwise stream serializers.

---

## 2. Project Objectives

- **First-Principles Mastery:** Implement core abstract data types (ADTs) manually, managing dynamic memory and pointer manipulation without third-party abstraction layers.
- **Visual Feedback:** Provide live, real-time graphical representations of in-memory data structures, illustrating dynamic capacity reallocation, pointer connections, tree branches, heap ordering, and graph topologies.
- **Practical Application (CompressX):** Demonstrate real-world utility by implementing a full Huffman lossless compression and decompression engine that performs true bit-level file encoding and decoding.
- **Graph Theory Exploration (GraphKit):** Implement classic graph traversal, shortest path, and minimum spanning tree algorithms with full support for disconnected graphs.
- **Dynamic Programming Laboratory (DP Lab):** Unpack optimal substructure and overlapping subproblems by interactively rendering the underlying state transitions, 2D recurrence matrices, and traceback pathways.

---

## 3. High-Level Features

AlgoLib is partitioned into four major interactive modules, each featuring a **Synchronized Dual-View (Graphical Visualizer on the Left, Live C++ Implementation Snippet on the Right)**:

```
+-----------------------------------------------------------------------------------+
|                                  AlgoLib GUI                                      |
+-------------------+-------------------+-------------------+-----------------------+
|  Data Structures  |     CompressX     |     GraphKit      |        DP Lab         |
|  - Dynamic Array  |  - Huffman Tree   |  - Adjacency List |  - Fibonacci (1D)     |
|  - Linked List    |  - Priority Queue |  - BFS Traversal  |  - 0/1 Knapsack (2D)  |
|  - Stack (LIFO)   |  - Bit Stream I/O |  - DFS Traversal  |  - LCS (2D Matrix)    |
|  - Queue (FIFO)   |  - File Archive   |  - Dijkstra Path  |  - Coin Change (1D)   |
|  - BST (Counting) |  - Decompression  |  - Kruskal MST    |  - Table Visualizer   |
|  - Min Heap       |  - Verification   |  - Graph Canvas   |  - Traceback Engine   |
+-------------------+-------------------+-------------------+-----------------------+
|  [Left: Real-time Visual Canvas]     <-->    [Right: Synchronized C++ Code Panel] |
|  [Bottom: Execution Log / Diagnostic Console / Path Trace / Traceback Analysis]   |
+-----------------------------------------------------------------------------------+
```

### Key Interactive Features
- **Execution-Synchronized C++ Code Panel:** Every successful operation dynamically displays the exact 3–10 lines of underlying C++ implementation responsible for it, with syntax highlighting, line numbers, file source identification, and one-click clipboard copying.
- **Full Enter-Key Navigation:** Every single-line input field across all 4 modules (value inputs, search inputs, vertex labels, edge weights, file paths, and DP inputs) supports instant Enter-key execution.
- **Defensive Input Validation:** Duplicate vertex prevention, negative weight detection in Dijkstra, empty label rejection, and bounds-checked data structures ensure error-free operation.

---

## 4. Technologies Used

- **Operating System:** Windows 10 / 11 (x64)
- **Programming Language:** C++17 (`-std=gnu++17`)
- **GUI Framework:** Qt Widgets 6.12.0 (C++ API)
- **Compiler:** MinGW GCC 13.1.0 64-bit (`C:\Qt\Tools\mingw1310_64`)
- **Build System:** CMake 3.21+ (`C:\Program Files\CMake`)
- **Generator:** Ninja (`C:\Qt\Tools\Ninja\ninja.exe`)

---

## 5. Module Breakdown & Implemented Data Structures

### 5.1 Data Structures Module
All data structures implement custom dynamic memory management, deep copying (Rule of Three), bound checking, and empty-state exception handling.

1. **Dynamic Array:**
   - Contiguous memory buffer with amortized $O(1)$ geometric expansion ($2\times$ factor).
   - Operations: `insert(val)`, `insertAt(idx, val)`, `remove()`, `removeAt(idx)`, `removeValue(val)`, `search(val)`, `indexOf(val)`, `at(idx)`, `clear()`.
   - **Visualization:** Draws index labels, occupied memory blocks, and dashed empty blocks for allocated capacity.

2. **Singly Linked List:**
   - Pointer-linked nodes (`struct Node { int data; Node* next; }`).
   - Operations: `insert(val)` (append), `insertHead(val)`, `remove(val)`, `search(val)`, `at(idx)`, `clear()`.
   - **Visualization:** Draws `HEAD` pointer badge, node compartments `[Data | Next]`, and arrow lines terminating in `NULL`.

3. **Stack (LIFO):**
   - Linked-node stack guaranteeing strict $O(1)$ operations with zero overflow risk.
   - Operations: `push(val)`, `pop()`, `peek()`, `empty()`, `size()`, `clear()`.
   - **Visualization:** Vertical chamber displaying stacked elements from bottom to top, with a dynamic `TOP ->` pointer.

4. **Queue (FIFO):**
   - Linked queue maintaining discrete `frontNode` and `rearNode` pointers.
   - Operations: `enqueue(val)`, `dequeue()`, `front()`, `empty()`, `size()`, `clear()`.
   - **Visualization:** Horizontal pipeline showing elements flowing from `FRONT` to `REAR`.

5. **Binary Search Tree (BST):**
   - Pointer-based binary tree with duplicate value support via node occurrence counting (`count` field).
   - Supported Operations: `insert(val)`, `remove(val)` (supports 0, 1, and 2 children with inorder successor replacement), `search(val)`, `inorder()`, `preorder()`, `postorder()`.
   - **Visualization:** Antialiased hierarchical node layout with connection lines and duplicate counter badges (`×N`).

6. **Min Heap:**
   - Complete binary tree represented in a dynamically resized 0-indexed array.
   - Operations: `insert(val)` with `siftUp()`, `extractMin()` with `siftDown()`, `peekMin()`, `search(val)`, `clear()`.
   - **Visualization:** Dual view displaying both the complete binary tree structure and the underlying indexed array.

---

### 5.2 CompressX Module (Huffman Lossless Compression)

CompressX implements a complete, self-contained file compressor and decompressor without external libraries:

- **Custom Min-Priority Queue:** Implemented from scratch using a min-heap array of `HuffmanNode*`.
- **Character Frequency Analysis:** Scans binary input streams to build byte distribution tables ($0 \dots 255$).
- **Canonical Tree Construction:** Repeatedly merges the two lowest-frequency nodes until a root node is formed.
- **Bitwise Stream Serialization:**
  - `BitWriter`: Buffers individual bits into a byte buffer and flushes to disk.
  - `BitReader`: Reads byte streams and extracts individual bits sequentially.
- **Archive File Specification (`.cmpx`):**
  - **Magic Header:** 4 bytes (`CMPX`)
  - **Original Size:** 8 bytes (`uint64_t`)
  - **Unique Symbols Count:** 2 bytes (`uint16_t`)
  - **Frequency Table:** Symbol-frequency pairs (`uint8_t` + `uint64_t`)
  - **Payload:** Encoded variable-length bit stream
- **Integrity Verification:** Automated byte-for-byte binary comparison between original and restored files.

---

### 5.3 GraphKit Module (Graph Theory & Network Analysis)

- **Custom Graph Representation:** Adjacency list using manual dynamic arrays of linked edge nodes (`EdgeNode** adjList`).
- **Algorithms:**
  - **Breadth-First Search (BFS):** Explores graph layer-by-layer using custom FIFO queue; detects and numbers all disconnected components.
  - **Depth-First Search (DFS):** Explores depth-first using custom LIFO stack; tracks multiple connected components.
  - **Dijkstra's Shortest Path:** Computes shortest paths on weighted graphs using a custom min-heap priority queue; detects unreachable vertices in disconnected components and reconstructs optimal paths.
  - **Kruskal's Minimum Spanning Tree (MST):** Extracts edges, sorts them using manual quicksort, and applies Disjoint Set Union (DSU with path compression and rank); handles disconnected graphs by generating a Minimum Spanning Forest.
- **Interactive Visualizer:** Real-time circular network layout with customizable edge weights and path highlighting (cyan for shortest paths, emerald for MST, amber for traversals).

---

### 5.4 DP Lab Module (Dynamic Programming)

1. **Fibonacci Sequence:**
   - Tabulation computing $F(N)$ for $0 \le N \le 90$.
   - Renders 1D state transition table.
2. **0/1 Knapsack Optimization:**
   - Computes maximum value for item weights and values under capacity $W$.
   - Allocates $(N+1) \times (W+1)$ 2D DP matrix; traces back selected items.
3. **Longest Common Subsequence (LCS):**
   - Finds length and reconstructs the optimal string subsequence between String A and String B.
   - Visualizes full 2D matrix with character axis headers.
4. **Coin Change (Fewest Coins):**
   - Unbounded dynamic programming to find the minimum number of coins to make target amount $A$.
   - Reconstructs exact coin breakdown or flags impossible combinations.

---

## 6. Time and Space Complexity Reference

| Module | Algorithm / Data Structure | Time Complexity (Average) | Time Complexity (Worst) | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **Data Structures** | Dynamic Array Append | $O(1)$ amortized | $O(N)$ | $O(N)$ |
| | Dynamic Array Insert/Remove at Index | $O(N)$ | $O(N)$ | $O(1)$ auxiliary |
| | Linked List Insert / Delete | $O(1)$ head / $O(N)$ | $O(N)$ | $O(N)$ |
| | Stack Push / Pop | $O(1)$ | $O(1)$ | $O(N)$ |
| | Queue Enqueue / Dequeue | $O(1)$ | $O(1)$ | $O(N)$ |
| | BST Search / Insert / Delete | $O(\log N)$ | $O(N)$ | $O(N)$ |
| | Min Heap Insert / ExtractMin | $O(\log N)$ | $O(\log N)$ | $O(N)$ |
| **CompressX** | Frequency Analysis | $O(N)$ | $O(N)$ | $O(1)$ (256 bins) |
| | Huffman Tree Construction | $O(K \log K)$ ($K \le 256$) | $O(K \log K)$ | $O(K)$ |
| | Bit Encoding & Decoding | $O(N)$ | $O(N)$ | $O(N)$ |
| **GraphKit** | Breadth-First Search (BFS) | $O(V + E)$ | $O(V + E)$ | $O(V)$ |
| | Depth-First Search (DFS) | $O(V + E)$ | $O(V + E)$ | $O(V)$ |
| | Dijkstra's Shortest Path | $O(E \log V)$ | $O(E \log V)$ | $O(V)$ |
| | Kruskal's MST (with DSU) | $O(E \log E)$ | $O(E \log E)$ | $O(V + E)$ |
| **DP Lab** | Fibonacci Tabulation | $O(N)$ | $O(N)$ | $O(N)$ |
| | 0/1 Knapsack Problem | $O(N \cdot W)$ | $O(N \cdot W)$ | $O(N \cdot W)$ |
| | Longest Common Subsequence | $O(M \cdot N)$ | $O(M \cdot N)$ | $O(M \cdot N)$ |
| | Coin Change Problem | $O(A \cdot C)$ | $O(A \cdot C)$ | $O(A)$ |

---

## 7. Project Architecture & Folder Structure

```
adsa/
└── AlgoLib/
    ├── CMakeLists.txt              # Unified CMake build script
    ├── .gitignore                  # Git repository exclusion rules
    ├── README.md                   # Academic project documentation
    ├── app/
    │   ├── main.cpp                # Main Qt Widgets GUI application
    │   └── main_backup.cpp         # Preserved original backup
    ├── include/
    │   └── algolib/
    │       ├── algolib.hpp         # Umbrella master include header
    │       ├── data_structures.hpp # Custom 6 ADTs (Array, List, Stack, Queue, BST, Heap)
    │       ├── visualizer.hpp      # Live memory & hierarchy visualizers
    │       ├── compressx.hpp       # Huffman coding & bit I/O engine
    │       ├── graphkit.hpp        # Graph algorithms (BFS, DFS, Dijkstra, Kruskal)
    │       ├── graph_visualizer.hpp# Interactive graph canvas widget
    │       ├── dplab.hpp           # DP algorithms (Fib, Knapsack, LCS, CoinChange)
    │       └── bst_visualizer.hpp  # Legacy BST header (preserved)
    ├── src/
    │   ├── visualizer.cpp          # Visualizer implementations for all 6 data structures
    │   ├── compressx.cpp           # Huffman compression & decompression engine
    │   ├── graphkit.cpp            # Custom Graph, DSU, Dijkstra Heap, Quicksort
    │   ├── graph_visualizer.cpp    # 2D graph renderer and path highlighter
    │   ├── dplab.cpp               # Dynamic programming solvers and state matrix generators
    │   └── bst_visualizer.cpp      # Preserved BST renderer implementation
    ├── tests/
    │   └── test_main.cpp           # Comprehensive 91-assertion automated test suite
    └── sample_files/
        ├── sample_text.txt         # Sample text file for Huffman compression testing
        └── sample_binary.bin       # Sample 256-byte binary pattern test file
```

---

## 8. Build & Execution Instructions

### Prerequisites
- Windows 10/11
- Qt 6.12.0 installed at `C:\Qt\6.12.0\mingw_64`
- MinGW 13.1.0 (64-bit) installed at `C:\Qt\Tools\mingw1310_64`
- Ninja installed at `C:\Qt\Tools\Ninja`
- CMake 3.21+

### Step-by-Step Build Commands
Open PowerShell and configure the environment:

```powershell
# 1. Set environment paths
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\Ninja;" + $env:PATH

# 2. Navigate to project root build directory
cd AlgoLib
if (-not (Test-Path build)) { mkdir build }
cd build

# 3. Configure build files with CMake and Ninja
cmake -G "Ninja" ..

# 4. Compile the GUI application and test suite
ninja
```

### Running the Application
```powershell
.\AlgoLibApp.exe
```

### Running the Automated Test Suite
```powershell
.\AlgoLibTests.exe
```

---

## 9. Automated Testing & Verification Report

The automated test runner (`tests/test_main.cpp`) validates all normal, boundary, and edge conditions:

```
========================================================
AlgoLib - Comprehensive Automated ADSA Test Runner
========================================================

TEST SUITE 1: CORE DATA STRUCTURES
  [PASS] Dynamic Array starts empty
  [PASS] Initial size is 0
  [PASS] Dynamic Array throws on empty remove
  [PASS] Array size is 3 after 3 inserts
  [PASS] Array elements match inserted values
  [PASS] Array search works
  [PASS] Insert at index 1 works
  [PASS] Remove at index 1 works
  [PASS] Remove back works
  [PASS] Dynamic Array deep copy works independently
  [PASS] Linked List starts empty
  [PASS] Linked List size is 3
  [PASS] Head and tail inserts correct
  [PASS] List search works
  [PASS] Remove middle node works
  [PASS] Remove head node works
  [PASS] Linked List deep copy works
  [PASS] Stack starts empty
  [PASS] Stack pop throws underflow when empty
  [PASS] Stack size is 3
  [PASS] Stack peek returns top (33)
  [PASS] Stack pop returns 33
  [PASS] Stack pop returns 22
  [PASS] Stack size is 1 with top 11
  [PASS] Queue starts empty
  [PASS] Queue dequeue throws underflow when empty
  [PASS] Queue size is 3
  [PASS] Queue front is 101
  [PASS] Queue FIFO dequeue is 101
  [PASS] New front is 102
  [PASS] BST starts empty
  [PASS] BST has 7 distinct nodes
  [PASS] BST search works
  [PASS] BST node count remains 7 on duplicate insert
  [PASS] BST total element count is 8 with duplicate
  [PASS] BST Inorder is sorted with duplicates
  [PASS] Removing duplicate decrements count
  [PASS] Leaf node deletion (0 children) works
  [PASS] Single child node deletion works
  [PASS] Two-children root deletion works
  [PASS] Min Heap starts empty
  [PASS] Min Heap throws underflow when empty
  [PASS] Min Heap size is 5
  [PASS] Min Heap minimum is 5
  [PASS] extractMin returns 5
  [PASS] extractMin returns 10
  [PASS] extractMin returns 20
  [PASS] extractMin returns 30
  [PASS] extractMin returns 40
  [PASS] Min Heap empty after extracting all

TEST SUITE 2: COMPRESSX HUFFMAN ENGINE
  [PASS] Text file compressed successfully
  [PASS] Compressed archive non-empty
  [PASS] Text file decompressed successfully
  [PASS] Decompressed size matches original size
  [PASS] VERIFIED: Bit-for-bit exact match on text file round-trip
  [PASS] Binary file compressed successfully
  [PASS] Binary file decompressed successfully
  [PASS] VERIFIED: Bit-for-bit exact match on binary file round-trip
  [PASS] Empty file compression handled safely
  [PASS] Empty file decompression handled safely
  [PASS] VERIFIED: Empty file bit-for-bit match

TEST SUITE 3: GRAPHKIT ALGORITHMS
  [PASS] Sample graph has 6 vertices
  [PASS] Sample graph has 9 undirected edges
  [PASS] BFS visited all 6 vertices
  [PASS] BFS detected 1 connected component
  [PASS] DFS visited all 6 vertices
  [PASS] DFS detected 1 connected component
  [PASS] Dijkstra found path from A to F
  [PASS] Dijkstra shortest distance A->F is 13
  [PASS] Kruskal confirms connected MST
  [PASS] MST has V-1 = 5 edges
  [PASS] MST total weight is 13
  [PASS] Disconnected graph has 7 vertices
  [PASS] BFS with visitAllComponents visits all components
  [PASS] BFS detects 2 disconnected components
  [PASS] Dijkstra correctly detects unreachable disconnected target
  [PASS] Kruskal correctly detects disconnected graph (Forest)
  [PASS] Kruskal reports 2 spanning components

TEST SUITE 4: DYNAMIC PROGRAMMING LAB
  [PASS] Fibonacci F(10) == 55
  [PASS] Fibonacci F(0) == 0
  [PASS] Fibonacci F(1) == 1
  [PASS] Fibonacci F(20) == 6765
  [PASS] 0/1 Knapsack optimal value is 7
  [PASS] 0/1 Knapsack total weight used is 5
  [PASS] 0/1 Knapsack selected exactly 2 items
  [PASS] LCS length is 4 (e.g. BDAB or BCAB)
  [PASS] Reconstructed LCS string length is 4
  [PASS] Coin change 11 with [1,2,5] is possible
  [PASS] Minimum coins for 11 is 3 (5 + 5 + 1)
  [PASS] Coin change 3 with [2] is impossible
  [PASS] Returns -1 for impossible amount

========================================================
TEST EXECUTION SUMMARY
========================================================
Total Assertions Passed: 91
Total Assertions Failed: 0

>>> ALL TESTS PASSED SUCCESSFULLY! <<<
```

---

## 10. Screenshots Section

| Module | Feature Showcase |
| :--- | :--- |
| **Data Structures** | Real-time memory slots for Dynamic Array with capacity doubling, linked nodes, vertical stack, horizontal queue, BST with duplicate badges, and complete Min Heap with tree & array synchronisation. |
| **CompressX** | File browser, character frequency breakdown table, Huffman prefix code generation, archive creation, and byte-for-byte exact verification badge. |
| **GraphKit** | Dynamic vertex & edge creator, sample network presets, interactive circular graph canvas with highlighted Dijkstra shortest path (cyan) and Kruskal MST edges (emerald). |
| **DP Lab** | Interactive input forms for Fibonacci, 0/1 Knapsack, LCS, and Coin Change; fully styled 2D state matrix table with colored optimal cells and step-by-step traceback logs. |

---

## 11. Academic Integrity & Non-STL Compliance

In full accordance with ADSA academic integrity guidelines:
1. **Zero STL Container Rule:** Containers (`std::vector`, `std::list`, `std::map`, `std::queue`, `std::stack`, `std::set`, `std::priority_queue`, etc.) are **not used** for core data structures or algorithms.
2. **Explicit Memory Deallocation:** All dynamically allocated memory structures define explicit destructors and clean-up routines to prevent leaks.
3. **No External Black-Box Algorithms:** Algorithms (`std::sort`, graph solvers, compression libraries) are replaced with bespoke C++ implementations (`customSortEdges`, `DisjointSet`, `DijkstraHeap`, `BitWriter`/`BitReader`).

## 🚀 How to Download, Build and Run AlgoLib

Follow these steps to run AlgoLib on a Windows computer.

### 1. Download the Project

**Option A: Download ZIP**
1. Open the [AlgoLib GitHub Repository](https://github.com/baidbhavya9a-ux/AlgoLib).
2. Click the green **Code** button.
3. Select **Download ZIP**.
4. Extract the downloaded ZIP file.

**Option B: Clone using Git**

Open a terminal and run:

```bash
git clone https://github.com/baidbhavya9a-ux/AlgoLib.git
```

### 2. Install the Required Software

Make sure the following tools are installed:

- **Visual Studio Code** – Code editor.
- **Qt 6.12.0 (MinGW 64-bit)** – GUI framework.
- **MinGW GCC 13.1.0** – C++ compiler.
- **CMake** – Build configuration tool.
- **Ninja** – Build tool.

### 3. Open the Project in VS Code

1. Open Visual Studio Code.
2. Select **File → Open Folder**.
3. Choose the extracted `AlgoLib` project folder.
4. Open the integrated terminal using **Terminal → New Terminal**.

### 4. Configure the Qt Environment

In the PowerShell terminal, run:

```powershell
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\mingw1310_64\x86_64-w64-mingw32\bin;C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\Ninja;$env:PATH"
```

**Note:** This command assumes Qt and MinGW are installed in the paths shown above. If they are installed elsewhere, update the paths accordingly.

### 5. Configure and Build the Project

Run these commands from the project root directory:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

Wait for the build to complete successfully.

### 6. Launch AlgoLib

Run:

```powershell
.\build\AlgoLibApp.exe
```

The AlgoLib desktop application should now launch.

### 7. Run Automated Tests (Optional)

To execute the test suite, run:

```powershell
.\build\AlgoLibTests.exe
```

The reported test result for the project is **91 assertions passed and 0 failed**.

---

### Important Notes

- This is a C++ desktop application, not a website. It cannot be launched directly from the GitHub repository in a browser.
- The project must be built on the computer before running it.
- The `build` folder is excluded from GitHub, so you need to generate it using the CMake commands above.
- Make sure the required Qt libraries and compiler toolchain are installed.

**Project Repository:** https://github.com/baidbhavya9a-ux/AlgoLib   

