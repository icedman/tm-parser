#include <QtWidgets>
#include <iostream>

#include "editor.h"
#include "gutter.h"
#include "mainwindow.h"
#include "minimap.h"
#include "reader.h"
#include "settings.h"
#include "commands.h"

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
    jsobj = new JSEditor((QObject*)this);
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
        fileName = path;
    }
}

void Editor::openFile(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        fileName = path;
        highlighter->setLanguage(lang);

        if (file.size() > (1024 * 256)) {
            // todo do super load!
            std::cout << "do threaded syntax highlighting at load" << std::endl;

            std::cout << file.size() << std::endl;
            highlighter->setDeferRendering(true);
            editor->setPlainText(file.readAll());
            highlightBlocks();
        } else {
            highlighter->setDeferRendering(false);
            editor->setPlainText(file.readAll());
        }
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

    theme_scrollbar(theme, "editor.background", *vscroll);
    theme_scrollbar(theme, "editor.background", *editor->horizontalScrollBar());

    editor->setStyleSheet("QPlainTextEdit { border: 0px; } QScrollBar:vertical { width: 0px }");
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);
}

void Editor::setLanguage(language_info_ptr _lang)
{
    lang = _lang;
    grammar = _lang->grammar;
    if (highlighter) {
        highlighter->setLanguage(lang);
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
    font.setFamily(settings->font.c_str());
    font.setPointSize(settings->font_size);
    font.setFixedPitch(true);

    editor = new TextmateEdit();
    editor->setFont(font);
    editor->setTabStopDistance(QFontMetrics(font).horizontalAdvance('w') * settings->tab_size);

    connect(editor, SIGNAL(blockCountChanged(int)), this, SLOT(updateGutter()));
    connect(editor, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateRequested(QRect, int)));

    gutter = new Gutter();
    gutter->font = font;
    gutter->editor = this;

    mini = new MiniMap();
    mini->editor = this;

    vscroll = new QScrollBar();

    connect(editor->verticalScrollBar(), SIGNAL(valueChanged(int)), vscroll, SLOT(setValue(int)));
    connect(vscroll, SIGNAL(valueChanged(int)), editor->verticalScrollBar(), SLOT(setValue(int)));
    connect(vscroll, SIGNAL(valueChanged(int)), this, SLOT(updateScrollBar(int)));

    QHBoxLayout* box = new QHBoxLayout(this);

    box->setMargin(0);
    box->setSpacing(0);

    box->addWidget(gutter);
    box->addWidget(editor);
    box->addWidget(mini);
    box->addWidget(vscroll);

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
    static QRect previousRect;

    if (previousRect == rect) {
        // probably just the cursor pulse
        return;
    }

    updateGutter();
    updateMiniMap();

    previousRect = rect;
}

