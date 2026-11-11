
#ifndef ALGOLIB_BST_VISUALIZER_HPP
#define ALGOLIB_BST_VISUALIZER_HPP

#include <QWidget>
#include <QVector>
#include <QPointF>

#include "algolib/data_structures.hpp"

class BSTVisualizer : public QWidget {

private:
    const algolib::BST* tree;

    void assignPositions(
        const algolib::BST::VisualNode* nodes,
        int index,
        int depth,
        int& nextX,
        QVector<QPointF>& positions
    );

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit BSTVisualizer(QWidget* parent = nullptr);

    void setTree(const algolib::BST* bst);
};

#endif
