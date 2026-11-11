#include "algolib/graph_visualizer.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <cmath>

namespace algolib {

GraphVisualizer::GraphVisualizer(QWidget* parent)
    : QWidget(parent), graph(nullptr), edgeHighlightColor("#38BDF8")
{
    setMinimumSize(420, 280);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GraphVisualizer::setGraph(const Graph* g) {
    graph = g;
    clearHighlights();
    update();
}

void GraphVisualizer::clearHighlights() {
    highlightedVertices.clear();
    highlightedEdges.clear();
    update();
}

void GraphVisualizer::highlightPath(const int* path, int length, const QColor& color) {
    clearHighlights();
    edgeHighlightColor = color;
    if (!path || length <= 0) return;

    for (int i = 0; i < length; ++i) {
        highlightedVertices.push_back(path[i]);
    }
    for (int i = 0; i + 1 < length; ++i) {
        highlightedEdges.push_back({path[i], path[i + 1], 0});
    }
    update();
}

void GraphVisualizer::highlightMST(const GraphEdge* edges, int count, const QColor& color) {
    clearHighlights();
    edgeHighlightColor = color;
    if (!edges || count <= 0) return;

    for (int i = 0; i < count; ++i) {
        highlightedEdges.push_back(edges[i]);
        if (!highlightedVertices.contains(edges[i].u))
            highlightedVertices.push_back(edges[i].u);
        if (!highlightedVertices.contains(edges[i].v))
            highlightedVertices.push_back(edges[i].v);
    }
    update();
}

void GraphVisualizer::highlightTraversal(const int* visited, int count, const QColor& color) {
    clearHighlights();
    edgeHighlightColor = color;
    if (!visited || count <= 0) return;

    for (int i = 0; i < count; ++i) {
        highlightedVertices.push_back(visited[i]);
    }
    update();
}

QPointF GraphVisualizer::getVertexPosition(int index, int total) const {
    if (total <= 0) return QPointF(width() / 2.0, height() / 2.0);

    qreal centerX = width() / 2.0;
    qreal centerY = height() / 2.0;
    qreal radius = qMin(centerX, centerY) - 45.0;
    if (radius < 40.0) radius = 40.0;

    qreal angle = (2.0 * M_PI * index) / total - (M_PI / 2.0); // Start from top
    qreal x = centerX + radius * std::cos(angle);
    qreal y = centerY + radius * std::sin(angle);
    return QPointF(x, y);
}

void GraphVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#171725"));

    if (!graph || graph->getVertexCount() == 0) {
        painter.setPen(QColor("#A0A0B5"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(rect(), Qt::AlignCenter, "Graph is empty.\nAdd vertices and edges or load a preset to visualize.");
        return;
    }

    int n = graph->getVertexCount();
    // Dynamically scale node radius based on vertex count
    const qreal nodeRadius = (n <= 8) ? 22.0 : qMax(14.0, 180.0 / n);

    // Collect all edges
    int totalEdges = graph->getTotalEdgeCount();
    GraphEdge* edges = new GraphEdge[totalEdges > 0 ? totalEdges : 1];
    int actualEdges = graph->getAllEdges(edges, totalEdges);

    // 1. Draw edges
    for (int i = 0; i < actualEdges; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        QPointF p1 = getVertexPosition(u, n);
        QPointF p2 = getVertexPosition(v, n);

        // Check if edge is highlighted
        bool isHighEdge = false;
        for (const auto& he : highlightedEdges) {
            if ((he.u == u && he.v == v) || (!graph->isDirected() && he.u == v && he.v == u)) {
                isHighEdge = true;
                break;
            }
        }

        if (isHighEdge) {
            painter.setPen(QPen(edgeHighlightColor, 3.5, Qt::SolidLine, Qt::RoundCap));
        } else {
            painter.setPen(QPen(QColor("#475569"), 1.8, Qt::SolidLine, Qt::RoundCap));
        }

        if (u == v) {
            // Self-loop: draw loop circle above vertex
            qreal loopR = nodeRadius * 1.2;
            QPointF loopCenter(p1.x(), p1.y() - nodeRadius - loopR / 2.0);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(loopCenter, loopR, loopR);

            // Weight badge
            painter.setPen(Qt::NoPen);
            painter.setBrush(isHighEdge ? QColor("#1E293B") : QColor("#0F172A"));
            painter.drawRoundedRect(QRectF(loopCenter.x() - 14, loopCenter.y() - loopR - 6, 28, 18), 4, 4);

            painter.setPen(isHighEdge ? edgeHighlightColor : QColor("#94A3B8"));
            painter.setFont(QFont("Segoe UI", 8, QFont::Bold));
            painter.drawText(QRectF(loopCenter.x() - 14, loopCenter.y() - loopR - 6, 28, 18), Qt::AlignCenter, QString::number(w));
            continue;
        }

        painter.drawLine(p1, p2);

        // If directed, draw arrowhead at target boundary
        if (graph->isDirected()) {
            qreal dx = p2.x() - p1.x();
            qreal dy = p2.y() - p1.y();
            qreal len = std::sqrt(dx * dx + dy * dy);
            if (len > 0.001) {
                QPointF targetEdge(p2.x() - (dx / len) * nodeRadius, p2.y() - (dy / len) * nodeRadius);
                qreal arrowSize = 10.0;
                QPointF arrowP1(targetEdge.x() - (dx / len) * arrowSize + (dy / len) * (arrowSize / 2.0),
                                targetEdge.y() - (dy / len) * arrowSize - (dx / len) * (arrowSize / 2.0));
                QPointF arrowP2(targetEdge.x() - (dx / len) * arrowSize - (dy / len) * (arrowSize / 2.0),
                                targetEdge.y() - (dy / len) * arrowSize + (dx / len) * (arrowSize / 2.0));
                QPolygonF arrowHead;
                arrowHead << targetEdge << arrowP1 << arrowP2;
                painter.setBrush(isHighEdge ? edgeHighlightColor : QColor("#64748B"));
                painter.drawPolygon(arrowHead);
            }
        }

        // Draw edge weight badge at midpoint
        QPointF mid = (p1 + p2) / 2.0;
        qreal dx = p2.x() - p1.x();
        qreal dy = p2.y() - p1.y();
        qreal len = std::sqrt(dx * dx + dy * dy);
        QPointF offset(0, 0);
        if (len > 0.001) {
            offset = QPointF(-dy / len * 12.0, dx / len * 12.0);
        }
        QPointF textPt = mid + offset;

        // Weight pill
        painter.setPen(Qt::NoPen);
        painter.setBrush(isHighEdge ? QColor("#1E293B") : QColor("#0F172A"));
        painter.drawRoundedRect(QRectF(textPt.x() - 14, textPt.y() - 10, 28, 20), 4, 4);

        painter.setPen(isHighEdge ? edgeHighlightColor : QColor("#94A3B8"));
        painter.setFont(QFont("Segoe UI", 8, QFont::Bold));
        painter.drawText(QRectF(textPt.x() - 14, textPt.y() - 10, 28, 20), Qt::AlignCenter, QString::number(w));
    }

    delete[] edges;

    // 2. Draw vertices
    for (int i = 0; i < n; ++i) {
        QPointF pt = getVertexPosition(i, n);
        bool isHighVertex = highlightedVertices.contains(i);

        if (isHighVertex) {
            painter.setPen(QPen(edgeHighlightColor, 3));
            painter.setBrush(QColor("#1E3A8A"));
        } else {
            painter.setPen(QPen(QColor("#3B82F6"), 2));
            painter.setBrush(QColor("#1E293B"));
        }

        painter.drawEllipse(pt, nodeRadius, nodeRadius);

        // Vertex label
        painter.setPen(isHighVertex ? edgeHighlightColor : Qt::white);
        int fontSize = (n <= 12) ? 10 : 8;
        painter.setFont(QFont("Segoe UI", fontSize, QFont::Bold));
        painter.drawText(QRectF(pt.x() - nodeRadius, pt.y() - nodeRadius, nodeRadius * 2, nodeRadius * 2),
                         Qt::AlignCenter, QString::fromStdString(graph->getVertexLabel(i)));
    }
}

} // namespace algolib
