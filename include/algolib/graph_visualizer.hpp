#ifndef ALGOLIB_GRAPH_VISUALIZER_HPP
#define ALGOLIB_GRAPH_VISUALIZER_HPP

#include <QWidget>
#include <QVector>
#include <QPointF>
#include "algolib/graphkit.hpp"

namespace algolib {

class GraphVisualizer : public QWidget {
private:
    const Graph* graph = nullptr;

    // Highlights
    QVector<int> highlightedVertices;
    QVector<GraphEdge> highlightedEdges;
    QColor edgeHighlightColor;

    QPointF getVertexPosition(int index, int total) const;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit GraphVisualizer(QWidget* parent = nullptr);

    void setGraph(const Graph* g);
    void clearHighlights();
    void highlightPath(const int* path, int length, const QColor& color = QColor("#38BDF8"));
    void highlightMST(const GraphEdge* edges, int count, const QColor& color = QColor("#10B981"));
    void highlightTraversal(const int* visited, int count, const QColor& color = QColor("#F59E0B"));
};

} // namespace algolib

#endif // ALGOLIB_GRAPH_VISUALIZER_HPP