void Editor::updateMiniMap()
{
    if (!mini) {
        return;
    }

    // todo
    if (settings->mini_map) {
        mini->show();
    } else {
        mini->hide();
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
    QScrollBar* editorScroll = editor->verticalScrollBar();
    size_t max = editorScroll->maximum();
    if (max > 0) {
        vscroll->show();
    } else {
        vscroll->hide();
    }
    vscroll->setMaximum(max);

    vscroll->setSingleStep(editorScroll->singleStep());
    vscroll->setPageStep(editorScroll->pageStep());

    updateMiniMap();
}

void Editor::updateScrollBar(int i)
{
    updateGutter();
    updateScrollBar();
}

static bool isFoldable(QTextBlock& block)
{
    HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
    if (blockData) {
        if (blockData->brackets.size()) {
            auto l = blockData->brackets.back();
            return l.open;
        }
    }
    return false;
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
                gutter->lineNumbers[index].foldable = isFoldable(block);
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

static QTextBlock findBracketMatch(QTextBlock& block)
{
    if (!block.isValid()) {
        return QTextBlock();
    }

    HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
    std::vector<bracket_info_t> brackets = blockData->brackets;

    if (!brackets.size()) {
        return QTextBlock();
    }

    QTextBlock res = block.next();
    while (res.isValid()) {
        HighlightBlockData* resData = reinterpret_cast<HighlightBlockData*>(res.userData());
        if (!resData) {
            continue;
        }
        std::vector<bracket_info_t> resBrackets = resData->brackets;
        for (auto b : resBrackets) {
            if (!b.open) {
                auto l = brackets.back();
                if (l.open && l.bracket == b.bracket) {
                    brackets.pop_back();
                } else {
                    return QTextBlock();
                }

                if (!brackets.size()) {
                    // std::cout << "found end!" << std::endl;
                    return res;
                }
                continue;
            }
            brackets.push_back(b);
        }
        res = res.next();
    }

    return res;
}

void Editor::toggleFold(size_t line)
{
    QTextDocument* doc = editor->document();
    QTextBlock folder = doc->findBlockByNumber(line - 1);
    QTextBlock end = findBracketMatch(folder);
    QTextBlock block = doc->findBlockByNumber(line);

    if (!end.isValid() || !folder.isValid() || !block.isValid()) {
        return;
    }

    HighlightBlockData* folderBlockData = reinterpret_cast<HighlightBlockData*>(folder.userData());
    HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());

    if (folderBlockData && blockData) {
        folderBlockData->folded = !folderBlockData->folded;

        while (block.isValid()) {
            blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
            blockData->folded = folderBlockData->folded;
            if (folderBlockData->folded) {
                block.setVisible(false);
                block.setLineCount(0);
            } else {
                block.setVisible(true);
                block.setLineCount(1);
            }

            if (block == end) {
                break;
            }
            block = block.next();
        }
    }

    editor->update();
    updateGutter();
}

void Overlay::paintEvent(QPaintEvent*)
{
    QWidget* container = (QWidget*)parent();
    resize(container->width(), container->height());

    QPainter p(this);
    TextmateEdit* editor = (TextmateEdit*)parent();
    Editor* e = (Editor*)editor->parent();

    QColor foldedBg;
    theme_color(e->theme, "editor.selectionBackground", foldedBg);
    foldedBg.setAlpha(64);

    // draw extras here
    QTextBlock block = editor->_firstVisibleBlock();
    while (block.isValid()) {
        if (block.isVisible()) {
            QRectF rect = editor->_blockBoundingGeometry(block).translated(editor->_contentOffset());
            // folded indicator
            HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
            if (blockData && blockData->folded) {
                p.fillRect(rect, foldedBg);
            }

            if (rect.top() > height())
                break;
        }
        block = block.next();
    }
}

void Overlay::mousePressEvent(QMouseEvent* event)
{
    // std::cout << "still got ot me" << std::endl;
    // listening to click events
}

TextmateEdit::TextmateEdit(QWidget* parent)
    : QPlainTextEdit(parent)
{
    overlay = new Overlay(this);
}

void TextmateEdit::paintEvent(QPaintEvent* e)
{
    QPlainTextEdit::paintEvent(e);
}

void TextmateEdit::mousePressEvent(QMouseEvent* e)
{
    QPlainTextEdit::mousePressEvent(e);
    overlay->mousePressEvent(e);
    overlay->update();
}

void TextmateEdit::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() != Qt::NoModifier) {
        QString keys = QKeySequence(e->modifiers() | e->key()).toString();
        MainWindow::instance()->processKeys(keys.toLower());
    }

    // multiple cursors like
    // QTextCursor cursor(document());
    // cursor.beginEditBlock();
    // cursor.insertText("Hello");
    // cursor.insertText("World");
    // cursor.endEditBlock();

    // trap key bindings here
    if (e->key() == Qt::Key_Tab && e->modifiers() == Qt::NoModifier) {
        Editor* e = (Editor*)parent();
        if (e->settings->tab_to_spaces) {
            Commands::insertTab(e, e->editor->textCursor());
            return;
        }
    }

    QPlainTextEdit::keyPressEvent(e);
    overlay->update();
}