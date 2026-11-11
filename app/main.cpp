#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QTextCursor>
#include <QStackedWidget>

#include "algolib/data_structures.hpp"
#include "algolib/visualizer.hpp"
#include "algolib/compressx.hpp"
#include "algolib/graphkit.hpp"
#include "algolib/graph_visualizer.hpp"
#include "algolib/dplab.hpp"

using namespace algolib;

class AlgoLibWindow : public QMainWindow {
private:
    // Tab 1: Data Structures state
    DynamicArray array;
    LinkedList linkedList;
    Stack stack;
    Queue queue;
    BST tree;
    MinHeap heap;

    QComboBox* structureBox;
    QLineEdit* valueInput;
    QSpinBox* indexInput;
    QLabel* indexLabel;
    UnifiedDSVisualizer* dsVisualizer;
    QTextEdit* dsOutput;

    // Tab 2: CompressX state
    QLineEdit* filePathInput;
    QLabel* origSizeLabel;
    QLabel* compSizeLabel;
    QLabel* ratioLabel;
    QLabel* verifyBadge;
    QTextEdit* compressOutput;

    // Tab 3: GraphKit state
    Graph graph;
    GraphVisualizer* graphVisualizer;
    QLineEdit* vertexInput;
    QComboBox* edgeFromBox;
    QComboBox* edgeToBox;
    QSpinBox* edgeWeightBox;
    QComboBox* graphAlgoBox;
    QComboBox* algoStartBox;
    QComboBox* algoTargetBox;
    QLabel* targetLabel;
    QCheckBox* allComponentsCheck;
    QTextEdit* graphOutput;

    // Tab 4: DP Lab state
    QComboBox* dpProblemBox;
    QStackedWidget* dpInputsStack;
    // Fib inputs
    QSpinBox* fibNSpin;
    // Knapsack inputs
    QSpinBox* ksCapSpin;
    QLineEdit* ksWeightsInput;
    QLineEdit* ksValuesInput;
    // LCS inputs
    QLineEdit* lcsStrAInput;
    QLineEdit* lcsStrBInput;
    // Coin Change inputs
    QSpinBox* ccAmountSpin;
    QLineEdit* ccCoinsInput;

    QLabel* dpTimeBadge;
    QLabel* dpSpaceBadge;
    QLabel* dpFormulaLabel;
    QTableWidget* dpTableWidget;
    QTextEdit* dpOutput;

    // Helper to log formatted HTML output to any console
    void logToConsole(QTextEdit* console, const QString& title, const QString& message, const QString& status = "SUCCESS") {
        QString color = "#34D399"; // Success Emerald
        QString icon = "✓";

        if (status == "ERROR") {
            color = "#F87171"; // Coral Red
            icon = "✕";
        } else if (status == "INFO") {
            color = "#60A5FA"; // Sky Blue
            icon = "●";
        } else if (status == "WARNING") {
            color = "#FBBF24"; // Amber
            icon = "!";
        }

        QString safeTitle = title.toHtmlEscaped();
        QString safeMessage = message.toHtmlEscaped();
        safeMessage.replace("\n", "<br>");

        QString time = QDateTime::currentDateTime().toString("hh:mm:ss");

        QString html = QString(R"(
            <div style="background-color: #252538; border-left: 4px solid %1; border-radius: 6px; padding: 10px 14px; margin-bottom: 10px;">
                <div style="display: flex; justify-content: space-between; margin-bottom: 4px;">
                    <span style="color: %1; font-weight: bold; font-size: 13px;">%2 %3: %4</span>
                    <span style="color: #64748B; font-size: 11px;">[%5]</span>
                </div>
                <div style="color: #CBD5E1; font-size: 12px; font-family: 'Consolas', monospace; line-height: 140%;">
                    %6
                </div>
            </div>
        )").arg(color, icon, status, safeTitle, time, safeMessage);

        console->moveCursor(QTextCursor::End);
        console->insertHtml(html);
        console->ensureCursorVisible();
    }

public:
    AlgoLibWindow() : graph(false) {
        setWindowTitle("AlgoLib — Advanced Data Structures & Algorithms Toolkit");
        resize(1140, 750);
        setMinimumSize(960, 640);

        QTabWidget* mainTabs = new QTabWidget(this);
        mainTabs->addTab(createDataStructuresTab(), "  Data Structures  ");
        mainTabs->addTab(createCompressXTab(), "  CompressX  ");
        mainTabs->addTab(createGraphKitTab(), "  GraphKit  ");
        mainTabs->addTab(createDPLabTab(), "  DP Lab  ");

        setCentralWidget(mainTabs);

        // Apply Global Modern Dark Theme Styling
        setStyleSheet(R"(
            QMainWindow, QWidget {
                background-color: #1E1E2E;
                color: #E2E8F0;
                font-family: "Segoe UI", sans-serif;
                font-size: 13px;
            }
            QTabWidget::pane {
                border: 1px solid #343449;
                background: #1E1E2E;
                border-radius: 6px;
            }
            QTabBar::tab {
                background: #252538;
                color: #94A3B8;
                padding: 12px 24px;
                margin-right: 4px;
                border-top-left-radius: 6px;
                border-top-right-radius: 6px;
                font-weight: bold;
                font-size: 13px;
            }
            QTabBar::tab:selected {
                background: #2563EB;
                color: #FFFFFF;
            }
            QTabBar::tab:hover:!selected {
                background: #2D2D44;
                color: #CBD5E1;
            }
            QGroupBox {
                background-color: #252538;
                border: 1px solid #3A3A52;
                border-radius: 8px;
                margin-top: 14px;
                padding: 14px;
                font-weight: bold;
            }
            QGroupBox::title {
                color: #60A5FA;
                subcontrol-origin: margin;
                left: 12px;
                padding: 0 6px;
            }
            QLineEdit, QComboBox, QSpinBox {
                background-color: #171725;
                color: #F1F5F9;
                border: 1px solid #41415A;
                border-radius: 6px;
                padding: 8px 10px;
                min-height: 20px;
                selection-background-color: #2563EB;
            }
            QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
                border: 1px solid #3B82F6;
            }
            QPushButton {
                background-color: #34344A;
                color: #E2E8F0;
                border: 1px solid #46465F;
                border-radius: 6px;
                padding: 9px 14px;
                font-weight: 600;
                min-height: 20px;
            }
            QPushButton:hover {
                background-color: #454563;
            }
            QPushButton:pressed {
                background-color: #28283B;
            }
            QPushButton#primaryBtn {
                background-color: #2563EB;
                color: #FFFFFF;
                border: none;
            }
            QPushButton#primaryBtn:hover {
                background-color: #3B82F6;
            }
            QPushButton#successBtn {
                background-color: #059669;
                color: #FFFFFF;
                border: none;
            }
            QPushButton#successBtn:hover {
                background-color: #10B981;
            }
            QPushButton#dangerBtn {
                background-color: #B91C1C;
                color: #FFFFFF;
                border: none;
            }
            QPushButton#dangerBtn:hover {
                background-color: #DC2626;
            }
            QTextEdit {
                background-color: #171725;
                color: #E2E8F0;
                border: 1px solid #35354B;
                border-radius: 8px;
                padding: 10px;
                font-family: "Segoe UI", sans-serif;
            }
            QScrollBar:vertical {
                background: #1E1E2E;
                width: 10px;
                margin: 0;
            }
            QScrollBar::handle:vertical {
                background: #475569;
                border-radius: 4px;
                min-height: 25px;
            }
            QScrollBar::handle:vertical:hover {
                background: #64748B;
            }
            QScrollBar:horizontal {
                background: #1E1E2E;
                height: 10px;
            }
            QScrollBar::handle:horizontal {
                background: #475569;
                border-radius: 4px;
                min-width: 25px;
            }
            QTableWidget {
                background-color: #171725;
                gridline-color: #35354B;
                border: 1px solid #35354B;
                border-radius: 6px;
                color: #F1F5F9;
            }
            QHeaderView::section {
                background-color: #252538;
                color: #60A5FA;
                padding: 6px;
                border: 1px solid #35354B;
                font-weight: bold;
            }
        )");

        // Initial setup
        updateDSVisualizer();
        refreshGraphVertexCombos();
    }

