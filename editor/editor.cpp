#include <QtWidgets>
#include <iostream>

#include "editor.h"
#include "gutter.h"
#include "minimap.h"
#include "reader.h"
#include "extension.h"
#include "settings.h"

Editor::Editor(QWidget* parent)
    : QWidget(parent)
    , theme(0)
    , grammar(0)
    , gutter(0)
    , mini(0)
    , highlighter(0)
    , editor(0)
    , updateTimer(this)
{
}

void Editor::newFile()
{
    editor->clear();
    fileName = "";
}

void Editor::saveFile(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << editor->toPlainText();
    }
}

void Editor::openFile(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        fileName = path;
        highlighter->setGrammar(grammar);
        
        if (file.size() > (1024*500)) {
            // do super load!
            std::cout << "do threaded syntax highlighting at load" << std::endl;
        }

        std::cout << file.size() << std::endl;
        highlighter->setDeferRendering(true);
        editor->setPlainText(file.readAll());
        highlightBlocks();
    }
}

void Editor::setTheme(theme_ptr _theme)
{
    theme = _theme;

    if (!editor) {
        return;
    }

    if (highlighter) {
        highlighter->setTheme(theme);
    }

    //------------------
    // editor theme
    //------------------
    QColor bgColor;
    QColor fgColor;
    QColor selectionBgColor;
    QColor lineNumberColor;

    if (theme_color(theme, "editor.foreground", fgColor)) {
        QTextCharFormat fmt;
        fmt.setForeground(QBrush(fgColor));
        editor->mergeCurrentCharFormat(fmt);
        gutter->lineNumberColor = fgColor;
    }

    if (theme_color(theme, "editorLineNumber.foreground", lineNumberColor)) {
        gutter->lineNumberColor = lineNumberColor;
    }

    if (theme_color(theme, "editor.background", bgColor)) {
        QPalette p = editor->palette();

        p.setColor(QPalette::Active, QPalette::Base, bgColor);
        p.setColor(QPalette::Inactive, QPalette::Base, bgColor);
        p.setColor(QPalette::HighlightedText, fgColor);

        editor->setPalette(p);

        gutter->backgroundColor = bgColor;
        mini->backgroundColor = bgColor;
    }

    if (theme_color(theme, "editor.selectionBackground", selectionBgColor)) {
        QPalette p = editor->palette();
        p.setColor(QPalette::Highlight, selectionBgColor);
        editor->setPalette(p);
    }

    vscroll->setStyleSheet(" \
    QScrollBar:vertical { \
      width: 12px; \
      background: " + bgColor.darker(105).name() + "; \
      padding: 3px; \
      border: none; \
    } \
    QScrollBar::handle:vertical { \
      background: " + bgColor.lighter(150).name() + "; \
      border-radius: 3px; \
    } \
    QScrollBar::add-line:vertical { \
      border: none; \
      background: none; \
    } \
    QScrollBar::sub-line:vertical { \
      border: none; \
      background: none; \
    } \
    QScrollBar::handle:hover:vertical { \
      background: " + bgColor.lighter(180).name() + "; \
    } \
    ");

    editor->setStyleSheet("QPlainTextEdit { border: 0px; } QScrollBar { width: 0px }");
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);
}

void Editor::setGrammar(parse::grammar_ptr _grammar)
{
    grammar = _grammar;
    if (highlighter) {
        highlighter->setGrammar(grammar);
    }
}

