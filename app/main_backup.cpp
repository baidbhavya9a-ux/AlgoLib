
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "algolib/data_structures.hpp"
using namespace algolib;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    QMainWindow window;

    window.setWindowTitle("AlgoLib - DSA Toolkit");
    window.resize(900, 600);

    QTabWidget *tabs = new QTabWidget();

    // Data Structures tab
    QWidget *dataPage = new QWidget();
    QVBoxLayout *dataLayout = new QVBoxLayout(dataPage);

    QLabel *dataLabel = new QLabel(
        "Dynamic Array | Linked List | Stack | Queue | BST"
    );

    dataLayout->addWidget(dataLabel);

    // CompressX tab
    QWidget *compressPage = new QWidget();
    QVBoxLayout *compressLayout =
        new QVBoxLayout(compressPage);

    QLabel *compressLabel = new QLabel(
        "Huffman File Compression and Decompression"
    );

    compressLayout->addWidget(compressLabel);

    // GraphKit tab
    QWidget *graphPage = new QWidget();
    QVBoxLayout *graphLayout = new QVBoxLayout(graphPage);

    QLabel *graphLabel = new QLabel(
        "BFS | DFS | Dijkstra | Graph Algorithms"
    );

    graphLayout->addWidget(graphLabel);

    // Add all three tabs
    tabs->addTab(dataPage, "Data Structures");
    tabs->addTab(compressPage, "CompressX");
    tabs->addTab(graphPage, "GraphKit");

    window.setCentralWidget(tabs);

    window.show();

    return app.exec();
}

