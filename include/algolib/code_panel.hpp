#ifndef ALGOLIB_CODE_PANEL_HPP
#define ALGOLIB_CODE_PANEL_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace algolib {

class CodeSnippetPanel : public QWidget {
private:
    QLabel* fileBadge;
    QLabel* functionBadge;
    QPushButton* copyBtn;
    QTextEdit* codeEditor;
    QString currentRawCode;

    QString formatHighlightedCode(const QString& rawCode);

public:
    explicit CodeSnippetPanel(QWidget* parent = nullptr);

    void setSnippet(const QString& filename, const QString& functionSignature, const QString& codeLines);
    void showEmptyState();
};

} // namespace algolib

#endif // ALGOLIB_CODE_PANEL_HPP
