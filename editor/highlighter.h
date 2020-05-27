#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#include "grammar.h"
#include "theme.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class HighlightBlockData : public QTextBlockUserData {
public:
    parse::stack_ptr parser_state;
    scope::scope_t last_scope;
};

//! [0]
class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(QTextDocument* parent = 0);

    void setTheme(theme_ptr theme);
    void setDeferRendering(bool defer);

protected:
    void highlightBlock(const QString& text) override;

private:
    bool deferRendering;
    parse::grammar_ptr grammar;
    theme_ptr theme;
};
//! [0]

#endif // HIGHLIGHTER_H