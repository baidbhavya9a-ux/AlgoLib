#include "algolib/code_panel.hpp"

#include <QGuiApplication>
#include <QClipboard>
#include <QTimer>
#include <QRegularExpression>

namespace algolib {

CodeSnippetPanel::CodeSnippetPanel(QWidget* parent)
    : QWidget(parent)
{
    setMinimumWidth(320);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(6);

    // Header bar
    QWidget* headerWidget = new QWidget;
    headerWidget->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A2A;
            border: 1px solid #35354B;
            border-radius: 6px;
        }
    )");
    QHBoxLayout* hLayout = new QHBoxLayout(headerWidget);
    hLayout->setContentsMargins(10, 6, 10, 6);
    hLayout->setSpacing(10);

    fileBadge = new QLabel("📄 Source Implementation");
    fileBadge->setStyleSheet("color: #60A5FA; font-weight: bold; font-size: 11px;");

    functionBadge = new QLabel("");
    functionBadge->setStyleSheet("color: #A78BFA; font-family: 'Consolas', monospace; font-size: 11px;");

    copyBtn = new QPushButton("Copy");
    copyBtn->setFixedWidth(60);
    copyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #26263B;
            color: #CBD5E1;
            border: 1px solid #41415A;
            border-radius: 4px;
            font-size: 11px;
            padding: 3px 8px;
        }
        QPushButton:hover {
            background-color: #383854;
            color: #FFFFFF;
        }
    )");

    hLayout->addWidget(fileBadge);
    hLayout->addWidget(functionBadge);
    hLayout->addStretch();
    hLayout->addWidget(copyBtn);

    root->addWidget(headerWidget);

    // Code Editor
    codeEditor = new QTextEdit;
    codeEditor->setReadOnly(true);
    codeEditor->setAcceptRichText(true);
    codeEditor->setStyleSheet(R"(
        QTextEdit {
            background-color: #11111B;
            color: #E2E8F0;
            border: 1px solid #2F2F45;
            border-radius: 6px;
            padding: 8px;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 12px;
            line-height: 140%;
        }
    )");

    root->addWidget(codeEditor);

    connect(copyBtn, &QPushButton::clicked, this, [this]() {
        if (!currentRawCode.isEmpty()) {
            QGuiApplication::clipboard()->setText(currentRawCode);
            copyBtn->setText("Copied! ✓");
            QTimer::singleShot(1500, this, [this]() {
                copyBtn->setText("Copy");
            });
        }
    });

    showEmptyState();
}

void CodeSnippetPanel::showEmptyState() {
    fileBadge->setText("📄 C++ Execution Inspector");
    functionBadge->setText("");
    currentRawCode.clear();

    QString emptyHtml = R"(
        <div style="text-align: center; color: #64748B; padding-top: 55px; font-family: 'Segoe UI', sans-serif;">
            <div style="font-size: 26px; margin-bottom: 10px;">⚡</div>
            <div style="font-size: 13px; font-weight: bold; color: #94A3B8; margin-bottom: 6px;">
                Synchronized Code Panel
            </div>
            <div style="font-size: 12px; color: #64748B;">
                Execute an operation to view its visualization and source code.
            </div>
        </div>
    )";
    codeEditor->setHtml(emptyHtml);
}

QString CodeSnippetPanel::formatHighlightedCode(const QString& rawCode) {
    QStringList lines = rawCode.split('\n');
    QString html;

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];

        // Escape HTML
        line = line.toHtmlEscaped();

        // Convert leading spaces to &nbsp;
        int leadingSpaces = 0;
        while (leadingSpaces < line.length() && line[leadingSpaces] == ' ')
            ++leadingSpaces;
        QString indent;
        for (int s = 0; s < leadingSpaces; ++s)
            indent += "&nbsp;";
        line = indent + line.mid(leadingSpaces);

        // Highlight comments
        if (line.contains("//")) {
            int commentPos = line.indexOf("//");
            QString before = line.left(commentPos);
            QString comment = line.mid(commentPos);
            line = before + "<span style=\"color: #64748B; font-style: italic;\">" + comment + "</span>";
        } else {
            // Highlight C++ Keywords
            static const QStringList keywords = {
                "for", "while", "if", "else", "return", "throw", "new", "delete",
                "const", "int", "bool", "void", "true", "false", "auto", "nullptr", "sizeof"
            };
            for (const auto& kw : keywords) {
                QRegularExpression re(QString(R"(\b%1\b)").arg(kw));
                line.replace(re, QString("<span style=\"color: #C084FC; font-weight: bold;\">%1</span>").arg(kw));
            }

            // Highlight Types
            static const QStringList types = {
                "Node", "DynamicArray", "LinkedList", "Stack", "Queue", "BST", "MinHeap",
                "HuffmanNode", "HuffmanPriorityQueue", "BitWriter", "BitReader", "Graph",
                "EdgeNode", "DisjointSet", "DijkstraHeap", "std::size_t", "uint64_t", "uint8_t"
            };
            for (const auto& ty : types) {
                QRegularExpression re(QString(R"(\b%1\b)").arg(ty));
                line.replace(re, QString("<span style=\"color: #38BDF8; font-weight: 600;\">%1</span>").arg(ty));
            }

            // Highlight String Literals
            QRegularExpression strRe(R"(&quot;(.*?)&quot;)");
            line.replace(strRe, "<span style=\"color: #34D399;\">&quot;\\1&quot;</span>");

            // Highlight Numbers
            QRegularExpression numRe(R"(\b(\d+)\b)");
            line.replace(numRe, "<span style=\"color: #F59E0B;\">\\1</span>");
        }

        // Line number formatting: 01, 02...
        QString lineNum = QString("%1").arg(i + 1, 2, 10, QChar('0'));
        html += QString("<div style=\"margin: 2px 0;\"><span style=\"color: #475569; user-select: none;\">%1  </span>%2</div>")
                .arg(lineNum, line);
    }

    return html;
}

void CodeSnippetPanel::setSnippet(const QString& filename, const QString& functionSignature, const QString& codeLines) {
    fileBadge->setText("📁 " + filename);
    functionBadge->setText("⚡ " + functionSignature);
    currentRawCode = codeLines;

    QString html = formatHighlightedCode(codeLines);
    codeEditor->setHtml(html);
}

} // namespace algolib
