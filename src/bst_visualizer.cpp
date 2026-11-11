
#include "algolib/bst_visualizer.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPen>
#include <QBrush>

BSTVisualizer::BSTVisualizer(QWidget* parent)
    : QWidget(parent), tree(nullptr)
{
    setMinimumHeight(230);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}


void BSTVisualizer::setTree(const algolib::BST* bst)
{
    tree = bst;

    if (!tree || tree->size() == 0) {
        setMinimumSize(500, 260);
        update();
        return;
    }

    int count = tree->size();

    auto* nodes = new algolib::BST::VisualNode[count];

    tree->exportVisualNodes(nodes, count);

    QVector<QPointF> positions(count);

    int nextX = 0;

    assignPositions(nodes, 0, 0, nextX, positions);

    int maxDepth = 0;

    for (int i = 0; i < count; ++i) {
        maxDepth = qMax(
            maxDepth,
            static_cast<int>(positions[i].y())
        );
    }

    // Automatically expand the drawing area.
    int canvasWidth = qMax(500, count * 75);
    int canvasHeight = qMax(260, (maxDepth + 1) * 75 + 80);

    setMinimumSize(canvasWidth, canvasHeight);
    resize(canvasWidth, canvasHeight);

    delete[] nodes;

    update();
}


void BSTVisualizer::assignPositions(
    const algolib::BST::VisualNode* nodes,
    int index,
    int depth,
    int& nextX,
    QVector<QPointF>& positions)
{
    if (index < 0)
        return;

    const auto& node = nodes[index];

    // Position left subtree first.
    assignPositions(
        nodes, node.left, depth + 1, nextX, positions
    );

    // Assign this node's horizontal position.
    positions[index] = QPointF(nextX++, depth);

    // Position right subtree.
    assignPositions(
        nodes, node.right, depth + 1, nextX, positions
    );
}

void BSTVisualizer::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor("#171725"));

    painter.setPen(QColor("#A0A0B5"));
    painter.setFont(QFont("Segoe UI", 11));

    if (!tree || tree->size() == 0) {
        painter.drawText(
            rect(),
            Qt::AlignCenter,
            "BST is empty\nInsert values to build the tree."
        );
        return;
    }

    const int count = tree->size();

    auto* nodes = new algolib::BST::VisualNode[count];

    tree->exportVisualNodes(nodes, count);

    QVector<QPointF> positions(count);

    int nextX = 0;

    assignPositions(
        nodes, 0, 0, nextX, positions
    );

    const qreal margin = 35.0;
    const qreal nodeRadius = 19.0;
    const qreal levelGap = 75.0;

    const qreal availableWidth =
        qMax(1.0, width() - 2.0 * margin);

    const qreal horizontalGap =
        count > 1
        ? availableWidth / (count - 1)
        : 0;

    auto screenPoint = [&](int index) {
        return QPointF(
            count > 1
                ? margin + positions[index].x() * horizontalGap
                : width() / 2.0,
            40.0 + positions[index].y() * levelGap
        );
    };

    // Draw connections first so they appear behind nodes.
    painter.setPen(QPen(QColor("#64748B"), 2));

    for (int i = 0; i < count; ++i) {
        QPointF parent = screenPoint(i);

        if (nodes[i].left >= 0) {
            painter.drawLine(
                parent,
                screenPoint(nodes[i].left)
            );
        }

        if (nodes[i].right >= 0) {
            painter.drawLine(
                parent,
                screenPoint(nodes[i].right)
            );
        }
    }

    // Draw tree nodes.
    for (int i = 0; i < count; ++i) {
        QPointF center = screenPoint(i);

        painter.setPen(
            QPen(QColor("#60A5FA"), 2)
        );

        painter.setBrush(QColor("#263D59"));

        painter.drawEllipse(
            center,
            nodeRadius,
            nodeRadius
        );

        painter.setPen(Qt::white);
        painter.setFont(
            QFont("Segoe UI", 10, QFont::Bold)
        );

        painter.drawText(
            QRectF(
                center.x() - nodeRadius,
                center.y() - nodeRadius,
                nodeRadius * 2,
                nodeRadius * 2
            ),
            Qt::AlignCenter,
            nodes[i].count > 1
    ? QString("%1 ×%2")
          .arg(nodes[i].value)
          .arg(nodes[i].count)
    : QString::number(nodes[i].value)
        );
    }

    delete[] nodes;
}