void Editor::setupEditor()
{
    if (editor != 0) {
        // require a theme
        // no reinit if already done
        return;
    }

    QFont font;
    //'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
    // font.setFamily("Droid Sans Mono");
    font.setFamily("Source Code Pro for Powerline");

    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new SublimeTextEdit();
    editor->setFont(font);

    connect(editor, SIGNAL(blockCountChanged(int)), this, SLOT(updateGutter()));
    connect(editor, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateRequested(QRect, int)));

    gutter = new Gutter();
    gutter->font = font;

    mini = new MiniMap();
    mini->editor = editor;

    vscroll = new QScrollBar();

    connect(editor->verticalScrollBar(), SIGNAL(valueChanged(int)), vscroll, SLOT(setValue(int)));
    connect(vscroll, SIGNAL(valueChanged(int)), editor->verticalScrollBar(), SLOT(setValue(int)));
    connect(vscroll, SIGNAL(valueChanged(int)), this, SLOT(updateScrollBar(int)));

    QHBoxLayout* box = new QHBoxLayout();
    box->addWidget(gutter);
    box->addWidget(editor);
    box->addWidget(mini);
    box->addWidget(vscroll);

    box->setMargin(0);
    box->setSpacing(0);

    setLayout(box);

    // gutter->hide();
    // mini->hide();

    if (theme) {
        setTheme(theme);
    }

    //------------------
    // setup highlighter
    //------------------
    highlighter = new Highlighter(editor->document());
    highlighter->setTheme(theme);

    updateMiniMap();
}

void Editor::updateRequested(const QRect& rect, int d)
{
    updateGutter();
    updateMiniMap();
}

void Editor::updateMiniMap()
{
    if (!mini) {
        return;
    }

    int sw = 60 + (width() * 0.03);
    mini->setMinimumSize(sw, 0);
    mini->update();

    int first = 0;
    if (gutter->lineNumbers.size()) {
        first = gutter->lineNumbers[0].number;
    }
    mini->setSizes(first, gutter->lineNumbers.size(), vscroll->value(), vscroll->maximum());
}

void Editor::updateScrollBar()
{
    size_t max = editor->verticalScrollBar()->maximum();
    if (max > 0) {
        vscroll->show();
    } else {
        vscroll->hide();
    }
    vscroll->setMaximum(max);
    updateMiniMap();
}

void Editor::updateScrollBar(int i)
{
    updateGutter();
    updateScrollBar();
}

void Editor::updateGutter()
{
    if (!gutter) {
        return;
    }

    int sw = 0;
    int digits = 2 + 2;
    int maxLines = editor->blockCount();
    for (int number = 10; number < maxLines; number *= 10)
        ++digits;
    sw += editor->fontMetrics().width('w') * digits;

    gutter->setMinimumSize(sw, height());
    QRectF sidebarRect(0, 0, sw, height());

    QTextBlock block = editor->_firstVisibleBlock();
    int index = 0;
    while (block.isValid()) {
        if (block.isVisible()) {
            QRectF rect = editor->_blockBoundingGeometry(block).translated(editor->_contentOffset());
            if (sidebarRect.intersects(rect)) {
                if (gutter->lineNumbers.count() >= index)
                    gutter->lineNumbers.resize(index + 1);
                gutter->lineNumbers[index].position = rect.top();
                gutter->lineNumbers[index].number = block.blockNumber() + 1;
                // gutter->lineNumbers[index].foldable = d->codeFolding ? isFoldable(block.blockNumber() + 1) : false;
                // gutter->lineNumbers[index].folded = d->codeFolding ? isFolded(block.blockNumber() + 1) : false;
                ++index;
            }
            if (rect.top() > sidebarRect.bottom())
                break;
        }
        block = block.next();
    }
    gutter->lineNumbers.resize(index);
    gutter->update();

    updateScrollBar();
}


void Editor::highlightBlocks()
{
    int rendered = 0;

    if (!updateIterator.isValid()) {
        QTextDocument* doc = editor->document();
        updateIterator = doc->begin();
    }
    
    while (updateIterator.isValid() && rendered < 100) {
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(updateIterator.userData());
        if (!blockData) {
            rendered++;
            blockData = new HighlightBlockData;
            updateIterator.setUserData(blockData);
            highlighter->rehighlightBlock(updateIterator);
        }
        updateIterator = updateIterator.next();
    }

    if (rendered > 0) {
        updateTimer.singleShot(50, this, SLOT(highlightBlocks()));
    } else {
        std::cout << "all rendering done" << std::endl;
        highlighter->setDeferRendering(false);
    }
}