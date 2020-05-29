#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include "highlighter.h"

#include "grammar.h"
#include "theme.h"

#include <QWidget>
#include <QTimer>
#include <QPlainTextEdit>   
#include <QTextBlock>

class MiniMap;
class Gutter;

class SublimeTextEdit : public QPlainTextEdit
{
public:
    QTextBlock _firstVisibleBlock() {
        return firstVisibleBlock();
    }
    
    QRectF _blockBoundingGeometry(QTextBlock &block) {
        return blockBoundingGeometry(block); 
    }

    QPointF _contentOffset() {
        return contentOffset();
    }

};

class Editor : public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);

    void setupEditor();

    void openFile(const QString& path = QString());
    void newFile();

private:

    SublimeTextEdit* editor;
    Gutter* gutter;
    MiniMap* mini;
    Highlighter* highlighter;
    QTimer updateTimer;

    theme_ptr theme;
    parse::grammar_ptr grammar;

public slots:
    void updateGutter();
    void updateMiniMap();

private Q_SLOTS:
    void updateRequested(const QRect &rect, int d);

    void highlightBlocks();
};

#endif // EDITOR_WINDOW_H