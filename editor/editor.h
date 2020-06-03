#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QTimer>
#include <QWidget>

#include "extension.h"
#include "grammar.h"
#include "highlighter.h"
#include "js.h"
#include "theme.h"

class MiniMap;
class Gutter;
class TextmateEdit;

class Overlay : public QWidget {
public:
    explicit Overlay(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
};

class TextmateEdit : public QPlainTextEdit {
public:
    TextmateEdit(QWidget* parent = nullptr);

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

private:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* e) override;

    Overlay* overlay;
};

class Editor : public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);

    void setupEditor();
    void setTheme(theme_ptr _theme);
    void setLanguage(language_info_ptr _lang);

    void openFile(const QString& path = QString());
    void saveFile(const QString& path = QString());
    void newFile();

    bool isAvailable()
    {
        return highlighter->isReady() && !updateTimer.isActive();
    }

    void toggleFold(size_t line);

    QString fileName;
    TextmateEdit* editor;
    Gutter* gutter;
    MiniMap* mini;
    Highlighter* highlighter;

    struct editor_settings_t* settings;

    theme_ptr theme;
    language_info_ptr lang;
    parse::grammar_ptr grammar;

    JSEditor jsobj;

private:
    QTimer updateTimer;
    QScrollBar* vscroll;
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