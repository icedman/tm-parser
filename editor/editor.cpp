#include <QtWidgets>
#include <iostream>

#include "gutter.h"
#include "minimap.h"
#include "editor.h"
#include "reader.h"

// QTextBlock SublimeTextEdit::_firstVisibleBlock()
// {
//     return firstVisibleBlock();
// }

Editor::Editor(QWidget* parent)
    : QWidget(parent),
    updateTimer(this)
{
    setupEditor();

    // connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursor()));
    connect(editor, SIGNAL(blockCountChanged(int)), this, SLOT(updateGutter()));
    connect(editor, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateRequested(QRect, int)));
}

void Editor::newFile()
{
    editor->clear();
}

void Editor::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            highlighter->setDeferRendering(true);
            highlighter->setGrammar(grammar);
            editor->setPlainText(file.readAll());
            highlightBlocks();
        }
    }
}

bool theme_color(theme_ptr theme, std::string name, QColor &qcolor) {
    color_info_t clr;
    theme->theme_color(name, clr);
    qcolor.setRed(clr.red * 255);
    qcolor.setGreen(clr.green * 255);
    qcolor.setBlue(clr.blue * 255);
    return !clr.is_blank();
}

void Editor::setupEditor()
{
    QFont font;
    //'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
    font.setFamily("Droid Sans Mono");
    // font.setFamily("Source Code Pro");

    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new SublimeTextEdit();
    editor->setFont(font);

    gutter = new Gutter();
    gutter->font = font;

    mini = new MiniMap();
    mini->editor = editor;

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(gutter);
    box->addWidget(editor);
    box->addWidget(mini);
    box->setMargin(0);
    box->setSpacing(0);

    setLayout(box);

    // Json::Value json_theme = parse::loadJson("./dark_vs.json");
    // Json::Value json_theme = parse::loadJson("./light_vs.json");
    Json::Value json_theme = parse::loadJson("./dracula-soft.json");
    // Json::Value json_theme = parse::loadJson("./monokai-color-theme.json");
    // Json::Value json_theme = parse::loadJson("./shades-of-purple-color-theme.json");
    theme = parse_theme(json_theme);

    Json::Value json_grammar = parse::loadJson("../extensions/cpp/syntaxes/c.tmLanguage.json");
    grammar = parse::parse_grammar(json_grammar);

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
    
    editor->setStyleSheet("QPlainTextEdit { border: 0px; } QScrollBar { width: 0px } ");

    //------------------
    // setup highlighter
    //------------------
    highlighter = new Highlighter(editor->document());
    highlighter->setTheme(theme);

    updateMiniMap();
}

void Editor::highlightBlocks()
{
    QTextDocument *doc = editor->document();
    int rendered = 0;
    QTextBlock block = doc->begin();
    while(block.isValid() && rendered<100) {
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
        if (!blockData) {
            rendered++;
            blockData = new HighlightBlockData;
            block.setUserData(blockData);
            highlighter->rehighlightBlock(block);
        }
        block = block.next();
    }

    if (rendered > 0) {
        updateTimer.singleShot(50, this, SLOT(highlightBlocks()));
    } else {
        std::cout << "all rendering done" << std::endl;
        highlighter->setDeferRendering(false);
    }
}

void Editor::updateRequested(const QRect &rect, int d)
{
    updateGutter();
}

void Editor::updateMiniMap()
{   
    int sw = 100;
    mini->setMinimumSize(sw, 0);
    mini->update();
}

void Editor::updateGutter()
{
    int sw = 0;
    int digits = 2 + 2;
    int maxLines = editor->blockCount();
    for (int number = 10; number < maxLines; number *= 10)
        ++digits;
    sw += editor->fontMetrics().width('w') * digits;
    
    gutter->setMinimumSize(sw, 0);
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
}