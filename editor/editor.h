#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include "highlighter.h"
#include "gutter.h"

#include "grammar.h"
#include "theme.h"

#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QPlainTextEdit;   //<< one-block = one-line
typedef QPlainTextEdit QSublimeTextEdit;
QT_END_NAMESPACE

//! [0]
class Editor : public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);

    void setupEditor();

    void openFile(const QString& path = QString());
    void newFile();

private:

    QSublimeTextEdit* editor;
    Highlighter* highlighter;
    QTimer updateTimer;

    theme_ptr theme;
    parse::grammar_ptr grammar;

private Q_SLOTS:
    void highlightBlocks();
};

#endif // EDITOR_WINDOW_H