#ifndef ALGOLIB_VISUALIZER_HPP
#define ALGOLIB_VISUALIZER_HPP

#include <QWidget>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVector>
#include <QPointF>
#include "algolib/data_structures.hpp"

namespace algolib {

// 1. Dynamic Array Visualizer
class ArrayVisualizer : public QWidget {
private:
    const DynamicArray* array = nullptr;
    int highlightedIndex = -1;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit ArrayVisualizer(QWidget* parent = nullptr);
    void setArray(const DynamicArray* arr, int highlight = -1);
};

// 2. Linked List Visualizer
class LinkedListVisualizer : public QWidget {
private:
    const LinkedList* list = nullptr;
    int highlightedIndex = -1;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit LinkedListVisualizer(QWidget* parent = nullptr);
    void setList(const LinkedList* l, int highlight = -1);
};

// 3. Stack Visualizer (Vertical LIFO)
class StackVisualizer : public QWidget {
private:
    const Stack* stack = nullptr;
    int highlightedIndex = -1;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit StackVisualizer(QWidget* parent = nullptr);
    void setStack(const Stack* s, int highlight = -1);
};

// 4. Queue Visualizer (Horizontal FIFO)
class QueueVisualizer : public QWidget {
private:
    const Queue* queue = nullptr;
    int highlightedIndex = -1;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit QueueVisualizer(QWidget* parent = nullptr);
    void setQueue(const Queue* q, int highlight = -1);
};

// 5. BST Visualizer (Hierarchical tree)
class BSTVisualizerWidget : public QWidget {
private:
    const BST* tree = nullptr;

    void assignPositions(
        const BST::VisualNode* nodes,
        int index,
        int depth,
        int& nextX,
        QVector<QPointF>& positions
    );

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit BSTVisualizerWidget(QWidget* parent = nullptr);
    void setTree(const BST* bst);
};

// 6. Min Heap Visualizer (Binary tree + array layout)
class HeapVisualizer : public QWidget {
private:
    const MinHeap* heap = nullptr;
    int highlightedIndex = -1;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit HeapVisualizer(QWidget* parent = nullptr);
    void setHeap(const MinHeap* h, int highlight = -1);
};

// Unified Data Structure Canvas managing all 6 visualizers inside a scroll area
class UnifiedDSVisualizer : public QWidget {
private:
    QStackedWidget* stackedWidget;
    QScrollArea* scrollArea;

    ArrayVisualizer* arrayVis;
    LinkedListVisualizer* listVis;
    StackVisualizer* stackVis;
    QueueVisualizer* queueVis;
    BSTVisualizerWidget* bstVis;
    HeapVisualizer* heapVis;

public:
    explicit UnifiedDSVisualizer(QWidget* parent = nullptr);

    void setMode(int index); // 0=Array, 1=List, 2=Stack, 3=Queue, 4=BST, 5=Heap

    void updateArray(const DynamicArray* arr, int highlight = -1);
    void updateList(const LinkedList* l, int highlight = -1);
    void updateStack(const Stack* s, int highlight = -1);
    void updateQueue(const Queue* q, int highlight = -1);
    void updateBST(const BST* bst);
    void updateHeap(const MinHeap* h, int highlight = -1);
};

} // namespace algolib

#endif // ALGOLIB_VISUALIZER_HPP

