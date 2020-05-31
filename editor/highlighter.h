#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTimer>

#include "grammar.h"
#include "theme.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

struct SpanInfo {
    int start;
    int length;
    int red;
    int green;
    int blue;
};

class HighlightBlockData : public QTextBlockUserData {
public:
    parse::stack_ptr parser_state;
    scope::scope_t last_scope;
    bool dirty;
    size_t last_prev_block_rule;

    std::vector<SpanInfo> spans;
};

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(QTextDocument* parent = 0);

    void setTheme(theme_ptr theme);
    void setGrammar(parse::grammar_ptr grammar);
    void setDeferRendering(bool defer);

    bool isDirty() { return hasDirtyBlocks; }

protected:
    void highlightBlock(const QString& text) override;
    void setFormatFromStyle(size_t start, size_t length, style_t& style, const char* line, HighlightBlockData* blockData);

private:
    bool deferRendering;
    parse::grammar_ptr grammar;
    theme_ptr theme;

    QColor foregroundColor;

    bool hasDirtyBlocks;
    QTextBlock updateIterator;
    QTimer updateTimer;

private Q_SLOTS:
    void onUpdate();
};

#endif // HIGHLIGHTER_H