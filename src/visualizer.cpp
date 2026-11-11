#include "algolib/visualizer.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QVBoxLayout>
#include <cmath>

namespace algolib {

// ==================================================
// 1. ARRAY VISUALIZER
// ==================================================
ArrayVisualizer::ArrayVisualizer(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void ArrayVisualizer::setArray(const DynamicArray* arr, int highlight) {
    array = arr;
    highlightedIndex = highlight;

    std::size_t totalSlots = (array ? (array->capacity() > 0 ? array->capacity() : 0) : 0);
    int neededWidth = qMax(500, static_cast<int>(totalSlots * 70 + 80));
    setMinimumSize(neededWidth, 220);
    resize(neededWidth, 220);
    update();
}

void ArrayVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!array || array->empty()) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Dynamic Array is empty.\nInsert values to visualize contiguous memory allocation.");
        return;
    }

    std::size_t size = array->size();
    std::size_t cap = array->capacity();

    // Stats banner
    painter.setPen(QColor("#60A5FA"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(30, 30, QString("DYNAMIC ARRAY  |  Size: %1  |  Capacity: %2  |  Element Size: %3 bytes")
        .arg(size).arg(cap).arg(sizeof(int)));

    const int cellWidth = 62;
    const int cellHeight = 55;
    const int startX = 35;
    const int startY = 70;

    for (std::size_t i = 0; i < cap; ++i) {
        int x = startX + static_cast<int>(i) * (cellWidth + 8);
        int y = startY;

        // Draw index label above cell
        painter.setPen(QColor("#94A3B8"));
        painter.setFont(QFont("Segoe UI", 9));
        painter.drawText(QRect(x, y - 22, cellWidth, 20), Qt::AlignCenter, QString("[%1]").arg(i));

        if (i < size) {
            // Occupied slot
            bool isHigh = (static_cast<int>(i) == highlightedIndex);
            painter.setPen(QPen(isHigh ? QColor("#FBBF24") : QColor("#3B82F6"), 2));
            painter.setBrush(isHigh ? QColor("#453414") : QColor("#1E293B"));
            painter.drawRoundedRect(x, y, cellWidth, cellHeight, 6, 6);

            // Draw value
            painter.setPen(isHigh ? QColor("#FBBF24") : Qt::white);
            painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
            painter.drawText(QRect(x, y, cellWidth, cellHeight), Qt::AlignCenter, QString::number(array->at(i)));
        } else {
            // Unused allocated capacity slot
            QPen dashedPen(QColor("#475569"), 1, Qt::DashLine);
            painter.setPen(dashedPen);
            painter.setBrush(QColor("#0F172A"));
            painter.drawRoundedRect(x, y, cellWidth, cellHeight, 6, 6);

            painter.setPen(QColor("#475569"));
            QFont emptyFont("Segoe UI", 9);
            emptyFont.setItalic(true);
            painter.setFont(emptyFont);
            painter.drawText(QRect(x, y, cellWidth, cellHeight), Qt::AlignCenter, "empty");
        }
    }

    // Legend
    painter.setPen(QColor("#64748B"));
    painter.setFont(QFont("Segoe UI", 9));
    painter.drawText(35, 175, "Solid blue = stored elements in memory  |  Dashed gray = allocated capacity awaiting insertion");
}


// ==================================================
// 2. LINKED LIST VISUALIZER
// ==================================================
LinkedListVisualizer::LinkedListVisualizer(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void LinkedListVisualizer::setList(const LinkedList* l, int highlight) {
    list = l;
    highlightedIndex = highlight;

    std::size_t count = (list ? list->size() : 0);
    int neededWidth = qMax(500, static_cast<int>(count * 130 + 160));
    setMinimumSize(neededWidth, 220);
    resize(neededWidth, 220);
    update();
}

void LinkedListVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!list || list->empty()) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Singly Linked List is empty.\nInsert values to create nodes connected via next pointers.");
        return;
    }

    std::size_t count = list->size();

    // Stats banner
    painter.setPen(QColor("#34D399"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(30, 30, QString("SINGLY LINKED LIST  |  Total Nodes: %1  |  Pointer Size: %2 bytes")
        .arg(count).arg(sizeof(void*)));

    int startX = 35;
    int startY = 80;
    const int nodeWidth = 84;
    const int nodeHeight = 48;
    const int gap = 46;

    // Draw HEAD pointer badge
    painter.setPen(QColor("#FBBF24"));
    painter.setBrush(QColor("#292524"));
    painter.drawRoundedRect(startX, startY + 8, 48, 32, 4, 4);
    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
    painter.drawText(QRect(startX, startY + 8, 48, 32), Qt::AlignCenter, "HEAD");

    // Arrow from HEAD to first node
    painter.setPen(QPen(QColor("#FBBF24"), 2));
    int curX = startX + 48;
    int curY = startY + 24;
    painter.drawLine(curX, curY, curX + 25, curY);
    painter.drawLine(curX + 25, curY, curX + 18, curY - 5);
    painter.drawLine(curX + 25, curY, curX + 18, curY + 5);

    curX += 25;

    for (std::size_t i = 0; i < count; ++i) {
        bool isHigh = (static_cast<int>(i) == highlightedIndex);

        // Draw node box (Data compartment + Next pointer compartment)
        int x = curX;
        int y = startY;

        // Node outline
        painter.setPen(QPen(isHigh ? QColor("#FBBF24") : QColor("#10B981"), 2));
        painter.setBrush(isHigh ? QColor("#453414") : QColor("#064E3B"));
        painter.drawRoundedRect(x, y, nodeWidth, nodeHeight, 6, 6);

        // Compartment divider
        int divX = x + 52;
        painter.drawLine(divX, y, divX, y + nodeHeight);

        // Data value
        painter.setPen(isHigh ? QColor("#FBBF24") : Qt::white);
        painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter.drawText(QRect(x, y, 52, nodeHeight), Qt::AlignCenter, QString::number(list->at(i)));

        // Next pointer bullet
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#34D399"));
        painter.drawEllipse(divX + 12, y + nodeHeight / 2 - 4, 8, 8);

        // Arrow to next or NULL
        int arrowStartX = x + nodeWidth;
        int arrowY = y + nodeHeight / 2;

        painter.setPen(QPen(QColor("#6EE7B7"), 2));
        painter.drawLine(arrowStartX, arrowY, arrowStartX + gap, arrowY);
        painter.drawLine(arrowStartX + gap, arrowY, arrowStartX + gap - 8, arrowY - 5);
        painter.drawLine(arrowStartX + gap, arrowY, arrowStartX + gap - 8, arrowY + 5);

        curX = arrowStartX + gap;
    }

    // Draw NULL terminator
    painter.setPen(QColor("#EF4444"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(QRect(curX + 6, startY, 55, nodeHeight), Qt::AlignCenter, "NULL");
}


// ==================================================
// 3. STACK VISUALIZER
// ==================================================
StackVisualizer::StackVisualizer(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void StackVisualizer::setStack(const Stack* s, int highlight) {
    stack = s;
    highlightedIndex = highlight;

    int count = (stack ? stack->size() : 0);
    int neededHeight = qMax(260, count * 45 + 120);
    setMinimumSize(500, neededHeight);
    resize(500, neededHeight);
    update();
}

void StackVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!stack || stack->empty()) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Stack is empty.\nPush values to visualize Last-In First-Out (LIFO) stack operations.");
        return;
    }

    int count = stack->size();

    // Stats banner
    painter.setPen(QColor("#A78BFA"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(30, 30, QString("STACK (LIFO)  |  Size: %1  |  Top Element: %2")
        .arg(count).arg(stack->peek()));

    int centerX = width() / 2;
    const int itemWidth = 140;
    const int itemHeight = 36;
    const int startY = 65;

    // Draw elements from Top (index 0) down to Bottom
    for (int i = 0; i < count; ++i) {
        int x = centerX - itemWidth / 2;
        int y = startY + i * (itemHeight + 6);

        bool isTop = (i == 0);
        bool isHigh = (i == highlightedIndex);

        painter.setPen(QPen(isHigh ? QColor("#FBBF24") : (isTop ? QColor("#C084FC") : QColor("#818CF8")), 2));
        painter.setBrush(isHigh ? QColor("#453414") : (isTop ? QColor("#4C1D95") : QColor("#312E81")));
        painter.drawRoundedRect(x, y, itemWidth, itemHeight, 6, 6);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter.drawText(QRect(x, y, itemWidth, itemHeight), Qt::AlignCenter, QString::number(stack->at(i)));

        if (isTop) {
            // Draw TOP pointer indicator
            painter.setPen(QColor("#F43F5E"));
            painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
            painter.drawText(QRect(x - 85, y, 75, itemHeight), Qt::AlignRight | Qt::AlignVCenter, "TOP ➔");
        }
    }

    // Draw container base line
    int bottomY = startY + count * (itemHeight + 6) + 4;
    painter.setPen(QPen(QColor("#64748B"), 3));
    painter.drawLine(centerX - itemWidth / 2 - 15, bottomY, centerX + itemWidth / 2 + 15, bottomY);
    painter.setPen(QColor("#64748B"));
    painter.setFont(QFont("Segoe UI", 8));
    painter.drawText(QRect(centerX - 50, bottomY + 2, 100, 20), Qt::AlignCenter, "BOTTOM");
}


// ==================================================
// 4. QUEUE VISUALIZER
// ==================================================
QueueVisualizer::QueueVisualizer(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void QueueVisualizer::setQueue(const Queue* q, int highlight) {
    queue = q;
    highlightedIndex = highlight;

    int count = (queue ? queue->size() : 0);
    int neededWidth = qMax(500, count * 80 + 200);
    setMinimumSize(neededWidth, 220);
    resize(neededWidth, 220);
    update();
}

void QueueVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!queue || queue->empty()) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Queue is empty.\nEnqueue values to visualize First-In First-Out (FIFO) queue operations.");
        return;
    }

    int count = queue->size();

    // Stats banner
    painter.setPen(QColor("#38BDF8"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(30, 30, QString("QUEUE (FIFO)  |  Size: %1  |  Front Element: %2")
        .arg(count).arg(queue->front()));

    int startX = 110;
    int startY = 85;
    const int itemWidth = 62;
    const int itemHeight = 52;
    const int gap = 12;

    // Draw FRONT label & arrow
    painter.setPen(QColor("#38BDF8"));
    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
    painter.drawText(QRect(15, startY + 12, 70, 30), Qt::AlignRight | Qt::AlignVCenter, "FRONT ➔");

    for (int i = 0; i < count; ++i) {
        int x = startX + i * (itemWidth + gap);
        int y = startY;

        bool isFront = (i == 0);
        bool isRear = (i == count - 1);
        bool isHigh = (i == highlightedIndex);

        painter.setPen(QPen(isHigh ? QColor("#FBBF24") : (isFront ? QColor("#38BDF8") : QColor("#0284C7")), 2));
        painter.setBrush(isHigh ? QColor("#453414") : (isFront ? QColor("#075985") : QColor("#0C4A6E")));
        painter.drawRoundedRect(x, y, itemWidth, itemHeight, 6, 6);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter.drawText(QRect(x, y, itemWidth, itemHeight), Qt::AlignCenter, QString::number(queue->at(i)));

        // Index tag
        painter.setPen(QColor("#94A3B8"));
        painter.setFont(QFont("Segoe UI", 8));
        painter.drawText(QRect(x, y + itemHeight + 2, itemWidth, 18), Qt::AlignCenter, QString("[%1]").arg(i));

        if (isRear) {
            // Draw REAR label & arrow
            painter.setPen(QColor("#F472B6"));
            painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
            painter.drawText(QRect(x + itemWidth + 8, startY + 12, 80, 30), Qt::AlignLeft | Qt::AlignVCenter, "⬅ REAR");
        }
    }
}


// ==================================================
// 5. BST VISUALIZER WIDGET
// ==================================================
BSTVisualizerWidget::BSTVisualizerWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void BSTVisualizerWidget::setTree(const BST* bst) {
    tree = bst;

    if (!tree || tree->size() == 0) {
        setMinimumSize(500, 260);
        update();
        return;
    }

    int count = tree->size();
    auto* nodes = new BST::VisualNode[count];
    tree->exportVisualNodes(nodes, count);

    QVector<QPointF> positions(count);
    int nextX = 0;
    assignPositions(nodes, 0, 0, nextX, positions);

    int maxDepth = 0;
    for (int i = 0; i < count; ++i) {
        maxDepth = qMax(maxDepth, static_cast<int>(positions[i].y()));
    }

    int canvasWidth = qMax(500, count * 75);
    int canvasHeight = qMax(260, (maxDepth + 1) * 75 + 80);

    setMinimumSize(canvasWidth, canvasHeight);
    resize(canvasWidth, canvasHeight);

    delete[] nodes;
    update();
}

void BSTVisualizerWidget::assignPositions(
    const BST::VisualNode* nodes,
    int index,
    int depth,
    int& nextX,
    QVector<QPointF>& positions)
{
    if (index < 0) return;

    const auto& node = nodes[index];
    assignPositions(nodes, node.left, depth + 1, nextX, positions);
    positions[index] = QPointF(nextX++, depth);
    assignPositions(nodes, node.right, depth + 1, nextX, positions);
}

void BSTVisualizerWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!tree || tree->size() == 0) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "BST is empty.\nInsert values to build and visualize the binary search tree.");
        return;
    }

    const int count = tree->size();
    auto* nodes = new BST::VisualNode[count];
    tree->exportVisualNodes(nodes, count);

    QVector<QPointF> positions(count);
    int nextX = 0;
    assignPositions(nodes, 0, 0, nextX, positions);

    const qreal margin = 35.0;
    const qreal nodeRadius = 20.0;
    const qreal levelGap = 75.0;
    const qreal availableWidth = qMax(1.0, width() - 2.0 * margin);
    const qreal horizontalGap = count > 1 ? availableWidth / (count - 1) : 0;

    auto screenPoint = [&](int index) {
        return QPointF(
            count > 1 ? margin + positions[index].x() * horizontalGap : width() / 2.0,
            45.0 + positions[index].y() * levelGap
        );
    };

    // Draw tree branches
    painter.setPen(QPen(QColor("#64748B"), 2));
    for (int i = 0; i < count; ++i) {
        QPointF parentPt = screenPoint(i);
        if (nodes[i].left >= 0)
            painter.drawLine(parentPt, screenPoint(nodes[i].left));
        if (nodes[i].right >= 0)
            painter.drawLine(parentPt, screenPoint(nodes[i].right));
    }

    // Draw tree nodes
    for (int i = 0; i < count; ++i) {
        QPointF center = screenPoint(i);

        painter.setPen(QPen(QColor("#60A5FA"), 2));
        painter.setBrush(QColor("#1E3A8A"));
        painter.drawEllipse(center, nodeRadius, nodeRadius);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 9, QFont::Bold));

        QString label = (nodes[i].count > 1)
            ? QString("%1 ×%2").arg(nodes[i].value).arg(nodes[i].count)
            : QString::number(nodes[i].value);

        painter.drawText(QRectF(center.x() - nodeRadius, center.y() - nodeRadius, nodeRadius * 2, nodeRadius * 2),
                         Qt::AlignCenter, label);
    }

    delete[] nodes;
}