private:
    // ==================================================
    // 1. DATA STRUCTURES TAB
    // ==================================================
    QWidget* createDataStructuresTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(20, 16, 20, 16);
        root->setSpacing(14);

        // Header Banner
        QHBoxLayout* head = new QHBoxLayout;
        QLabel* title = new QLabel("Data Structures Explorer");
        title->setStyleSheet("font-size: 22px; font-weight: bold; color: #60A5FA;");
        QLabel* desc = new QLabel("Interactive visual manipulation of custom C++ data structures without STL containers.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        QVBoxLayout* headText = new QVBoxLayout;
        headText->addWidget(title);
        headText->addWidget(desc);
        head->addLayout(headText);
        head->addStretch();
        root->addLayout(head);

        // Main Layout: Left controls, Right visualization + output
        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(16);

        // Control Panel
        QGroupBox* controls = new QGroupBox(" OPERATIONS PANEL ");
        controls->setFixedWidth(330);
        QVBoxLayout* cLayout = new QVBoxLayout(controls);
        cLayout->setSpacing(12);

        QLabel* sLabel = new QLabel("Select Structure:");
        structureBox = new QComboBox;
        structureBox->addItems({
            "1. Dynamic Array",
            "2. Singly Linked List",
            "3. Stack (LIFO)",
            "4. Queue (FIFO)",
            "5. Binary Search Tree",
            "6. Min Heap"
        });

        QLabel* vLabel = new QLabel("Value (Integer):");
        valueInput = new QLineEdit;
        valueInput->setPlaceholderText("e.g. 42");

        indexLabel = new QLabel("Index (Optional for Array/List):");
        indexInput = new QSpinBox;
        indexInput->setRange(0, 10000);
        indexInput->setValue(0);
        indexInput->setEnabled(true);

        cLayout->addWidget(sLabel);
        cLayout->addWidget(structureBox);
        cLayout->addWidget(vLabel);
        cLayout->addWidget(valueInput);
        cLayout->addWidget(indexLabel);
        cLayout->addWidget(indexInput);

        QPushButton* insertBtn = new QPushButton("Insert / Push / Enqueue");
        insertBtn->setObjectName("primaryBtn");
        QPushButton* insertAtBtn = new QPushButton("Insert at Index (Array)");
        QPushButton* deleteBtn = new QPushButton("Delete / Pop / Dequeue");
        QPushButton* searchBtn = new QPushButton("Search / Peek");
        QPushButton* displayBtn = new QPushButton("Display Summary");
        QPushButton* clearBtn = new QPushButton("Clear Structure");
        clearBtn->setObjectName("dangerBtn");

        cLayout->addSpacing(6);
        cLayout->addWidget(insertBtn);
        cLayout->addWidget(insertAtBtn);
        cLayout->addWidget(deleteBtn);
        cLayout->addWidget(searchBtn);
        cLayout->addWidget(displayBtn);
        cLayout->addWidget(clearBtn);
        cLayout->addStretch();

        QLabel* tip = new QLabel("Tip: All data structures use manual dynamic memory with zero STL dependencies.");
        tip->setStyleSheet("color: #64748B; font-size: 11px;");
        tip->setWordWrap(true);
        cLayout->addWidget(tip);

        // Right Visualization & Output Panel
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(12);

        // Live Visualizer Canvas
        dsVisualizer = new UnifiedDSVisualizer(this);
        rightLayout->addWidget(dsVisualizer, 3);

        // Output Console with Header
        QGroupBox* consoleBox = new QGroupBox(" EXECUTION LOG & TRACE ");
        QVBoxLayout* consoleLayout = new QVBoxLayout(consoleBox);
        consoleLayout->setContentsMargins(10, 12, 10, 10);

        QHBoxLayout* consoleHeader = new QHBoxLayout;
        QLabel* cTitle = new QLabel("Real-time Operation Results");
        cTitle->setStyleSheet("font-weight: bold; color: #94A3B8;");
        QPushButton* clearConsoleBtn = new QPushButton("Clear Log");
        clearConsoleBtn->setFixedWidth(100);
        consoleHeader->addWidget(cTitle);
        consoleHeader->addStretch();
        consoleHeader->addWidget(clearConsoleBtn);
        consoleLayout->addLayout(consoleHeader);

        dsOutput = new QTextEdit;
        dsOutput->setReadOnly(true);
        dsOutput->setMinimumHeight(150);
        consoleLayout->addWidget(dsOutput);

        rightLayout->addWidget(consoleBox, 2);

        mainLayout->addWidget(controls);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Connect Structure Box Switcher
        connect(structureBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
            dsVisualizer->setMode(idx);
            bool isArray = (idx == 0);
            indexLabel->setVisible(isArray);
            indexInput->setVisible(isArray);
            updateDSVisualizer();
        });

        // Button Actions
        connect(insertBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(0); });
        connect(insertAtBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(1); }); // Insert at index
        connect(deleteBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(2); });
        connect(searchBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(3); });
        connect(displayBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(4); });
        connect(clearBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(5); });
        connect(clearConsoleBtn, &QPushButton::clicked, dsOutput, &QTextEdit::clear);

        // Return key shortcut
        connect(valueInput, &QLineEdit::returnPressed, this, [this]() { executeDSOperation(0); });

        return page;
    }

    void updateDSVisualizer(int highlight = -1) {
        int idx = structureBox->currentIndex();
        switch (idx) {
            case 0: dsVisualizer->updateArray(&array, highlight); break;
            case 1: dsVisualizer->updateList(&linkedList, highlight); break;
            case 2: dsVisualizer->updateStack(&stack, highlight); break;
            case 3: dsVisualizer->updateQueue(&queue, highlight); break;
            case 4: dsVisualizer->updateBST(&tree); break;
            case 5: dsVisualizer->updateHeap(&heap, highlight); break;
        }
    }

    void executeDSOperation(int op) {
        int sel = structureBox->currentIndex();
        bool ok = false;
        int val = valueInput->text().toInt(&ok);
        int indexVal = indexInput->value();

        bool needsValue = (op == 0 || op == 1 || (op == 2 && (sel == 1 || sel == 4)) || (op == 3 && sel != 2 && sel != 3));

        if (needsValue && !ok && op != 5 && op != 4) {
            QMessageBox::warning(this, "Input Required", "Please enter a valid integer value.");
            valueInput->setFocus();
            return;
        }

        try {
            QString title;
            QString msg;
            int foundIdx = -1;

            if (sel == 0) { // Dynamic Array
                title = "Dynamic Array";
                if (op == 0) {
                    array.insert(val);
                    msg = QString("Inserted %1 at end. Current size: %2, capacity: %3.").arg(val).arg(array.size()).arg(array.capacity());
                } else if (op == 1) {
                    array.insertAt(static_cast<std::size_t>(indexVal), val);
                    msg = QString("Inserted %1 at index [%2]. Current size: %3.").arg(val).arg(indexVal).arg(array.size());
                } else if (op == 2) {
                    if (ok && array.search(val)) {
                        array.removeValue(val);
                        msg = QString("Removed first occurrence of value %1. Current size: %2.").arg(val).arg(array.size());
                    } else {
                        array.remove();
                        msg = QString("Removed last element. Current size: %1.").arg(array.size());
                    }
                } else if (op == 3) {
                    foundIdx = array.indexOf(val);
                    msg = (foundIdx != -1) ? QString("Value %1 FOUND at index [%2].").arg(val).arg(foundIdx)
                                           : QString("Value %1 NOT found in array.").arg(val);
                } else if (op == 4) {
                    msg = QString::fromStdString(array.display());
                } else if (op == 5) {
                    array.clear();
                    msg = "Dynamic Array cleared successfully.";
                }
            } else if (sel == 1) { // Linked List
                title = "Singly Linked List";
                if (op == 0) {
                    linkedList.insert(val);
                    msg = QString("Appended node with value %1. Total nodes: %2.").arg(val).arg(linkedList.size());
                } else if (op == 1) {
                    linkedList.insertHead(val);
                    msg = QString("Inserted %1 at HEAD. Total nodes: %2.").arg(val).arg(linkedList.size());
                } else if (op == 2) {
                    bool rem = linkedList.remove(val);
                    msg = rem ? QString("Deleted node with value %1. Remaining nodes: %2.").arg(val).arg(linkedList.size())
                              : QString("Value %1 not found in linked list.").arg(val);
                } else if (op == 3) {
                    bool found = linkedList.search(val);
                    msg = found ? QString("Value %1 FOUND in linked list.").arg(val)
                                : QString("Value %1 NOT found in linked list.").arg(val);
                } else if (op == 4) {
                    msg = QString::fromStdString(linkedList.display());
                } else if (op == 5) {
                    linkedList.clear();
                    msg = "Linked list cleared.";
                }
            } else if (sel == 2) { // Stack
                title = "Stack (LIFO)";
                if (op == 0 || op == 1) {
                    stack.push(val);
                    msg = QString("Pushed %1 onto Stack. Top is now %1. Size: %2.").arg(val).arg(stack.size());
                } else if (op == 2) {
                    int p = stack.pop();
                    msg = QString("Popped %1 from Stack. New size: %2.").arg(p).arg(stack.size());
                } else if (op == 3) {
                    int top = stack.peek();
                    msg = QString("PEEK: Top element is %1. Stack size: %2.").arg(top).arg(stack.size());
                } else if (op == 4) {
                    msg = QString::fromStdString(stack.display());
                } else if (op == 5) {
                    stack.clear();
                    msg = "Stack cleared.";
                }
            } else if (sel == 3) { // Queue
                title = "Queue (FIFO)";
                if (op == 0 || op == 1) {
                    queue.enqueue(val);
                    msg = QString("Enqueued %1 into Queue. Size: %2.").arg(val).arg(queue.size());
                } else if (op == 2) {
                    int d = queue.dequeue();
                    msg = QString("Dequeued %1 from Front of Queue. Size: %2.").arg(d).arg(queue.size());
                } else if (op == 3) {
                    int front = queue.front();
                    msg = QString("FRONT: Front element is %1. Queue size: %2.").arg(front).arg(queue.size());
                } else if (op == 4) {
                    msg = QString::fromStdString(queue.display());
                } else if (op == 5) {
                    queue.clear();
                    msg = "Queue cleared.";
                }
            } else if (sel == 4) { // BST
                title = "Binary Search Tree";
                if (op == 0 || op == 1) {
                    tree.insert(val);
                    msg = QString("Inserted %1 into BST. Distinct nodes: %2, Total elements: %3.")
                          .arg(val).arg(tree.size()).arg(tree.totalCount());
                } else if (op == 2) {
                    bool rem = tree.remove(val);
                    msg = rem ? QString("Removed %1 from BST (decremented count or deleted node).").arg(val)
                              : QString("Value %1 not found in BST.").arg(val);
                } else if (op == 3) {
                    bool f = tree.search(val);
                    msg = f ? QString("Value %1 FOUND in BST.").arg(val)
                            : QString("Value %1 NOT found in BST.").arg(val);
                } else if (op == 4) {
                    msg = QString("INORDER (Sorted): %1\nPREORDER: %2\nPOSTORDER: %3")
                          .arg(QString::fromStdString(tree.inorder()))
                          .arg(QString::fromStdString(tree.preorder()))
                          .arg(QString::fromStdString(tree.postorder()));
                } else if (op == 5) {
                    tree.clear();
                    msg = "Binary Search Tree cleared.";
                }
            } else if (sel == 5) { // Min Heap
                title = "Min Heap";
                if (op == 0 || op == 1) {
                    heap.insert(val);
                    msg = QString("Inserted %1 into Min Heap. Current minimum: %2. Total elements: %3.")
                          .arg(val).arg(heap.peekMin()).arg(heap.size());
                } else if (op == 2) {
                    int minV = heap.extractMin();
                    msg = QString("Extracted minimum element: %1. New size: %2.").arg(minV).arg(heap.size());
                    if (!heap.empty())
                        msg += QString(" New minimum: %1.").arg(heap.peekMin());
                } else if (op == 3) {
                    bool f = heap.search(val);
                    msg = f ? QString("Value %1 FOUND in Min Heap.").arg(val)
                            : QString("Value %1 NOT found in Min Heap.").arg(val);
                } else if (op == 4) {
                    msg = QString::fromStdString(heap.display());
                } else if (op == 5) {
                    heap.clear();
                    msg = "Min Heap cleared.";
                }
            }

            logToConsole(dsOutput, title, msg, "SUCCESS");
            updateDSVisualizer(foundIdx);
            valueInput->clear();
            valueInput->setFocus();

        } catch (const std::exception& e) {
            logToConsole(dsOutput, "Operation Failed", QString::fromLocal8Bit(e.what()), "ERROR");
        }
    }


    // ==================================================
    // 2. COMPRESSX TAB
    // ==================================================
    QWidget* createCompressXTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(20, 16, 20, 16);
        root->setSpacing(14);

        // Header
        QLabel* title = new QLabel("CompressX — Lossless Huffman Coding Engine");
        title->setStyleSheet("font-size: 22px; font-weight: bold; color: #34D399;");
        QLabel* desc = new QLabel("Custom Huffman Tree, Priority Queue, and bit-level file compressor and decompressor.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(16);

        // Left Action Panel
        QGroupBox* filePanel = new QGroupBox(" FILE COMPRESSION WORKFLOW ");
        filePanel->setFixedWidth(360);
        QVBoxLayout* fpLayout = new QVBoxLayout(filePanel);
        fpLayout->setSpacing(12);

        QLabel* selectLabel = new QLabel("Selected Target File:");
        QHBoxLayout* pickLayout = new QHBoxLayout;
        filePathInput = new QLineEdit;
        filePathInput->setPlaceholderText("Select a file to compress or decompress...");
        QPushButton* browseBtn = new QPushButton("Browse...");
        browseBtn->setFixedWidth(85);
        pickLayout->addWidget(filePathInput);
        pickLayout->addWidget(browseBtn);

        fpLayout->addWidget(selectLabel);
        fpLayout->addLayout(pickLayout);

        // Actions
        QPushButton* analyzeBtn = new QPushButton("1. Analyze Frequencies & Codes");
        QPushButton* compressBtn = new QPushButton("2. Compress File (.cmpx)");
        compressBtn->setObjectName("primaryBtn");
        QPushButton* decompressBtn = new QPushButton("3. Decompress Archive");
        decompressBtn->setObjectName("successBtn");
        QPushButton* verifyBtn = new QPushButton("4. Verify Bit-for-Bit Integrity");

        fpLayout->addSpacing(8);
        fpLayout->addWidget(analyzeBtn);
        fpLayout->addWidget(compressBtn);
        fpLayout->addWidget(decompressBtn);
        fpLayout->addWidget(verifyBtn);

        // Statistics Card
        QGroupBox* statsCard = new QGroupBox(" COMPRESSION STATISTICS ");
        QVBoxLayout* statsLayout = new QVBoxLayout(statsCard);
        origSizeLabel = new QLabel("Original File Size: -- bytes");
        compSizeLabel = new QLabel("Compressed Size: -- bytes");
        ratioLabel    = new QLabel("Space Savings: -- %");
        verifyBadge   = new QLabel("Integrity Status: Not Verified");
        verifyBadge->setStyleSheet("color: #94A3B8; font-weight: bold;");

        statsLayout->addWidget(origSizeLabel);
        statsLayout->addWidget(compSizeLabel);
        statsLayout->addWidget(ratioLabel);
        statsLayout->addWidget(verifyBadge);

        fpLayout->addWidget(statsCard);
        fpLayout->addStretch();

        // Right Console
        QGroupBox* outGroup = new QGroupBox(" COMPRESSX ENGINE CONSOLE ");
        QVBoxLayout* outLayout = new QVBoxLayout(outGroup);
        compressOutput = new QTextEdit;
        compressOutput->setReadOnly(true);
        outLayout->addWidget(compressOutput);

        mainLayout->addWidget(filePanel);
        mainLayout->addWidget(outGroup, 1);
        root->addLayout(mainLayout, 1);

        // Connections
        connect(browseBtn, &QPushButton::clicked, this, [this]() {
            QString path = QFileDialog::getOpenFileName(this, "Select File to Compress/Decompress");
            if (!path.isEmpty()) {
                filePathInput->setText(path);
            }
        });

        connect(analyzeBtn, &QPushButton::clicked, this, [this]() {
            QString path = filePathInput->text().trimmed();
            if (path.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select a file first.");
                return;
            }
            std::string analysis = CompressX::analyzeFrequenciesAndCodes(path.toStdString());
            compressOutput->clear();
            logToConsole(compressOutput, "Frequency & Code Analysis", QString::fromStdString(analysis), "INFO");
        });

        connect(compressBtn, &QPushButton::clicked, this, [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select a file to compress.");
                return;
            }
            QString outPath = inPath + ".cmpx";
            CompressionStats stats = CompressX::compressFile(inPath.toStdString(), outPath.toStdString());

            if (stats.success) {
                origSizeLabel->setText(QString("Original File Size: %1 bytes").arg(stats.originalBytes));
                compSizeLabel->setText(QString("Compressed Size: %1 bytes").arg(stats.compressedBytes));
                ratioLabel->setText(QString("Space Savings: %1% (Ratio: %2%)")
                                    .arg(QString::number(stats.spaceSaved, 'f', 2))
                                    .arg(QString::number(stats.compressionRatio, 'f', 2)));
                verifyBadge->setText("Compressed: Awaiting Decompression Test");
                verifyBadge->setStyleSheet("color: #60A5FA; font-weight: bold;");

                QString msg = QString("File compressed successfully!\nOutput Archive: %1\nOriginal Size: %2 bytes\nCompressed Size: %3 bytes\nSpace Saved: %4%")
                              .arg(outPath).arg(stats.originalBytes).arg(stats.compressedBytes).arg(stats.spaceSaved, 0, 'f', 2);
                logToConsole(compressOutput, "Compression Complete", msg, "SUCCESS");
            } else {
                logToConsole(compressOutput, "Compression Failed", QString::fromStdString(stats.errorMessage), "ERROR");
            }
        });

        connect(decompressBtn, &QPushButton::clicked, this, [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select a file or .cmpx archive.");
                return;
            }
            QString compPath = inPath.endsWith(".cmpx") ? inPath : inPath + ".cmpx";
            QString outPath = compPath;
            if (outPath.endsWith(".cmpx"))
                outPath.chop(5);
            outPath += ".restored";

            CompressionStats stats = CompressX::decompressFile(compPath.toStdString(), outPath.toStdString());

            if (stats.success) {
                QString msg = QString("Archive decompressed successfully!\nRestored File: %1\nRestored Bytes: %2 bytes")
                              .arg(outPath).arg(stats.originalBytes);
                logToConsole(compressOutput, "Decompression Complete", msg, "SUCCESS");
            } else {
                logToConsole(compressOutput, "Decompression Failed", QString::fromStdString(stats.errorMessage), "ERROR");
            }
        });

        connect(verifyBtn, &QPushButton::clicked, this, [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select the original file to verify.");
                return;
            }
            QString origPath = inPath.endsWith(".cmpx") ? inPath.left(inPath.length() - 5) : inPath;
            QString compPath = origPath + ".cmpx";
            QString restPath = origPath + ".restored";

            bool match = CompressX::verifyFilesMatch(origPath.toStdString(), restPath.toStdString());

            if (match) {
                verifyBadge->setText("STATUS: Byte-for-byte exact match! ✓");
                verifyBadge->setStyleSheet("color: #34D399; font-weight: bold;");
                logToConsole(compressOutput, "Integrity Verification", "SUCCESS: The decompressed file is a 100% byte-for-byte exact match to the original input file!", "SUCCESS");
            } else {
                verifyBadge->setText("STATUS: Mismatch or missing restored file ✕");
                verifyBadge->setStyleSheet("color: #F87171; font-weight: bold;");
                logToConsole(compressOutput, "Integrity Verification", "FAILED: Mismatch detected or restored file not found. Ensure you compressed and decompressed first.", "ERROR");
            }
        });

        return page;
    }


    // ==================================================
    // 3. GRAPHKIT TAB
    // ==================================================
    QWidget* createGraphKitTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(20, 16, 20, 16);
        root->setSpacing(14);

        // Header
        QLabel* title = new QLabel("GraphKit — Custom Graph Algorithms & Visualization");
        title->setStyleSheet("font-size: 22px; font-weight: bold; color: #60A5FA;");
        QLabel* desc = new QLabel("Custom Adjacency List graph with BFS, DFS, Dijkstra Shortest Path, and Kruskal's MST (supports disconnected graphs).");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(16);

        // Left Graph Control Panel
        QGroupBox* gPanel = new QGroupBox(" GRAPH CONSTRUCTION & CONTROLS ");
        gPanel->setFixedWidth(340);
        QVBoxLayout* gpLayout = new QVBoxLayout(gPanel);
        gpLayout->setSpacing(10);

        // Add Vertex
        QLabel* vLabel = new QLabel("Add Vertex Label:");
        QHBoxLayout* addVLayout = new QHBoxLayout;
        vertexInput = new QLineEdit;
        vertexInput->setPlaceholderText("e.g. G");
        QPushButton* addVBtn = new QPushButton("Add Node");
        addVBtn->setFixedWidth(90);
        addVLayout->addWidget(vertexInput);
        addVLayout->addWidget(addVBtn);

        gpLayout->addWidget(vLabel);
        gpLayout->addLayout(addVLayout);

        // Add Edge
        QLabel* eLabel = new QLabel("Add Weighted Edge:");
        QGridLayout* eGrid = new QGridLayout;
        edgeFromBox = new QComboBox;
        edgeToBox = new QComboBox;
        edgeWeightBox = new QSpinBox;
        edgeWeightBox->setRange(1, 1000);
        edgeWeightBox->setValue(5);

        eGrid->addWidget(new QLabel("From:"), 0, 0);
        eGrid->addWidget(edgeFromBox, 0, 1);
        eGrid->addWidget(new QLabel("To:"), 1, 0);
        eGrid->addWidget(edgeToBox, 1, 1);
        eGrid->addWidget(new QLabel("Weight:"), 2, 0);
        eGrid->addWidget(edgeWeightBox, 2, 1);

        QPushButton* addEdgeBtn = new QPushButton("Connect Edge");
        addEdgeBtn->setObjectName("primaryBtn");

        gpLayout->addWidget(eLabel);
        gpLayout->addLayout(eGrid);
        gpLayout->addWidget(addEdgeBtn);

        // Presets
        QHBoxLayout* presetLayout = new QHBoxLayout;
        QPushButton* loadSampleBtn = new QPushButton("Load Connected Preset");
        QPushButton* loadDiscBtn = new QPushButton("Load Disconnected Preset");
        presetLayout->addWidget(loadSampleBtn);
        presetLayout->addWidget(loadDiscBtn);
        QPushButton* clearGBtn = new QPushButton("Clear Graph");
        clearGBtn->setObjectName("dangerBtn");

        gpLayout->addSpacing(6);
        gpLayout->addLayout(presetLayout);
        gpLayout->addWidget(clearGBtn);

        // Algorithm Execution Box
        QGroupBox* algoBox = new QGroupBox(" ALGORITHM SELECTION ");
        QVBoxLayout* abLayout = new QVBoxLayout(algoBox);
        abLayout->setSpacing(8);

        graphAlgoBox = new QComboBox;
        graphAlgoBox->addItems({
            "1. Breadth First Search (BFS)",
            "2. Depth First Search (DFS)",
            "3. Dijkstra's Shortest Path",
            "4. Kruskal's Minimum Spanning Tree"
        });

        QHBoxLayout* srcDstLayout = new QHBoxLayout;
        algoStartBox = new QComboBox;
        algoTargetBox = new QComboBox;
        targetLabel = new QLabel("Target:");
        srcDstLayout->addWidget(new QLabel("Start:"));
        srcDstLayout->addWidget(algoStartBox);
        srcDstLayout->addWidget(targetLabel);
        srcDstLayout->addWidget(algoTargetBox);

        allComponentsCheck = new QCheckBox("Traverse All Disconnected Components");
        allComponentsCheck->setChecked(true);

        QPushButton* runAlgoBtn = new QPushButton("Execute Graph Algorithm");
        runAlgoBtn->setObjectName("successBtn");

        abLayout->addWidget(graphAlgoBox);
        abLayout->addLayout(srcDstLayout);
        abLayout->addWidget(allComponentsCheck);
        abLayout->addWidget(runAlgoBtn);

        gpLayout->addWidget(algoBox);
        gpLayout->addStretch();

        // Right Visualizer & Output
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(12);

        graphVisualizer = new GraphVisualizer(this);
        rightLayout->addWidget(graphVisualizer, 3);

        QGroupBox* gOutBox = new QGroupBox(" GRAPH ANALYSIS & PATH TRACE ");
        QVBoxLayout* gobLayout = new QVBoxLayout(gOutBox);
        graphOutput = new QTextEdit;
        graphOutput->setReadOnly(true);
        gobLayout->addWidget(graphOutput);

        rightLayout->addWidget(gOutBox, 2);

        mainLayout->addWidget(gPanel);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Connect graph actions
        connect(addVBtn, &QPushButton::clicked, this, [this]() {
            QString lbl = vertexInput->text().trimmed();
            if (lbl.isEmpty()) return;
            graph.addVertex(lbl.toStdString());
            vertexInput->clear();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Vertex Added", QString("Added vertex '%1'. Total vertices: %2.").arg(lbl).arg(graph.getVertexCount()), "INFO");
        });

        connect(addEdgeBtn, &QPushButton::clicked, this, [this]() {
            if (edgeFromBox->currentIndex() < 0 || edgeToBox->currentIndex() < 0) return;
            int u = edgeFromBox->currentIndex();
            int v = edgeToBox->currentIndex();
            int w = edgeWeightBox->value();
            graph.addEdge(u, v, w);
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Edge Connected", QString("Connected edge %1 <--> %2 (Weight: %3).")
                         .arg(QString::fromStdString(graph.getVertexLabel(u)))
                         .arg(QString::fromStdString(graph.getVertexLabel(v))).arg(w), "INFO");
        });

        connect(loadSampleBtn, &QPushButton::clicked, this, [this]() {
            graph.loadSampleGraph();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Sample Graph Loaded", "Loaded 6-vertex connected weighted graph preset.", "SUCCESS");
        });

        connect(loadDiscBtn, &QPushButton::clicked, this, [this]() {
            graph.loadDisconnectedSample();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Disconnected Preset Loaded", "Loaded disconnected graph preset with 2 separate components (A-B-C-D and X-Y-Z).", "WARNING");
        });

        connect(clearGBtn, &QPushButton::clicked, this, [this]() {
            graph.clear();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Graph Cleared", "All vertices and edges have been removed.", "INFO");
        });

        connect(graphAlgoBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
            bool isDijkstra = (idx == 2);
            bool isTraversal = (idx == 0 || idx == 1);
            algoTargetBox->setVisible(isDijkstra);
            targetLabel->setVisible(isDijkstra);
            allComponentsCheck->setVisible(isTraversal);
        });

        connect(runAlgoBtn, &QPushButton::clicked, this, [this]() {
            if (graph.getVertexCount() == 0) {
                QMessageBox::warning(this, "Empty Graph", "Please add vertices and edges or load a preset first.");
                return;
            }

            int algo = graphAlgoBox->currentIndex();
            int start = algoStartBox->currentIndex();
            int target = algoTargetBox->currentIndex();
            bool visitAll = allComponentsCheck->isChecked();

            if (algo == 0) { // BFS
                BFSResult res = graph.bfs(start, visitAll);
                logToConsole(graphOutput, "BFS Traversal", QString::fromStdString(res.summary), "SUCCESS");
                graphVisualizer->highlightTraversal(res.visitedOrder, res.visitedCount, QColor("#F59E0B"));
                delete[] res.visitedOrder;
            } else if (algo == 1) { // DFS
                DFSResult res = graph.dfs(start, visitAll);
                logToConsole(graphOutput, "DFS Traversal", QString::fromStdString(res.summary), "SUCCESS");
                graphVisualizer->highlightTraversal(res.visitedOrder, res.visitedCount, QColor("#F59E0B"));
                delete[] res.visitedOrder;
            } else if (algo == 2) { // Dijkstra
                DijkstraResult res = graph.dijkstra(start, target);
                QString status = res.pathExists ? "SUCCESS" : "WARNING";
                logToConsole(graphOutput, "Dijkstra Shortest Path", QString::fromStdString(res.summary), status);
                if (res.pathExists) {
                    graphVisualizer->highlightPath(res.path, res.pathLength, QColor("#38BDF8"));
                } else {
                    graphVisualizer->clearHighlights();
                }
                delete[] res.distances;
                delete[] res.parent;
                delete[] res.path;
            } else if (algo == 3) { // Kruskal MST
                KruskalResult res = graph.kruskalMST();
                QString status = res.isConnected ? "SUCCESS" : "WARNING";
                logToConsole(graphOutput, "Kruskal Minimum Spanning Tree", QString::fromStdString(res.summary), status);
                graphVisualizer->highlightMST(res.mstEdges, res.mstEdgeCount, QColor("#10B981"));
                delete[] res.mstEdges;
            }
        });

        // Initialize with default sample
        graph.loadSampleGraph();
        graphVisualizer->setGraph(&graph);

        return page;
    }

    void refreshGraphVertexCombos() {
        edgeFromBox->clear();
        edgeToBox->clear();
        algoStartBox->clear();
        algoTargetBox->clear();

        for (int i = 0; i < graph.getVertexCount(); ++i) {
            QString lbl = QString::fromStdString(graph.getVertexLabel(i));
            edgeFromBox->addItem(lbl);
            edgeToBox->addItem(lbl);
            algoStartBox->addItem(lbl);
            algoTargetBox->addItem(lbl);
        }

        if (edgeToBox->count() > 1)
            edgeToBox->setCurrentIndex(1);
        if (algoTargetBox->count() > 1)
            algoTargetBox->setCurrentIndex(algoTargetBox->count() - 1);
    }


    // ==================================================
    // 4. DP LAB TAB
    // ==================================================
    QWidget* createDPLabTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(20, 16, 20, 16);
        root->setSpacing(14);

        // Header
        QLabel* title = new QLabel("DP Lab — Dynamic Programming Interactive Laboratory");
        title->setStyleSheet("font-size: 22px; font-weight: bold; color: #A78BFA;");
        QLabel* desc = new QLabel("Tabulation, 2D matrix state tracking, and optimal traceback without STL containers.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(16);

        // Left Problem Panel
        QGroupBox* pPanel = new QGroupBox(" PROBLEM CONFIGURATION ");
        pPanel->setFixedWidth(340);
        QVBoxLayout* ppLayout = new QVBoxLayout(pPanel);
        ppLayout->setSpacing(12);

        QLabel* selLabel = new QLabel("Select DP Problem:");
        dpProblemBox = new QComboBox;
        dpProblemBox->addItems({
            "1. Fibonacci Sequence",
            "2. 0/1 Knapsack",
            "3. Longest Common Subsequence",
            "4. Coin Change (Fewest Coins)"
        });

        ppLayout->addWidget(selLabel);
        ppLayout->addWidget(dpProblemBox);

        // Stack of dynamic inputs
        dpInputsStack = new QStackedWidget;

        // 0: Fib
        QWidget* fibPage = new QWidget;
        QVBoxLayout* fibLayout = new QVBoxLayout(fibPage);
        fibLayout->addWidget(new QLabel("Nth Fibonacci Term (0..90):"));
        fibNSpin = new QSpinBox;
        fibNSpin->setRange(0, 90);
        fibNSpin->setValue(10);
        fibLayout->addWidget(fibNSpin);
        fibLayout->addStretch();
        dpInputsStack->addWidget(fibPage);

        // 1: Knapsack
        QWidget* ksPage = new QWidget;
        QVBoxLayout* ksLayout = new QVBoxLayout(ksPage);
        ksLayout->addWidget(new QLabel("Knapsack Capacity (W):"));
        ksCapSpin = new QSpinBox;
        ksCapSpin->setRange(1, 100);
        ksCapSpin->setValue(7);
        ksLayout->addWidget(ksCapSpin);

        ksLayout->addWidget(new QLabel("Item Weights (comma separated):"));
        ksWeightsInput = new QLineEdit("2, 3, 4, 5");
        ksLayout->addWidget(ksWeightsInput);

        ksLayout->addWidget(new QLabel("Item Values (comma separated):"));
        ksValuesInput = new QLineEdit("3, 4, 5, 6");
        ksLayout->addWidget(ksValuesInput);
        ksLayout->addStretch();
        dpInputsStack->addWidget(ksPage);

        // 2: LCS
        QWidget* lcsPage = new QWidget;
        QVBoxLayout* lcsLayout = new QVBoxLayout(lcsPage);
        lcsLayout->addWidget(new QLabel("String A:"));
        lcsStrAInput = new QLineEdit("ABCBDAB");
        lcsLayout->addWidget(lcsStrAInput);

        lcsLayout->addWidget(new QLabel("String B:"));
        lcsStrBInput = new QLineEdit("BDCAB");
        lcsLayout->addWidget(lcsStrBInput);
        lcsLayout->addStretch();
        dpInputsStack->addWidget(lcsPage);

        // 3: Coin Change
        QWidget* ccPage = new QWidget;
        QVBoxLayout* ccLayout = new QVBoxLayout(ccPage);
        ccLayout->addWidget(new QLabel("Target Amount:"));
        ccAmountSpin = new QSpinBox;
        ccAmountSpin->setRange(1, 100);
        ccAmountSpin->setValue(11);
        ccLayout->addWidget(ccAmountSpin);

        ccLayout->addWidget(new QLabel("Coin Denominations (comma separated):"));
        ccCoinsInput = new QLineEdit("1, 2, 5");
        ccLayout->addWidget(ccCoinsInput);
        ccLayout->addStretch();
        dpInputsStack->addWidget(ccPage);

        ppLayout->addWidget(dpInputsStack);

        QPushButton* solveBtn = new QPushButton("Compute Optimal Solution");
        solveBtn->setObjectName("primaryBtn");
        ppLayout->addWidget(solveBtn);

        // Complexity Card
        QGroupBox* compCard = new QGroupBox(" THEORETICAL COMPLEXITY ");
        QVBoxLayout* compLayout = new QVBoxLayout(compCard);
        dpTimeBadge = new QLabel("Time Complexity: O(N)");
        dpSpaceBadge = new QLabel("Space Complexity: O(N)");
        dpFormulaLabel = new QLabel("Recurrence: F(i) = F(i-1) + F(i-2)");
        dpFormulaLabel->setWordWrap(true);
        dpFormulaLabel->setStyleSheet("color: #60A5FA; font-size: 11px;");

        compLayout->addWidget(dpTimeBadge);
        compLayout->addWidget(dpSpaceBadge);
        compLayout->addWidget(dpFormulaLabel);

        ppLayout->addWidget(compCard);
        ppLayout->addStretch();

        // Right Table Visualizer & Explanation
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(12);

        QGroupBox* tableBox = new QGroupBox(" DYNAMIC PROGRAMMING STATE MATRIX / TABLE ");
        QVBoxLayout* tbLayout = new QVBoxLayout(tableBox);
        dpTableWidget = new QTableWidget;
        tbLayout->addWidget(dpTableWidget);
        rightLayout->addWidget(tableBox, 3);

        QGroupBox* dpOutBox = new QGroupBox(" TRACEBACK & RECONSTRUCTION EXPLANATION ");
        QVBoxLayout* dobLayout = new QVBoxLayout(dpOutBox);
        dpOutput = new QTextEdit;
        dpOutput->setReadOnly(true);
        dobLayout->addWidget(dpOutput);
        rightLayout->addWidget(dpOutBox, 2);

        mainLayout->addWidget(pPanel);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Switch problem inputs
        connect(dpProblemBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
            dpInputsStack->setCurrentIndex(idx);
            if (idx == 0) {
                dpTimeBadge->setText("Time Complexity: O(N)");
                dpSpaceBadge->setText("Space Complexity: O(N)");
                dpFormulaLabel->setText("Recurrence: F(i) = F(i-1) + F(i-2)");
            } else if (idx == 1) {
                dpTimeBadge->setText("Time Complexity: O(N * W)");
                dpSpaceBadge->setText("Space Complexity: O(N * W)");
                dpFormulaLabel->setText("Recurrence: dp[i][w] = max(dp[i-1][w], val[i-1] + dp[i-1][w - wt[i-1]])");
            } else if (idx == 2) {
                dpTimeBadge->setText("Time Complexity: O(M * N)");
                dpSpaceBadge->setText("Space Complexity: O(M * N)");
                dpFormulaLabel->setText("Recurrence: if A[i]==B[j] 1+dp[i-1][j-1] else max(up, left)");
            } else if (idx == 3) {
                dpTimeBadge->setText("Time Complexity: O(Amount * Coins)");
                dpSpaceBadge->setText("Space Complexity: O(Amount)");
                dpFormulaLabel->setText("Recurrence: dp[i] = min(dp[i], 1 + dp[i - c])");
            }
        });

        // Solve button
        connect(solveBtn, &QPushButton::clicked, this, &AlgoLibWindow::solveCurrentDPProblem);

        // Run initial default solution
        solveCurrentDPProblem();

        return page;
    }

    void solveCurrentDPProblem() {
        int prob = dpProblemBox->currentIndex();

        if (prob == 0) { // Fibonacci
            int n = fibNSpin->value();
            FibResult res = DPLab::computeFibonacci(n);

            dpTableWidget->clear();
            dpTableWidget->setRowCount(2);
            dpTableWidget->setColumnCount(n + 1);

            QStringList vHeaders;
            vHeaders << "i" << "F(i)";
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            for (int i = 0; i <= n; ++i)
                hHeaders << QString::number(i);
            dpTableWidget->setHorizontalHeaderLabels(hHeaders);

            for (int i = 0; i <= n; ++i) {
                QTableWidgetItem* itemIdx = new QTableWidgetItem(QString::number(i));
                itemIdx->setTextAlignment(Qt::AlignCenter);
                dpTableWidget->setItem(0, i, itemIdx);

                QTableWidgetItem* itemVal = new QTableWidgetItem(QString::number(res.sequence[i]));
                itemVal->setTextAlignment(Qt::AlignCenter);
                if (i == n) {
                    itemVal->setBackground(QColor("#1E3A8A"));
                    itemVal->setForeground(QColor("#60A5FA"));
                }
                dpTableWidget->setItem(1, i, itemVal);
            }

            dpOutput->clear();
            logToConsole(dpOutput, "Fibonacci Solved", QString::fromStdString(res.explanation), "SUCCESS");
            DPLab::freeFibResult(res);

        } else if (prob == 1) { // 0/1 Knapsack
            int cap = ksCapSpin->value();
            QStringList wStrs = ksWeightsInput->text().split(',', Qt::SkipEmptyParts);
            QStringList vStrs = ksValuesInput->text().split(',', Qt::SkipEmptyParts);

            int count = qMin(wStrs.size(), vStrs.size());
            if (count == 0) {
                QMessageBox::warning(this, "Input Error", "Please provide comma-separated weights and values.");
                return;
            }

            int* weights = new int[count];
            int* values = new int[count];
            for (int i = 0; i < count; ++i) {
                weights[i] = wStrs[i].trimmed().toInt();
                values[i] = vStrs[i].trimmed().toInt();
            }

            KnapsackResult res = DPLab::solveKnapsack(cap, weights, values, count);

            dpTableWidget->clear();
            dpTableWidget->setRowCount(count + 1);
            dpTableWidget->setColumnCount(cap + 1);

            QStringList vHeaders;
            vHeaders << "0 (Empty)";
            for (int i = 1; i <= count; ++i)
                vHeaders << QString("Item %1 (w:%2, v:%3)").arg(i).arg(weights[i - 1]).arg(values[i - 1]);
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            for (int w = 0; w <= cap; ++w)
                hHeaders << QString("w=%1").arg(w);
            dpTableWidget->setHorizontalHeaderLabels(hHeaders);

            for (int i = 0; i <= count; ++i) {
                for (int w = 0; w <= cap; ++w) {
                    QTableWidgetItem* item = new QTableWidgetItem(QString::number(res.dpTable[i][w]));
                    item->setTextAlignment(Qt::AlignCenter);
                    if (i == count && w == cap) {
                        item->setBackground(QColor("#065F46")); // Optimal cell green
                        item->setForeground(QColor("#34D399"));
                    }
                    dpTableWidget->setItem(i, w, item);
                }
            }

            dpOutput->clear();
            logToConsole(dpOutput, "0/1 Knapsack Solved", QString::fromStdString(res.explanation), "SUCCESS");

            delete[] weights;
            delete[] values;
            DPLab::freeKnapsackResult(res);

        } else if (prob == 2) { // LCS
            std::string sA = lcsStrAInput->text().trimmed().toStdString();
            std::string sB = lcsStrBInput->text().trimmed().toStdString();

            if (sA.empty() || sB.empty()) {
                QMessageBox::warning(this, "Input Error", "Please provide both strings.");
                return;
            }

            LCSResult res = DPLab::solveLCS(sA, sB);
            int m = static_cast<int>(sA.length());
            int n = static_cast<int>(sB.length());

            dpTableWidget->clear();
            dpTableWidget->setRowCount(m + 1);
            dpTableWidget->setColumnCount(n + 1);

            QStringList vHeaders;
            vHeaders << "Ø";
            for (int i = 0; i < m; ++i)
                vHeaders << QString("A[%1] = '%2'").arg(i).arg(sA[i]);
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            hHeaders << "Ø";
            for (int j = 0; j < n; ++j)
                hHeaders << QString("B[%1] = '%2'").arg(j).arg(sB[j]);
            dpTableWidget->setHorizontalHeaderLabels(hHeaders);

            for (int i = 0; i <= m; ++i) {
                for (int j = 0; j <= n; ++j) {
                    QTableWidgetItem* item = new QTableWidgetItem(QString::number(res.dpTable[i][j]));
                    item->setTextAlignment(Qt::AlignCenter);
                    if (i == m && j == n) {
                        item->setBackground(QColor("#065F46"));
                        item->setForeground(QColor("#34D399"));
                    }
                    dpTableWidget->setItem(i, j, item);
                }
            }

            dpOutput->clear();
            logToConsole(dpOutput, "LCS Solved", QString::fromStdString(res.explanation), "SUCCESS");
            DPLab::freeLCSResult(res);

        } else if (prob == 3) { // Coin Change
            int amount = ccAmountSpin->value();
            QStringList cStrs = ccCoinsInput->text().split(',', Qt::SkipEmptyParts);
            int count = cStrs.size();

            if (count == 0) {
                QMessageBox::warning(this, "Input Error", "Please provide coin denominations.");
                return;
            }

            int* coins = new int[count];
            for (int i = 0; i < count; ++i)
                coins[i] = cStrs[i].trimmed().toInt();

            CoinChangeResult res = DPLab::solveCoinChange(amount, coins, count);

            dpTableWidget->clear();
            dpTableWidget->setRowCount(2);
            dpTableWidget->setColumnCount(amount + 1);

            QStringList vHeaders;
            vHeaders << "Amount" << "Min Coins";
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            for (int i = 0; i <= amount; ++i)
                hHeaders << QString::number(i);
            dpTableWidget->setHorizontalHeaderLabels(hHeaders);

            for (int i = 0; i <= amount; ++i) {
                QTableWidgetItem* itemAmt = new QTableWidgetItem(QString::number(i));
                itemAmt->setTextAlignment(Qt::AlignCenter);
                dpTableWidget->setItem(0, i, itemAmt);

                QString valStr = (res.dpTable[i] >= 1e9) ? "INF" : QString::number(res.dpTable[i]);
                QTableWidgetItem* itemCoins = new QTableWidgetItem(valStr);
                itemCoins->setTextAlignment(Qt::AlignCenter);
                if (i == amount) {
                    itemCoins->setBackground(QColor("#1E3A8A"));
                    itemCoins->setForeground(QColor("#60A5FA"));
                }
                dpTableWidget->setItem(1, i, itemCoins);
            }

            dpOutput->clear();
            QString status = res.isPossible ? "SUCCESS" : "WARNING";
            logToConsole(dpOutput, "Coin Change Solved", QString::fromStdString(res.explanation), status);

            delete[] coins;
            DPLab::freeCoinChangeResult(res);
        }
    }
};

// ==============================================
// APPLICATION ENTRY POINT
// ==============================================
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    AlgoLibWindow window;
    window.show();

    return app.exec();
}
