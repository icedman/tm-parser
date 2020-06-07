#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QTextCursor>
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
class Editor;

struct editor_settings_t {
    bool mini_map;
    bool gutter;
    float font_size;
    std::string font;
    int tab_size;
    bool tab_to_spaces;
    bool word_wrap;
};

typedef std::shared_ptr<editor_settings_t> editor_settings_ptr;

class Overlay : public QWidget {
    Q_OBJECT
public:
    Overlay(QWidget* parent = nullptr);
    
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

    QPixmap buffer;

    QTimer updateTimer;
    bool cursorOn;

private Q_SLOTS:
    void updateCursor();
};

class TextmateEdit : public QPlainTextEdit {
    Q_OBJECT
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

public:
    void addExtraCursor(QTextCursor cursor = QTextCursor());
    void removeExtraCursors();
    void updateExtraCursors(QKeyEvent *e);
    QList<QTextCursor> extraCursors;

    void paintToBuffer();

private:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* e) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    Overlay* overlay;
    Editor* editor;
};

class Editor : public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);

    void setupEditor();
    void setTheme(theme_ptr _theme);
    void setLanguage(language_info_ptr _lang);

    bool openFile(const QString& path = QString());
    bool saveFile(const QString& path = QString());
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

    QColor backgroundColor;
    QColor selectionBgColor;

    editor_settings_ptr settings;

    theme_ptr theme;
    language_info_ptr lang;
    parse::grammar_ptr grammar;

    JSEditor* jsobj;

private:
    QTimer updateTimer;
    QScrollBar* vscroll;
    QTextBlock updateIterator;

public slots:
    void updateScrollBar();
    void updateScrollBar(int i);
    void updateGutter(bool force = false);
    void updateMiniMap(bool force = false);

private Q_SLOTS:
    void updateRequested(const QRect& rect, int d);
    void highlightBlocks();
};

#endif // EDITOR_WINDOW_H