// ==================================================
// 6. MIN HEAP VISUALIZER
// ==================================================
HeapVisualizer::HeapVisualizer(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void HeapVisualizer::setHeap(const MinHeap* h, int highlight) {
    heap = h;
    highlightedIndex = highlight;

    int count = (heap ? heap->size() : 0);
    int depth = (count > 0) ? static_cast<int>(std::floor(std::log2(count))) : 0;
    int neededWidth = qMax(500, static_cast<int>(std::pow(2, depth) * 75 + 100));
    int neededHeight = qMax(260, (depth + 1) * 70 + 130);

    setMinimumSize(neededWidth, neededHeight);
    resize(neededWidth, neededHeight);
    update();
}

void HeapVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!heap || heap->empty()) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Min Heap is empty.\nInsert values to visualize the binary min-heap property.");
        return;
    }

    int count = heap->size();

    // Stats banner
    painter.setPen(QColor("#F59E0B"));
    painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
    painter.drawText(30, 30, QString("MIN HEAP  |  Size: %1  |  Minimum Element: %2")
        .arg(count).arg(heap->at(0)));

    const qreal nodeRadius = 18.0;
    const qreal levelGap = 65.0;

    // Helper to calculate (x, y) coordinates for binary heap index
    auto getHeapPoint = [&](int index) {
        int level = static_cast<int>(std::floor(std::log2(index + 1)));
        int levelStartIndex = (1 << level) - 1;
        int indexInLevel = index - levelStartIndex;
        int nodesInLevel = 1 << level;

        qreal levelWidth = width();
        qreal x = (levelWidth / (nodesInLevel + 1)) * (indexInLevel + 1);
        qreal y = 60.0 + level * levelGap;
        return QPointF(x, y);
    };

    // Draw branch lines
    painter.setPen(QPen(QColor("#64748B"), 2));
    for (int i = 0; i < count; ++i) {
        QPointF parentPt = getHeapPoint(i);
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < count)
            painter.drawLine(parentPt, getHeapPoint(left));
        if (right < count)
            painter.drawLine(parentPt, getHeapPoint(right));
    }

    // Draw heap tree nodes
    for (int i = 0; i < count; ++i) {
        QPointF pt = getHeapPoint(i);
        bool isMin = (i == 0);
        bool isHigh = (i == highlightedIndex);

        painter.setPen(QPen(isHigh ? QColor("#FBBF24") : (isMin ? QColor("#10B981") : QColor("#F59E0B")), 2));
        painter.setBrush(isHigh ? QColor("#453414") : (isMin ? QColor("#065F46") : QColor("#78350F")));
        painter.drawEllipse(pt, nodeRadius, nodeRadius);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
        painter.drawText(QRectF(pt.x() - nodeRadius, pt.y() - nodeRadius, nodeRadius * 2, nodeRadius * 2),
                         Qt::AlignCenter, QString::number(heap->at(i)));
    }

    // Draw array representation at bottom
    int maxLevel = static_cast<int>(std::floor(std::log2(count)));
    int arrayY = static_cast<int>(60.0 + (maxLevel + 1) * levelGap + 25);

    painter.setPen(QColor("#CBD5E1"));
    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
    painter.drawText(30, arrayY - 10, "Underlying Array: [ ");

    const int cellW = 42;
    const int cellH = 30;
    int curX = 145;

    for (int i = 0; i < count; ++i) {
        painter.setPen(QPen(QColor("#D97706"), 1));
        painter.setBrush(QColor("#292524"));
        painter.drawRect(curX, arrayY - 22, cellW, cellH);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 9));
        painter.drawText(QRect(curX, arrayY - 22, cellW, cellH), Qt::AlignCenter, QString::number(heap->at(i)));

        // Index
        painter.setPen(QColor("#94A3B8"));
        painter.setFont(QFont("Segoe UI", 7));
        painter.drawText(QRect(curX, arrayY + 10, cellW, 16), Qt::AlignCenter, QString::number(i));

        curX += cellW + 4;
    }
    painter.setPen(QColor("#CBD5E1"));
    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
    painter.drawText(curX + 6, arrayY - 10, "]");
}


