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
#include <QScrollArea>
#include <QSplitter>
#include <QFileInfo>

#include "algolib/data_structures.hpp"
#include "algolib/visualizer.hpp"
#include "algolib/code_panel.hpp"
#include "algolib/compressx.hpp"
#include "algolib/graphkit.hpp"
#include "algolib/graph_visualizer.hpp"
#include "algolib/dplab.hpp"

using namespace algolib;

class AlgoLibWindow : public QMainWindow {
private:
    // ==========================================
    // 1. DATA STRUCTURES MODULE STATE
    // ==========================================
    DynamicArray array;
    LinkedList linkedList;
    Stack stack;
    Queue queue;
    BST tree;
    MinHeap heap;

    QComboBox* structureBox;
    QLineEdit* valueInput;
    QLineEdit* searchInput;
    QSpinBox* indexInput;
    QLabel* indexLabel;
    UnifiedDSVisualizer* dsVisualizer;
    CodeSnippetPanel* dsCodePanel;
    QTextEdit* dsOutput;

    // ==========================================
    // 2. COMPRESSX MODULE STATE
    // ==========================================
    QLineEdit* filePathInput;
    QLabel* origSizeLabel;
    QLabel* compSizeLabel;
    QLabel* ratioLabel;
    QLabel* verifyBadge;
    CodeSnippetPanel* compressCodePanel;
    QTextEdit* compressOutput;

    // ==========================================
    // 3. GRAPHKIT MODULE STATE
    // ==========================================
    Graph graph;
    GraphVisualizer* graphVisualizer;
    CodeSnippetPanel* graphCodePanel;
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

    // ==========================================
    // 4. DP LAB MODULE STATE
    // ==========================================
    QComboBox* dpProblemBox;
    QStackedWidget* dpInputsStack;
    QSpinBox* fibNSpin;
    QSpinBox* ksCapSpin;
    QLineEdit* ksWeightsInput;
    QLineEdit* ksValuesInput;
    QLineEdit* lcsStrAInput;
    QLineEdit* lcsStrBInput;
    QSpinBox* ccAmountSpin;
    QLineEdit* ccCoinsInput;

    QLabel* dpTimeBadge;
    QLabel* dpSpaceBadge;
    QLabel* dpFormulaLabel;
    QTableWidget* dpTableWidget;
    CodeSnippetPanel* dpCodePanel;
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
            <div style="background-color: #252538; border-left: 4px solid %1; border-radius: 6px; padding: 10px 14px; margin-bottom: 8px;">
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
        resize(1200, 780);
        setMinimumSize(980, 660);

        QTabWidget* mainTabs = new QTabWidget(this);
        mainTabs->addTab(createDataStructuresTab(), "  Data Structures Lab  ");
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
                padding: 12px;
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
                padding: 7px 10px;
                min-height: 22px;
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
                padding: 8px 14px;
                font-weight: 600;
                min-height: 22px;
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
            QSplitter::handle {
                background-color: #2D2D44;
            }
            QSplitter::handle:hover {
                background-color: #3B82F6;
            }
        )");

        // Initial setup
        updateDSVisualizer();
        refreshGraphVertexCombos();
    }

