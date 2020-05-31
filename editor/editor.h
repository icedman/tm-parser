#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include "highlighter.h"

#include "grammar.h"
#include "theme.h"

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QTimer>
#include <QWidget>

class MiniMap;
class Gutter;

class TextmateEdit : public QPlainTextEdit {
public:
    QTextBlock _firstVisibleBlock()
    {
        return firstVisibleBlock();
    }

    QRectF _blockBoundingGeometry(QTextBlock& block)
    {
        return blockBoundingGeometry(block);
    }

    QPointF _contentOffset()
    {
        return contentOffset();
    }
};

struct editor_settings_t {
    bool miniMap : true;
};

class Editor : public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);

    void setupEditor();
    void setTheme(theme_ptr _theme);
    void setGrammar(parse::grammar_ptr _grammar);

    void openFile(const QString& path = QString());
    void saveFile(const QString& path = QString());
    void newFile();

    bool isAvailable() {
        return highlighter->isReady() && !updateTimer.isActive();
    }

    QString fileName;
    TextmateEdit* editor;
    Gutter* gutter;
    MiniMap* mini;
    Highlighter* highlighter;

    struct editor_settings_t *settings;

private:
    QTimer updateTimer;

    QScrollBar* vscroll;

    theme_ptr theme;
    parse::grammar_ptr grammar;

    QTextBlock updateIterator;

public slots:
    void updateScrollBar();
    void updateScrollBar(int i);
    void updateGutter();
    void updateMiniMap();

private Q_SLOTS:
    void updateRequested(const QRect& rect, int d);

    void highlightBlocks();
};

#endif // EDITOR_WINDOW_H