// ==================================================
// UNIFIED DATA STRUCTURE VISUALIZER
// ==================================================
UnifiedDSVisualizer::UnifiedDSVisualizer(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(260);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #171725;
            border: 1px solid #35354B;
            border-radius: 8px;
        }
    )");

    stackedWidget = new QStackedWidget;

    arrayVis = new ArrayVisualizer;
    listVis = new LinkedListVisualizer;
    stackVis = new StackVisualizer;
    queueVis = new QueueVisualizer;
    bstVis = new BSTVisualizerWidget;
    heapVis = new HeapVisualizer;

    stackedWidget->addWidget(arrayVis); // 0
    stackedWidget->addWidget(listVis);  // 1
    stackedWidget->addWidget(stackVis); // 2
    stackedWidget->addWidget(queueVis); // 3
    stackedWidget->addWidget(bstVis);   // 4
    stackedWidget->addWidget(heapVis);  // 5

    scrollArea->setWidget(stackedWidget);
    layout->addWidget(scrollArea);
}

void UnifiedDSVisualizer::setMode(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    }
}

void UnifiedDSVisualizer::updateArray(const DynamicArray* arr, int highlight) {
    arrayVis->setArray(arr, highlight);
}

void UnifiedDSVisualizer::updateList(const LinkedList* l, int highlight) {
    listVis->setList(l, highlight);
}

void UnifiedDSVisualizer::updateStack(const Stack* s, int highlight) {
    stackVis->setStack(s, highlight);
}

void UnifiedDSVisualizer::updateQueue(const Queue* q, int highlight) {
    queueVis->setQueue(q, highlight);
}

void UnifiedDSVisualizer::updateBST(const BST* bst) {
    bstVis->setTree(bst);
}

void UnifiedDSVisualizer::updateHeap(const MinHeap* h, int highlight) {
    heapVis->setHeap(h, highlight);
}

} // namespace algolib