private:
    // ==================================================
    // 1. DATA STRUCTURES LAB TAB
    // ==================================================
    QWidget* createDataStructuresTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(16, 12, 16, 12);
        root->setSpacing(10);

        // Header
        QHBoxLayout* head = new QHBoxLayout;
        QLabel* title = new QLabel("Data Structures Lab");
        title->setStyleSheet("font-size: 20px; font-weight: bold; color: #60A5FA;");
        QLabel* desc = new QLabel("Custom C++ dynamic data structures with live graphical memory layout and synchronized C++ source code.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        QVBoxLayout* headText = new QVBoxLayout;
        headText->addWidget(title);
        headText->addWidget(desc);
        head->addLayout(headText);
        head->addStretch();
        root->addLayout(head);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(14);

        // Scrollable Left Control Panel
        QScrollArea* controlScroll = new QScrollArea;
        controlScroll->setWidgetResizable(true);
        controlScroll->setFixedWidth(330);
        controlScroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

        QWidget* controlContainer = new QWidget;
        QVBoxLayout* cLayout = new QVBoxLayout(controlContainer);
        cLayout->setContentsMargins(0, 0, 10, 0);
        cLayout->setSpacing(10);

        QGroupBox* controls = new QGroupBox(" OPERATIONS PANEL ");
        QVBoxLayout* cSubLayout = new QVBoxLayout(controls);
        cSubLayout->setSpacing(10);

        QLabel* sLabel = new QLabel("Data Structure:");
        structureBox = new QComboBox;
        structureBox->addItems({
            "1. Dynamic Array",
            "2. Singly Linked List",
            "3. Stack (LIFO)",
            "4. Queue (FIFO)",
            "5. Binary Search Tree",
            "6. Min Heap"
        });

        QLabel* vLabel = new QLabel("Value Input (Enter ➔ Insert):");
        valueInput = new QLineEdit;
        valueInput->setPlaceholderText("Enter integer, press Enter...");

        QLabel* srchLabel = new QLabel("Search Input (Enter ➔ Search):");
        searchInput = new QLineEdit;
        searchInput->setPlaceholderText("Value to search, press Enter...");

        indexLabel = new QLabel("Index (Insert/Remove at index):");
        indexInput = new QSpinBox;
        indexInput->setRange(0, 10000);
        indexInput->setValue(0);

        cSubLayout->addWidget(sLabel);
        cSubLayout->addWidget(structureBox);
        cSubLayout->addWidget(vLabel);
        cSubLayout->addWidget(valueInput);
        cSubLayout->addWidget(srchLabel);
        cSubLayout->addWidget(searchInput);
        cSubLayout->addWidget(indexLabel);
        cSubLayout->addWidget(indexInput);

        QPushButton* insertBtn = new QPushButton("Insert / Push / Enqueue");
        insertBtn->setObjectName("primaryBtn");
        QPushButton* insertAtBtn = new QPushButton("Insert at Index (Array)");
        QPushButton* deleteBtn = new QPushButton("Delete / Pop / Dequeue");
        QPushButton* searchBtn = new QPushButton("Search / Peek");
        QPushButton* displayBtn = new QPushButton("Display Traversal / Elements");
        QPushButton* clearBtn = new QPushButton("Clear Structure");
        clearBtn->setObjectName("dangerBtn");

        cSubLayout->addSpacing(4);
        cSubLayout->addWidget(insertBtn);
        cSubLayout->addWidget(insertAtBtn);
        cSubLayout->addWidget(deleteBtn);
        cSubLayout->addWidget(searchBtn);
        cSubLayout->addWidget(displayBtn);
        cSubLayout->addWidget(clearBtn);

        cLayout->addWidget(controls);
        cLayout->addStretch();
        controlScroll->setWidget(controlContainer);

        // Right Results Area: Splitter (Visualization on Left, Code on Right)
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(10);

        QSplitter* visualCodeSplitter = new QSplitter(Qt::Horizontal);
        dsVisualizer = new UnifiedDSVisualizer(this);
        dsCodePanel = new CodeSnippetPanel(this);

        visualCodeSplitter->addWidget(dsVisualizer);
        visualCodeSplitter->addWidget(dsCodePanel);
        visualCodeSplitter->setStretchFactor(0, 3);
        visualCodeSplitter->setStretchFactor(1, 2);

        rightLayout->addWidget(visualCodeSplitter, 3);

        // Bottom Console
        QGroupBox* consoleBox = new QGroupBox(" EXECUTION LOG & TRACE ");
        QVBoxLayout* consoleLayout = new QVBoxLayout(consoleBox);
        consoleLayout->setContentsMargins(8, 8, 8, 8);

        QHBoxLayout* consoleHeader = new QHBoxLayout;
        QLabel* cTitle = new QLabel("Operation Results & Diagnostic Output");
        cTitle->setStyleSheet("font-weight: bold; color: #94A3B8;");
        QPushButton* clearConsoleBtn = new QPushButton("Clear Log");
        clearConsoleBtn->setFixedWidth(90);
        consoleHeader->addWidget(cTitle);
        consoleHeader->addStretch();
        consoleHeader->addWidget(clearConsoleBtn);
        consoleLayout->addLayout(consoleHeader);

        dsOutput = new QTextEdit;
        dsOutput->setReadOnly(true);
        dsOutput->setMinimumHeight(130);
        consoleLayout->addWidget(dsOutput);

        rightLayout->addWidget(consoleBox, 2);

        mainLayout->addWidget(controlScroll);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Connect Structure Switcher
        connect(structureBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, insertAtBtn](int idx) {
            dsVisualizer->setMode(idx);
            bool isArray = (idx == 0);
            indexLabel->setVisible(isArray);
            indexInput->setVisible(isArray);
            insertAtBtn->setVisible(isArray);
            updateDSVisualizer();
        });

        // Buttons
        connect(insertBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(0); });
        connect(insertAtBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(1); });
        connect(deleteBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(2); });
        connect(searchBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(3); });
        connect(displayBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(4); });
        connect(clearBtn, &QPushButton::clicked, this, [this]() { executeDSOperation(5); });
        connect(clearConsoleBtn, &QPushButton::clicked, dsOutput, &QTextEdit::clear);

        // Enter key support
        connect(valueInput, &QLineEdit::returnPressed, this, [this]() { executeDSOperation(0); });
        connect(searchInput, &QLineEdit::returnPressed, this, [this]() { executeDSOperation(3); });
        if (QLineEdit* edit = indexInput->findChild<QLineEdit*>()) {
            connect(edit, &QLineEdit::returnPressed, this, [this]() { executeDSOperation(1); });
        }

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
        bool okVal = false;
        int val = valueInput->text().trimmed().toInt(&okVal);

        bool okSearch = false;
        int sVal = searchInput->text().trimmed().toInt(&okSearch);
        if (op == 3 && okSearch) {
            val = sVal;
            okVal = true;
        }

        int indexVal = indexInput->value();
        bool needsVal = (op == 0 || op == 1 || (op == 2 && (sel == 1 || sel == 4)) || (op == 3 && sel != 2 && sel != 3));

        if (needsVal && !okVal && op != 4 && op != 5) {
            QMessageBox::warning(this, "Input Required", "Please enter a valid integer.");
            if (op == 3) searchInput->setFocus();
            else valueInput->setFocus();
            return;
        }

        try {
            QString title;
            QString msg;
            int foundIdx = -1;
            QString codeFile = "algolib/data_structures.hpp";
            QString codeFunc;
            QString codeLines;

            if (sel == 0) { // Dynamic Array
                title = "Dynamic Array";
                if (op == 0) {
                    array.insert(val);
                    msg = QString("Inserted %1 at end. Current size: %2, capacity: %3.").arg(val).arg(array.size()).arg(array.capacity());
                    codeFunc = "DynamicArray::insert(int value)";
                    codeLines = "if (len == cap)\n    grow(); // 2x geometric capacity reallocation\n\ndata[len++] = value;";
                } else if (op == 1) {
                    array.insertAt(static_cast<std::size_t>(indexVal), val);
                    msg = QString("Inserted %1 at index [%2]. Current size: %3.").arg(val).arg(indexVal).arg(array.size());
                    codeFunc = "DynamicArray::insertAt(size_t index, int value)";
                    codeLines = "if (len == cap) grow();\nfor (std::size_t i = len; i > index; --i)\n    data[i] = data[i - 1];\ndata[index] = value;\n++len;";
                } else if (op == 2) {
                    if (okVal && array.search(val)) {
                        array.removeValue(val);
                        msg = QString("Removed first occurrence of value %1. Current size: %2.").arg(val).arg(array.size());
                        codeFunc = "DynamicArray::removeValue(int value)";
                        codeLines = "for (std::size_t i = 0; i < len; ++i) {\n    if (data[i] == value) {\n        removeAt(i);\n        return true;\n    }\n}\nreturn false;";
                    } else {
                        array.remove();
                        msg = QString("Removed last element. Current size: %1.").arg(array.size());
                        codeFunc = "DynamicArray::remove()";
                        codeLines = "if (len == 0)\n    throw std::runtime_error(\"Dynamic array is empty: cannot remove\");\n--len;";
                    }
                } else if (op == 3) {
                    foundIdx = array.indexOf(val);
                    msg = (foundIdx != -1) ? QString("Value %1 FOUND at index [%2].").arg(val).arg(foundIdx)
                                           : QString("Value %1 NOT found in array.").arg(val);
                    codeFunc = "DynamicArray::search(int value) const";
                    codeLines = "for (std::size_t i = 0; i < len; ++i) {\n    if (data[i] == value)\n        return true;\n}\nreturn false;";
                } else if (op == 4) {
                    msg = QString::fromStdString(array.display());
                    codeFunc = "DynamicArray::display() const";
                    codeLines = "for (std::size_t i = 0; i < len; ++i)\n    out << data[i] << \" \";\nout << \"]\\nSize: \" << len << \" | Capacity: \" << cap;";
                } else if (op == 5) {
                    array.clear();
                    msg = "Dynamic Array cleared successfully.";
                    codeFunc = "DynamicArray::clear()";
                    codeLines = "delete[] data;\ndata = nullptr;\nlen = 0;\ncap = 0;";
                }
            } else if (sel == 1) { // Linked List
                title = "Singly Linked List";
                if (op == 0) {
                    linkedList.insert(val);
                    msg = QString("Appended node with value %1. Total nodes: %2.").arg(val).arg(linkedList.size());
                    codeFunc = "LinkedList::insert(int value)";
                    codeLines = "Node* node = new Node(value);\nif (!head) { head = node; return; }\nNode* current = head;\nwhile (current->next) current = current->next;\ncurrent->next = node;";
                } else if (op == 1) {
                    linkedList.insertHead(val);
                    msg = QString("Inserted %1 at HEAD. Total nodes: %2.").arg(val).arg(linkedList.size());
                    codeFunc = "LinkedList::insertHead(int value)";
                    codeLines = "Node* node = new Node(value);\nnode->next = head;\nhead = node;";
                } else if (op == 2) {
                    bool rem = linkedList.remove(val);
                    msg = rem ? QString("Deleted node with value %1. Remaining nodes: %2.").arg(val).arg(linkedList.size())
                              : QString("Value %1 not found in linked list.").arg(val);
                    codeFunc = "LinkedList::remove(int value)";
                    codeLines = "while (current && current->data != value) {\n    previous = current; current = current->next;\n}\nif (!current) return false;\nif (previous) previous->next = current->next;\nelse head = current->next;\ndelete current; return true;";
                } else if (op == 3) {
                    bool f = linkedList.search(val);
                    msg = f ? QString("Value %1 FOUND in linked list.").arg(val)
                            : QString("Value %1 NOT found in linked list.").arg(val);
                    codeFunc = "LinkedList::search(int value) const";
                    codeLines = "Node* current = head;\nwhile (current) {\n    if (current->data == value) return true;\n    current = current->next;\n}\nreturn false;";
                } else if (op == 4) {
                    msg = QString::fromStdString(linkedList.display());
                    codeFunc = "LinkedList::display() const";
                    codeLines = "while (current) {\n    out << current->data << \" -> \";\n    current = current->next;\n}\nout << \"NULL\";";
                } else if (op == 5) {
                    linkedList.clear();
                    msg = "Linked list cleared.";
                    codeFunc = "LinkedList::clear()";
                    codeLines = "while (head) {\n    Node* temp = head;\n    head = head->next;\n    delete temp;\n}";
                }
            } else if (sel == 2) { // Stack
                title = "Stack (LIFO)";
                if (op == 0 || op == 1) {
                    stack.push(val);
                    msg = QString("Pushed %1 onto Stack. Top is now %1. Size: %2.").arg(val).arg(stack.size());
                    codeFunc = "Stack::push(int value)";
                    codeLines = "topNode = new Node(value, topNode);";
                } else if (op == 2) {
                    int p = stack.pop();
                    msg = QString("Popped %1 from Stack. New size: %2.").arg(p).arg(stack.size());
                    codeFunc = "Stack::pop()";
                    codeLines = "if (!topNode) throw std::runtime_error(\"Stack underflow\");\nNode* temp = topNode;\nint value = temp->data;\ntopNode = topNode->next;\ndelete temp;\nreturn value;";
                } else if (op == 3) {
                    int top = stack.peek();
                    msg = QString("PEEK: Top element is %1. Stack size: %2.").arg(top).arg(stack.size());
                    codeFunc = "Stack::peek() const";
                    codeLines = "if (!topNode) throw std::runtime_error(\"Stack is empty\");\nreturn topNode->data;";
                } else if (op == 4) {
                    msg = QString::fromStdString(stack.display());
                    codeFunc = "Stack::display() const";
                    codeLines = "while (current) {\n    out << \"|  \" << current->data << \"  |\\n\";\n    current = current->next;\n}";
                } else if (op == 5) {
                    stack.clear();
                    msg = "Stack cleared.";
                    codeFunc = "Stack::clear()";
                    codeLines = "while (topNode) {\n    Node* temp = topNode;\n    topNode = topNode->next;\n    delete temp;\n}";
                }
            } else if (sel == 3) { // Queue
                title = "Queue (FIFO)";
                if (op == 0 || op == 1) {
                    queue.enqueue(val);
                    msg = QString("Enqueued %1 into Queue. Size: %2.").arg(val).arg(queue.size());
                    codeFunc = "Queue::enqueue(int value)";
                    codeLines = "Node* node = new Node(value);\nif (!rearNode) {\n    frontNode = rearNode = node;\n} else {\n    rearNode->next = node; rearNode = node;\n}";
                } else if (op == 2) {
                    int d = queue.dequeue();
                    msg = QString("Dequeued %1 from Front of Queue. Size: %2.").arg(d).arg(queue.size());
                    codeFunc = "Queue::dequeue()";
                    codeLines = "if (!frontNode) throw std::runtime_error(\"Queue underflow\");\nNode* temp = frontNode;\nint value = temp->data;\nfrontNode = frontNode->next;\nif (!frontNode) rearNode = nullptr;\ndelete temp; return value;";
                } else if (op == 3) {
                    int front = queue.front();
                    msg = QString("FRONT: Front element is %1. Queue size: %2.").arg(front).arg(queue.size());
                    codeFunc = "Queue::front() const";
                    codeLines = "if (!frontNode) throw std::runtime_error(\"Queue is empty\");\nreturn frontNode->data;";
                } else if (op == 4) {
                    msg = QString::fromStdString(queue.display());
                    codeFunc = "Queue::display() const";
                    codeLines = "while (current) {\n    out << current->data << \" -> \";\n    current = current->next;\n}\nout << \"REAR\";";
                } else if (op == 5) {
                    queue.clear();
                    msg = "Queue cleared.";
                    codeFunc = "Queue::clear()";
                    codeLines = "while (frontNode) {\n    Node* temp = frontNode;\n    frontNode = frontNode->next;\n    delete temp;\n}\nrearNode = nullptr;";
                }
            } else if (sel == 4) { // BST
                title = "Binary Search Tree";
                if (op == 0 || op == 1) {
                    tree.insert(val);
                    msg = QString("Inserted %1 into BST. Distinct nodes: %2, Total elements: %3.")
                          .arg(val).arg(tree.size()).arg(tree.totalCount());
                    codeFunc = "BST::insertNode(Node* node, int value)";
                    codeLines = "if (!node) return new Node(value);\nif (value < node->data)\n    node->left = insertNode(node->left, value);\nelse if (value > node->data)\n    node->right = insertNode(node->right, value);\nelse\n    ++node->count; // Duplicate occurrence\nreturn node;";
                } else if (op == 2) {
                    bool rem = tree.remove(val);
                    msg = rem ? QString("Removed %1 from BST (decremented count or deleted node).").arg(val)
                              : QString("Value %1 not found in BST.").arg(val);
                    codeFunc = "BST::eraseNode(Node* node, int value)";
                    codeLines = "if (node->count > 1) { --node->count; return node; }\nif (!node->left) { Node* temp = node->right; delete node; return temp; }\nif (!node->right) { Node* temp = node->left; delete node; return temp; }\nNode* succ = node->right;\nwhile (succ->left) succ = succ->left;\nnode->data = succ->data; node->count = succ->count;\nnode->right = removeWholeNode(node->right, succ->data);";
                } else if (op == 3) {
                    bool f = tree.search(val);
                    msg = f ? QString("Value %1 FOUND in BST.").arg(val)
                            : QString("Value %1 NOT found in BST.").arg(val);
                    codeFunc = "BST::searchNode(Node* node, int value) const";
                    codeLines = "if (!node) return false;\nif (node->data == value) return true;\nif (value < node->data) return searchNode(node->left, value);\nreturn searchNode(node->right, value);";
                } else if (op == 4) {
                    msg = QString("INORDER (Sorted): %1\nPREORDER: %2\nPOSTORDER: %3")
                          .arg(QString::fromStdString(tree.inorder()))
                          .arg(QString::fromStdString(tree.preorder()))
                          .arg(QString::fromStdString(tree.postorder()));
                    codeFunc = "BST::inorderNode(Node* node, ostringstream& out) const";
                    codeLines = "if (!node) return;\ninorderNode(node->left, out);\nfor (int i = 0; i < node->count; ++i) out << node->data << \" \";\ninorderNode(node->right, out);";
                } else if (op == 5) {
                    tree.clear();
                    msg = "Binary Search Tree cleared.";
                    codeFunc = "BST::clear()";
                    codeLines = "destroy(root);\nroot = nullptr;";
                }
            } else if (sel == 5) { // Min Heap
                title = "Min Heap";
                if (op == 0 || op == 1) {
                    heap.insert(val);
                    msg = QString("Inserted %1 into Min Heap. Current minimum: %2. Total elements: %3.")
                          .arg(val).arg(heap.peekMin()).arg(heap.size());
                    codeFunc = "MinHeap::insert(int value)";
                    codeLines = "if (len == cap) grow();\ndata[len] = value;\nsiftUp(len);\n++len;";
                } else if (op == 2) {
                    int minV = heap.extractMin();
                    msg = QString("Extracted minimum element: %1. New size: %2.").arg(minV).arg(heap.size());
                    if (!heap.empty()) msg += QString(" New minimum: %1.").arg(heap.peekMin());
                    codeFunc = "MinHeap::extractMin()";
                    codeLines = "int result = data[0];\n--len;\nif (len > 0) {\n    data[0] = data[len];\n    siftDown(0);\n}\nreturn result;";
                } else if (op == 3) {
                    bool f = heap.search(val);
                    msg = f ? QString("Value %1 FOUND in Min Heap.").arg(val)
                            : QString("Value %1 NOT found in Min Heap.").arg(val);
                    codeFunc = "MinHeap::search(int value) const";
                    codeLines = "for (int i = 0; i < len; ++i) {\n    if (data[i] == value) return true;\n}\nreturn false;";
                } else if (op == 4) {
                    msg = QString::fromStdString(heap.display());
                    codeFunc = "MinHeap::display() const";
                    codeLines = "for (int i = 0; i < len; ++i) out << data[i] << \" \";\nif (len > 0) out << \"\\nMinimum: \" << data[0];";
                } else if (op == 5) {
                    heap.clear();
                    msg = "Min Heap cleared.";
                    codeFunc = "MinHeap::clear()";
                    codeLines = "delete[] data;\ndata = nullptr;\nlen = 0;\ncap = 0;";
                }
            }

            logToConsole(dsOutput, title, msg, "SUCCESS");
            updateDSVisualizer(foundIdx);
            dsCodePanel->setSnippet(codeFile, codeFunc, codeLines);

            valueInput->clear();
            searchInput->clear();
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
        root->setContentsMargins(16, 12, 16, 12);
        root->setSpacing(10);

        // Header
        QLabel* title = new QLabel("CompressX — Lossless Huffman Coding Engine");
        title->setStyleSheet("font-size: 20px; font-weight: bold; color: #34D399;");
        QLabel* desc = new QLabel("Bespoke Huffman Tree, Priority Queue, and bit-level file compressor & decompressor without STL.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(14);

        // Scrollable Left Action Panel
        QScrollArea* fpScroll = new QScrollArea;
        fpScroll->setWidgetResizable(true);
        fpScroll->setFixedWidth(340);
        fpScroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

        QWidget* fpContainer = new QWidget;
        QVBoxLayout* fpLayout = new QVBoxLayout(fpContainer);
        fpLayout->setContentsMargins(0, 0, 10, 0);
        fpLayout->setSpacing(10);

        QGroupBox* filePanel = new QGroupBox(" FILE COMPRESSION WORKFLOW ");
        QVBoxLayout* fpSubLayout = new QVBoxLayout(filePanel);
        fpSubLayout->setSpacing(10);

        QLabel* selectLabel = new QLabel("Target File (Enter ➔ Analyze):");
        QHBoxLayout* pickLayout = new QHBoxLayout;
        filePathInput = new QLineEdit;
        filePathInput->setPlaceholderText("Select file to compress...");
        QPushButton* browseBtn = new QPushButton("Browse...");
        browseBtn->setFixedWidth(80);
        pickLayout->addWidget(filePathInput);
        pickLayout->addWidget(browseBtn);

        fpSubLayout->addWidget(selectLabel);
        fpSubLayout->addLayout(pickLayout);

        QPushButton* analyzeBtn = new QPushButton("1. Analyze Frequencies & Codes");
        QPushButton* compressBtn = new QPushButton("2. Compress File (.cmpx)");
        compressBtn->setObjectName("primaryBtn");
        QPushButton* decompressBtn = new QPushButton("3. Decompress Archive");
        decompressBtn->setObjectName("successBtn");
        QPushButton* verifyBtn = new QPushButton("4. Verify Bit-for-Bit Integrity");

        fpSubLayout->addSpacing(6);
        fpSubLayout->addWidget(analyzeBtn);
        fpSubLayout->addWidget(compressBtn);
        fpSubLayout->addWidget(decompressBtn);
        fpSubLayout->addWidget(verifyBtn);

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

        fpLayout->addWidget(filePanel);
        fpLayout->addWidget(statsCard);
        fpLayout->addStretch();
        fpScroll->setWidget(fpContainer);

        // Right Results Area: Splitter (Stats/Info on Left, Code on Right)
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(10);

        QSplitter* compSplitter = new QSplitter(Qt::Horizontal);

        QGroupBox* visualGroup = new QGroupBox(" ALGORITHM WORKFLOW ");
        QVBoxLayout* vgLayout = new QVBoxLayout(visualGroup);
        QLabel* flowChart = new QLabel(
            "<b>Lossless Huffman Coding Pipeline:</b><br><br>"
            "1. <b>Frequency Counting:</b> Tally byte distribution across 256 bins.<br>"
            "2. <b>Priority Queue:</b> Sift nodes into custom binary min-heap.<br>"
            "3. <b>Canonical Tree:</b> Greedily merge 2 lowest-frequency nodes.<br>"
            "4. <b>Prefix Codes:</b> Recursive traversal assigning binary bitstrings.<br>"
            "5. <b>Bit Serialization:</b> BitWriter streams packed bitstream to .cmpx.<br>"
            "6. <b>Decompression:</b> BitReader traverses tree root to reconstruct exact bytes."
        );
        flowChart->setStyleSheet("color: #CBD5E1; line-height: 150%; font-size: 12px;");
        flowChart->setWordWrap(true);
        vgLayout->addWidget(flowChart);
        vgLayout->addStretch();

        compressCodePanel = new CodeSnippetPanel(this);

        compSplitter->addWidget(visualGroup);
        compSplitter->addWidget(compressCodePanel);
        compSplitter->setStretchFactor(0, 3);
        compSplitter->setStretchFactor(1, 2);

        rightLayout->addWidget(compSplitter, 3);

        QGroupBox* outGroup = new QGroupBox(" COMPRESSX ENGINE CONSOLE ");
        QVBoxLayout* outLayout = new QVBoxLayout(outGroup);
        outLayout->setContentsMargins(8, 8, 8, 8);
        compressOutput = new QTextEdit;
        compressOutput->setReadOnly(true);
        outLayout->addWidget(compressOutput);

        rightLayout->addWidget(outGroup, 2);

        mainLayout->addWidget(fpScroll);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Connections
        connect(browseBtn, &QPushButton::clicked, this, [this]() {
            QString path = QFileDialog::getOpenFileName(this, "Select File to Compress/Decompress");
            if (!path.isEmpty()) filePathInput->setText(path);
        });

        auto runAnalyze = [this]() {
            QString path = filePathInput->text().trimmed();
            if (path.isEmpty() || !QFileInfo::exists(path)) {
                QMessageBox::warning(this, "File Error", "Please select a valid existing file first.");
                return;
            }
            std::string analysis = CompressX::analyzeFrequenciesAndCodes(path.toStdString());
            compressOutput->clear();
            logToConsole(compressOutput, "Frequency & Code Analysis", QString::fromStdString(analysis), "INFO");
            compressCodePanel->setSnippet(
                "src/compressx.cpp",
                "CompressX::analyzeFrequenciesAndCodes()",
                "while (inFile.read(readBuf, sizeof(readBuf)) || inFile.gcount() > 0) {\n    std::streamsize bytesRead = inFile.gcount();\n    for (std::streamsize i = 0; i < bytesRead; ++i)\n        ++frequencies[static_cast<uint8_t>(readBuf[i])];\n}"
            );
        };

        auto runCompress = [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty() || !QFileInfo::exists(inPath)) {
                QMessageBox::warning(this, "File Error", "Please select a valid existing file to compress.");
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

                compressCodePanel->setSnippet(
                    "src/compressx.cpp",
                    "CompressX::compressFile()",
                    "BitWriter bitWriter(outFile);\nwhile (reInFile.read(readBuf, sizeof(readBuf)) || reInFile.gcount() > 0) {\n    for (std::streamsize i = 0; i < reInFile.gcount(); ++i) {\n        uint8_t byteVal = static_cast<uint8_t>(readBuf[i]);\n        bitWriter.writeCode(codeTable[byteVal].bits, codeTable[byteVal].len);\n    }\n}\nbitWriter.flush();"
                );
            } else {
                logToConsole(compressOutput, "Compression Failed", QString::fromStdString(stats.errorMessage), "ERROR");
            }
        };

        auto runDecompress = [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select a file or .cmpx archive.");
                return;
            }
            QString compPath = inPath.endsWith(".cmpx") ? inPath : inPath + ".cmpx";
            if (!QFileInfo::exists(compPath)) {
                QMessageBox::warning(this, "Archive Error", "Compressed archive does not exist: " + compPath);
                return;
            }
            QString outPath = compPath;
            if (outPath.endsWith(".cmpx")) outPath.chop(5);
            outPath += ".restored";

            CompressionStats stats = CompressX::decompressFile(compPath.toStdString(), outPath.toStdString());

            if (stats.success) {
                QString msg = QString("Archive decompressed successfully!\nRestored File: %1\nRestored Bytes: %2 bytes")
                              .arg(outPath).arg(stats.originalBytes);
                logToConsole(compressOutput, "Decompression Complete", msg, "SUCCESS");

                compressCodePanel->setSnippet(
                    "src/compressx.cpp",
                    "CompressX::decompressFile()",
                    "while (decodedBytes < originalSize) {\n    int bit = bitReader.readBit();\n    current = (bit == 0) ? current->left : current->right;\n    if (!current->left && !current->right) {\n        outFile.put(static_cast<char>(current->byteVal));\n        ++decodedBytes; current = root;\n    }\n}"
                );
            } else {
                logToConsole(compressOutput, "Decompression Failed", QString::fromStdString(stats.errorMessage), "ERROR");
            }
        };

        auto runVerify = [this]() {
            QString inPath = filePathInput->text().trimmed();
            if (inPath.isEmpty()) {
                QMessageBox::warning(this, "File Error", "Please select the original file to verify.");
                return;
            }
            QString origPath = inPath.endsWith(".cmpx") ? inPath.left(inPath.length() - 5) : inPath;
            QString restPath = origPath + ".restored";

            if (!QFileInfo::exists(origPath) || !QFileInfo::exists(restPath)) {
                QMessageBox::warning(this, "Verification Error", "Both original and restored files must exist to verify. Please compress and decompress first.");
                return;
            }

            bool match = CompressX::verifyFilesMatch(origPath.toStdString(), restPath.toStdString());

            if (match) {
                verifyBadge->setText("STATUS: Byte-for-byte exact match! ✓");
                verifyBadge->setStyleSheet("color: #34D399; font-weight: bold;");
                logToConsole(compressOutput, "Integrity Verification", "SUCCESS: The decompressed file is a 100% byte-for-byte exact match to the original input file!", "SUCCESS");

                compressCodePanel->setSnippet(
                    "src/compressx.cpp",
                    "CompressX::verifyFilesMatch()",
                    "while (f1.good() && f2.good()) {\n    f1.read(buf1, sizeof(buf1));\n    f2.read(buf2, sizeof(buf2));\n    if (f1.gcount() != f2.gcount() || std::memcmp(buf1, buf2, f1.gcount()) != 0)\n        return false;\n}\nreturn true;"
                );
            } else {
                verifyBadge->setText("STATUS: Mismatch detected ✕");
                verifyBadge->setStyleSheet("color: #F87171; font-weight: bold;");
                logToConsole(compressOutput, "Integrity Verification", "FAILED: Mismatch detected between original and restored files.", "ERROR");
            }
        };

        connect(analyzeBtn, &QPushButton::clicked, this, runAnalyze);
        connect(compressBtn, &QPushButton::clicked, this, runCompress);
        connect(decompressBtn, &QPushButton::clicked, this, runDecompress);
        connect(verifyBtn, &QPushButton::clicked, this, runVerify);

        // Enter key in filePathInput triggers analyze (or decompress if .cmpx)
        connect(filePathInput, &QLineEdit::returnPressed, this, [this, runAnalyze, runDecompress]() {
            if (filePathInput->text().trimmed().endsWith(".cmpx")) runDecompress();
            else runAnalyze();
        });

        return page;
    }


    // ==================================================
    // 3. GRAPHKIT TAB (REPAIRED & REDESIGNED LAYOUT)
    // ==================================================
    QWidget* createGraphKitTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(16, 12, 16, 12);
        root->setSpacing(10);

        // Header
        QLabel* title = new QLabel("GraphKit — Graph Theory Algorithms & Visualization");
        title->setStyleSheet("font-size: 20px; font-weight: bold; color: #60A5FA;");
        QLabel* desc = new QLabel("Custom Adjacency List graph with BFS, DFS, Dijkstra Shortest Path, and Kruskal's MST (with full disconnected graph support).");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(14);

        // Scrollable Left Graph Control Panel (Fixed Width with comfortable scrolling)
        QScrollArea* gScroll = new QScrollArea;
        gScroll->setWidgetResizable(true);
        gScroll->setFixedWidth(340);
        gScroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

        QWidget* gContainer = new QWidget;
        QVBoxLayout* gpLayout = new QVBoxLayout(gContainer);
        gpLayout->setContentsMargins(0, 0, 10, 0);
        gpLayout->setSpacing(10);

        // 1. Add Vertex Group
        QGroupBox* vGroup = new QGroupBox(" 1. ADD VERTEX ");
        QVBoxLayout* vgLayout = new QVBoxLayout(vGroup);
        vgLayout->setSpacing(8);
        QHBoxLayout* addVLayout = new QHBoxLayout;
        vertexInput = new QLineEdit;
        vertexInput->setPlaceholderText("Label (e.g. G)");
        QPushButton* addVBtn = new QPushButton("Add Node");
        addVBtn->setFixedWidth(90);
        addVBtn->setObjectName("primaryBtn");
        addVLayout->addWidget(vertexInput);
        addVLayout->addWidget(addVBtn);
        vgLayout->addLayout(addVLayout);
        gpLayout->addWidget(vGroup);

        // 2. Add Weighted Edge Group
        QGroupBox* eGroup = new QGroupBox(" 2. CONNECT WEIGHTED EDGE ");
        QGridLayout* egLayout = new QGridLayout(eGroup);
        egLayout->setSpacing(8);
        edgeFromBox = new QComboBox;
        edgeToBox = new QComboBox;
        edgeWeightBox = new QSpinBox;
        edgeWeightBox->setRange(1, 1000);
        edgeWeightBox->setValue(5);
        QPushButton* addEdgeBtn = new QPushButton("Connect Edge");
        addEdgeBtn->setObjectName("primaryBtn");

        egLayout->addWidget(new QLabel("From:"), 0, 0);
        egLayout->addWidget(edgeFromBox, 0, 1);
        egLayout->addWidget(new QLabel("To:"), 1, 0);
        egLayout->addWidget(edgeToBox, 1, 1);
        egLayout->addWidget(new QLabel("Weight:"), 2, 0);
        egLayout->addWidget(edgeWeightBox, 2, 1);
        egLayout->addWidget(addEdgeBtn, 3, 0, 1, 2);
        gpLayout->addWidget(eGroup);

        // 3. Presets & Actions Group
        QGroupBox* pGroup = new QGroupBox(" 3. GRAPH PRESETS ");
        QVBoxLayout* pgLayout = new QVBoxLayout(pGroup);
        pgLayout->setSpacing(6);
        QPushButton* loadSampleBtn = new QPushButton("Load Connected Preset");
        QPushButton* loadDiscBtn = new QPushButton("Load Disconnected Preset");
        QPushButton* clearGBtn = new QPushButton("Clear Graph");
        clearGBtn->setObjectName("dangerBtn");
        pgLayout->addWidget(loadSampleBtn);
        pgLayout->addWidget(loadDiscBtn);
        pgLayout->addWidget(clearGBtn);
        gpLayout->addWidget(pGroup);

        // 4. Algorithm Selection & Execution Group
        QGroupBox* algoBox = new QGroupBox(" 4. RUN ALGORITHMS ");
        QVBoxLayout* abLayout = new QVBoxLayout(algoBox);
        abLayout->setSpacing(8);

        graphAlgoBox = new QComboBox;
        graphAlgoBox->addItems({
            "1. Breadth First Search (BFS)",
            "2. Depth First Search (DFS)",
            "3. Dijkstra's Shortest Path",
            "4. Kruskal's Minimum Spanning Tree"
        });

        QHBoxLayout* startRow = new QHBoxLayout;
        algoStartBox = new QComboBox;
        startRow->addWidget(new QLabel("Start:"));
        startRow->addWidget(algoStartBox);

        QHBoxLayout* targetRow = new QHBoxLayout;
        targetLabel = new QLabel("Target:");
        algoTargetBox = new QComboBox;
        targetRow->addWidget(targetLabel);
        targetRow->addWidget(algoTargetBox);

        allComponentsCheck = new QCheckBox("Traverse All Disconnected Components");
        allComponentsCheck->setChecked(true);

        QPushButton* runAlgoBtn = new QPushButton("Execute Selected Algorithm");
        runAlgoBtn->setObjectName("successBtn");

        abLayout->addWidget(graphAlgoBox);
        abLayout->addLayout(startRow);
        abLayout->addLayout(targetRow);
        abLayout->addWidget(allComponentsCheck);
        abLayout->addWidget(runAlgoBtn);
        gpLayout->addWidget(algoBox);

        gpLayout->addStretch();
        gScroll->setWidget(gContainer);

        // Right Results Area: Splitter (Graph Visualization on Left, Code on Right)
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(10);

        QSplitter* graphSplitter = new QSplitter(Qt::Horizontal);
        graphVisualizer = new GraphVisualizer(this);
        graphCodePanel = new CodeSnippetPanel(this);

        graphSplitter->addWidget(graphVisualizer);
        graphSplitter->addWidget(graphCodePanel);
        graphSplitter->setStretchFactor(0, 3);
        graphSplitter->setStretchFactor(1, 2);

        rightLayout->addWidget(graphSplitter, 3);

        // Bottom Path Trace Console
        QGroupBox* gOutBox = new QGroupBox(" GRAPH ANALYSIS & PATH TRACE ");
        QVBoxLayout* gobLayout = new QVBoxLayout(gOutBox);
        gobLayout->setContentsMargins(8, 8, 8, 8);
        graphOutput = new QTextEdit;
        graphOutput->setReadOnly(true);
        gobLayout->addWidget(graphOutput);

        rightLayout->addWidget(gOutBox, 2);

        mainLayout->addWidget(gScroll);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Algorithm View Switcher
        connect(graphAlgoBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
            bool isDijkstra = (idx == 2);
            bool isTraversal = (idx == 0 || idx == 1);
            algoTargetBox->setVisible(isDijkstra);
            targetLabel->setVisible(isDijkstra);
            allComponentsCheck->setVisible(isTraversal);
        });

        // Add Vertex Action with Validation
        auto runAddVertex = [this]() {
            QString lbl = vertexInput->text().trimmed();
            if (lbl.isEmpty()) {
                QMessageBox::warning(this, "Invalid Vertex", "Please enter a non-empty vertex label.");
                vertexInput->setFocus();
                return;
            }
            if (graph.getVertexIndex(lbl.toStdString()) != -1) {
                QMessageBox::warning(this, "Duplicate Vertex", QString("Vertex '%1' already exists in the graph.").arg(lbl));
                vertexInput->selectAll();
                vertexInput->setFocus();
                return;
            }
            graph.addVertex(lbl.toStdString());
            vertexInput->clear();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Vertex Added", QString("Added vertex '%1'. Total vertices: %2.").arg(lbl).arg(graph.getVertexCount()), "INFO");
            graphCodePanel->setSnippet(
                "src/graphkit.cpp",
                "Graph::addVertex(const string& label)",
                "if (getVertexIndex(label) != -1) return -1;\nif (vertexCount == vertexCapacity) growVertices();\nchar* str = new char[label.length() + 1];\nstd::strcpy(str, label.c_str());\nvertexLabels[vertexCount] = str;\nreturn vertexCount++;"
            );
            vertexInput->setFocus();
        };

        // Add Edge Action with Validation
        auto runAddEdge = [this]() {
            if (graph.getVertexCount() < 2) {
                QMessageBox::warning(this, "Insufficient Vertices", "Please add at least 2 vertices to connect an edge.");
                return;
            }
            if (edgeFromBox->currentIndex() < 0 || edgeToBox->currentIndex() < 0) {
                QMessageBox::warning(this, "Invalid Edge", "Please select valid source and target vertices.");
                return;
            }
            int u = edgeFromBox->currentIndex();
            int v = edgeToBox->currentIndex();
            int w = edgeWeightBox->value();

            if (w <= 0) {
                QMessageBox::warning(this, "Invalid Weight", "Edge weight must be positive.");
                return;
            }

            bool existed = graph.hasEdge(u, v);
            graph.addEdge(u, v, w);
            graphVisualizer->setGraph(&graph);

            QString uLbl = QString::fromStdString(graph.getVertexLabel(u));
            QString vLbl = QString::fromStdString(graph.getVertexLabel(v));

            if (u == v) {
                logToConsole(graphOutput, "Self-Loop Connected", QString("Connected self-loop on vertex '%1' with weight %2.").arg(uLbl).arg(w), "INFO");
            } else if (existed) {
                logToConsole(graphOutput, "Edge Updated", QString("Updated existing edge %1 <--> %2 with weight %3.").arg(uLbl, vLbl).arg(w), "INFO");
            } else {
                logToConsole(graphOutput, "Edge Connected", QString("Connected edge %1 <--> %2 (Weight: %3).").arg(uLbl, vLbl).arg(w), "INFO");
            }

            graphCodePanel->setSnippet(
                "src/graphkit.cpp",
                "Graph::addEdge(int u, int v, int weight)",
                "adjList[u] = new EdgeNode(v, weight, adjList[u]);\nif (!directed && u != v) {\n    adjList[v] = new EdgeNode(u, weight, adjList[v]);\n}\nreturn true;"
            );
        };

        // Run Graph Algorithm with Validation
        auto runAlgorithm = [this]() {
            if (graph.getVertexCount() == 0) {
                QMessageBox::warning(this, "Empty Graph", "Please add vertices and edges or load a preset first.");
                return;
            }

            int algo = graphAlgoBox->currentIndex();
            int start = algoStartBox->currentIndex();
            int target = algoTargetBox->currentIndex();
            bool visitAll = allComponentsCheck->isChecked();

            if (start < 0 || start >= graph.getVertexCount()) {
                QMessageBox::warning(this, "Invalid Vertex", "Please select a valid start vertex.");
                return;
            }

            if (algo == 0) { // BFS
                BFSResult res = graph.bfs(start, visitAll);
                logToConsole(graphOutput, "BFS Traversal", QString::fromStdString(res.summary), "SUCCESS");
                graphVisualizer->highlightTraversal(res.visitedOrder, res.visitedCount, QColor("#F59E0B"));
                delete[] res.visitedOrder;

                graphCodePanel->setSnippet(
                    "src/graphkit.cpp",
                    "Graph::bfs(int startVertex, bool visitAllComponents) const",
                    "visited[root] = true; queue[qRear++] = root;\nwhile (qFront < qRear) {\n    int u = queue[qFront++]; order[orderIdx++] = u;\n    for (EdgeNode* cur = adjList[u]; cur; cur = cur->next) {\n        if (!visited[cur->to]) {\n            visited[cur->to] = true;\n            queue[qRear++] = cur->to;\n        }\n    }\n}"
                );
            } else if (algo == 1) { // DFS
                DFSResult res = graph.dfs(start, visitAll);
                logToConsole(graphOutput, "DFS Traversal", QString::fromStdString(res.summary), "SUCCESS");
                graphVisualizer->highlightTraversal(res.visitedOrder, res.visitedCount, QColor("#F59E0B"));
                delete[] res.visitedOrder;

                graphCodePanel->setSnippet(
                    "src/graphkit.cpp",
                    "Graph::dfs(int startVertex, bool visitAllComponents) const",
                    "stack[top++] = root;\nwhile (top > 0) {\n    int u = stack[--top];\n    if (!visited[u]) {\n        visited[u] = true; order[orderIdx++] = u;\n        for (EdgeNode* cur = adjList[u]; cur; cur = cur->next)\n            if (!visited[cur->to]) stack[top++] = cur->to;\n    }\n}"
                );
            } else if (algo == 2) { // Dijkstra
                if (target < 0 || target >= graph.getVertexCount()) {
                    QMessageBox::warning(this, "Invalid Target", "Please select a valid target vertex for Dijkstra's shortest path.");
                    return;
                }
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

                graphCodePanel->setSnippet(
                    "src/graphkit.cpp",
                    "Graph::dijkstra(int startVertex, int targetVertex) const",
                    "pq.push(startVertex, 0); dist[startVertex] = 0;\nwhile (!pq.empty()) {\n    int u = pq.pop().vertex;\n    if (visited[u]) continue; visited[u] = true;\n    for (EdgeNode* edge = adjList[u]; edge; edge = edge->next) {\n        if (!visited[edge->to] && dist[u] + edge->weight < dist[edge->to]) {\n            dist[edge->to] = dist[u] + edge->weight; prev[edge->to] = u;\n            pq.push(edge->to, dist[edge->to]);\n        }\n    }\n}"
                );
            } else if (algo == 3) { // Kruskal MST
                KruskalResult res = graph.kruskalMST();
                QString status = res.isConnected ? "SUCCESS" : "WARNING";
                logToConsole(graphOutput, "Kruskal Minimum Spanning Tree", QString::fromStdString(res.summary), status);
                graphVisualizer->highlightMST(res.mstEdges, res.mstEdgeCount, QColor("#10B981"));
                delete[] res.mstEdges;

                graphCodePanel->setSnippet(
                    "src/graphkit.cpp",
                    "Graph::kruskalMST() const",
                    "customSortEdges(allEdges, 0, totalEdges - 1);\nDisjointSet dsu(vertexCount);\nfor (int i = 0; i < totalEdges; ++i) {\n    if (dsu.unite(allEdges[i].u, allEdges[i].v)) {\n        mst[mstCount++] = allEdges[i];\n        totalCost += allEdges[i].weight;\n        if (mstCount == vertexCount - 1) break;\n    }\n}"
                );
            }
        };

        connect(addVBtn, &QPushButton::clicked, this, runAddVertex);
        connect(addEdgeBtn, &QPushButton::clicked, this, runAddEdge);
        connect(runAlgoBtn, &QPushButton::clicked, this, runAlgorithm);

        connect(loadSampleBtn, &QPushButton::clicked, this, [this]() {
            graph.loadSampleGraph();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Sample Graph Loaded", "Loaded 6-vertex connected weighted graph preset.", "SUCCESS");
            graphCodePanel->showEmptyState();
        });

        connect(loadDiscBtn, &QPushButton::clicked, this, [this]() {
            graph.loadDisconnectedSample();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Disconnected Preset Loaded", "Loaded disconnected graph preset with 2 separate components (A-B-C-D and X-Y-Z).", "WARNING");
            graphCodePanel->showEmptyState();
        });

        connect(clearGBtn, &QPushButton::clicked, this, [this]() {
            graph.clear();
            refreshGraphVertexCombos();
            graphVisualizer->setGraph(&graph);
            logToConsole(graphOutput, "Graph Cleared", "All vertices and edges have been removed.", "INFO");
            graphCodePanel->showEmptyState();
        });

        // Enter key support for GraphKit
        connect(vertexInput, &QLineEdit::returnPressed, this, runAddVertex);
        if (QLineEdit* edit = edgeWeightBox->findChild<QLineEdit*>()) {
            connect(edit, &QLineEdit::returnPressed, this, runAddEdge);
        }

        // Initial default sample
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

        if (edgeToBox->count() > 1) edgeToBox->setCurrentIndex(1);
        if (algoTargetBox->count() > 1) algoTargetBox->setCurrentIndex(algoTargetBox->count() - 1);
    }


    // ==================================================
    // 4. DP LAB TAB
    // ==================================================
    QWidget* createDPLabTab() {
        QWidget* page = new QWidget;
        QVBoxLayout* root = new QVBoxLayout(page);
        root->setContentsMargins(16, 12, 16, 12);
        root->setSpacing(10);

        // Header
        QLabel* title = new QLabel("DP Lab — Dynamic Programming Interactive Laboratory");
        title->setStyleSheet("font-size: 20px; font-weight: bold; color: #A78BFA;");
        QLabel* desc = new QLabel("Tabulation, 2D matrix state tracking, and optimal traceback with live C++ source synchronization.");
        desc->setStyleSheet("color: #94A3B8; font-size: 12px;");
        root->addWidget(title);
        root->addWidget(desc);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->setSpacing(14);

        // Scrollable Left Problem Panel
        QScrollArea* dpScroll = new QScrollArea;
        dpScroll->setWidgetResizable(true);
        dpScroll->setFixedWidth(340);
        dpScroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

        QWidget* dpContainer = new QWidget;
        QVBoxLayout* ppLayout = new QVBoxLayout(dpContainer);
        ppLayout->setContentsMargins(0, 0, 10, 0);
        ppLayout->setSpacing(10);

        QGroupBox* pPanel = new QGroupBox(" PROBLEM CONFIGURATION ");
        QVBoxLayout* pSubLayout = new QVBoxLayout(pPanel);
        pSubLayout->setSpacing(10);

        QLabel* selLabel = new QLabel("Select DP Problem:");
        dpProblemBox = new QComboBox;
        dpProblemBox->addItems({
            "1. Fibonacci Sequence",
            "2. 0/1 Knapsack",
            "3. Longest Common Subsequence",
            "4. Coin Change (Fewest Coins)"
        });

        pSubLayout->addWidget(selLabel);
        pSubLayout->addWidget(dpProblemBox);

        // Dynamic inputs stack
        dpInputsStack = new QStackedWidget;

        // 0: Fib
        QWidget* fibPage = new QWidget;
        QVBoxLayout* fibLayout = new QVBoxLayout(fibPage);
        fibLayout->addWidget(new QLabel("Nth Fibonacci Term (0..90, Enter ➔ Solve):"));
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

        ksLayout->addWidget(new QLabel("Item Values (comma separated, Enter ➔ Solve):"));
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

        lcsLayout->addWidget(new QLabel("String B (Enter ➔ Solve):"));
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

        ccLayout->addWidget(new QLabel("Coins (comma separated, Enter ➔ Solve):"));
        ccCoinsInput = new QLineEdit("1, 2, 5");
        ccLayout->addWidget(ccCoinsInput);
        ccLayout->addStretch();
        dpInputsStack->addWidget(ccPage);

        pSubLayout->addWidget(dpInputsStack);

        QPushButton* solveBtn = new QPushButton("Compute Optimal Solution");
        solveBtn->setObjectName("primaryBtn");
        pSubLayout->addWidget(solveBtn);

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

        ppLayout->addWidget(pPanel);
        ppLayout->addWidget(compCard);
        ppLayout->addStretch();
        dpScroll->setWidget(dpContainer);

        // Right Results Area: Splitter (DP Table on Left, Code on Right)
        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->setSpacing(10);

        QSplitter* dpSplitter = new QSplitter(Qt::Horizontal);

        QGroupBox* tableBox = new QGroupBox(" DYNAMIC PROGRAMMING STATE MATRIX / TABLE ");
        QVBoxLayout* tbLayout = new QVBoxLayout(tableBox);
        tbLayout->setContentsMargins(8, 8, 8, 8);
        dpTableWidget = new QTableWidget;
        tbLayout->addWidget(dpTableWidget);

        dpCodePanel = new CodeSnippetPanel(this);

        dpSplitter->addWidget(tableBox);
        dpSplitter->addWidget(dpCodePanel);
        dpSplitter->setStretchFactor(0, 3);
        dpSplitter->setStretchFactor(1, 2);

        rightLayout->addWidget(dpSplitter, 3);

        QGroupBox* dpOutBox = new QGroupBox(" TRACEBACK & RECONSTRUCTION EXPLANATION ");
        QVBoxLayout* dobLayout = new QVBoxLayout(dpOutBox);
        dobLayout->setContentsMargins(8, 8, 8, 8);
        dpOutput = new QTextEdit;
        dpOutput->setReadOnly(true);
        dobLayout->addWidget(dpOutput);

        rightLayout->addWidget(dpOutBox, 2);

        mainLayout->addWidget(dpScroll);
        mainLayout->addLayout(rightLayout, 1);
        root->addLayout(mainLayout, 1);

        // Problem Switcher
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

        // Solve Button
        connect(solveBtn, &QPushButton::clicked, this, &AlgoLibWindow::solveCurrentDPProblem);

        // Enter key support for DP Lab
        if (QLineEdit* edit = fibNSpin->findChild<QLineEdit*>()) {
            connect(edit, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        }
        if (QLineEdit* edit = ksCapSpin->findChild<QLineEdit*>()) {
            connect(edit, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        }
        connect(ksWeightsInput, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        connect(ksValuesInput, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        connect(lcsStrAInput, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        connect(lcsStrBInput, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        if (QLineEdit* edit = ccAmountSpin->findChild<QLineEdit*>()) {
            connect(edit, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);
        }
        connect(ccCoinsInput, &QLineEdit::returnPressed, this, &AlgoLibWindow::solveCurrentDPProblem);

        // Run initial solution
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
            for (int i = 0; i <= n; ++i) hHeaders << QString::number(i);
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

            dpCodePanel->setSnippet(
                "src/dplab.cpp",
                "DPLab::computeFibonacci(int n)",
                "sequence[0] = 0; sequence[1] = 1;\nfor (int i = 2; i <= n; ++i) {\n    sequence[i] = sequence[i - 1] + sequence[i - 2];\n}\nresult = sequence[n];"
            );

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
            for (int w = 0; w <= cap; ++w) hHeaders << QString("w=%1").arg(w);
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

            dpCodePanel->setSnippet(
                "src/dplab.cpp",
                "DPLab::solveKnapsack(...)",
                "for (int i = 1; i <= count; ++i) {\n    for (int w = 0; w <= capacity; ++w) {\n        if (weights[i - 1] <= w)\n            dpTable[i][w] = std::max(dpTable[i - 1][w], values[i - 1] + dpTable[i - 1][w - weights[i - 1]]);\n        else\n            dpTable[i][w] = dpTable[i - 1][w];\n    }\n}"
            );

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
            for (int i = 0; i < m; ++i) vHeaders << QString("A[%1] = '%2'").arg(i).arg(sA[i]);
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            hHeaders << "Ø";
            for (int j = 0; j < n; ++j) hHeaders << QString("B[%1] = '%2'").arg(j).arg(sB[j]);
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

            dpCodePanel->setSnippet(
                "src/dplab.cpp",
                "DPLab::solveLCS(const string& strA, const string& strB)",
                "for (int i = 1; i <= m; ++i) {\n    for (int j = 1; j <= n; ++j) {\n        if (strA[i - 1] == strB[j - 1])\n            dpTable[i][j] = 1 + dpTable[i - 1][j - 1];\n        else\n            dpTable[i][j] = std::max(dpTable[i - 1][j], dpTable[i][j - 1]);\n    }\n}"
            );

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
            for (int i = 0; i < count; ++i) coins[i] = cStrs[i].trimmed().toInt();

            CoinChangeResult res = DPLab::solveCoinChange(amount, coins, count);

            dpTableWidget->clear();
            dpTableWidget->setRowCount(2);
            dpTableWidget->setColumnCount(amount + 1);

            QStringList vHeaders;
            vHeaders << "Amount" << "Min Coins";
            dpTableWidget->setVerticalHeaderLabels(vHeaders);

            QStringList hHeaders;
            for (int i = 0; i <= amount; ++i) hHeaders << QString::number(i);
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

            dpCodePanel->setSnippet(
                "src/dplab.cpp",
                "DPLab::solveCoinChange(int amount, const int* coins, int count)",
                "dpTable[0] = 0;\nfor (int i = 1; i <= amount; ++i) {\n    for (int c = 0; c < count; ++c) {\n        if (coins[c] <= i && dpTable[i - coins[c]] != INF_COINS) {\n            if (1 + dpTable[i - coins[c]] < dpTable[i])\n                dpTable[i] = 1 + dpTable[i - coins[c]];\n        }\n    }\n}"
            );

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
