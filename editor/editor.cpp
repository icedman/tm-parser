#include <QtWidgets>
#include <iostream>

#include "editor.h"
#include "reader.h"

Editor::Editor(QWidget* parent)
    : QWidget(parent),
    updateTimer(this)
{
    setupEditor();
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

void Editor::setupEditor()
{
    QFont font;
    //'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
    font.setFamily("Droid Sans Mono");
    // font.setFamily("Source Code Pro");

    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new QSublimeTextEdit();
    editor->setFont(font);

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(editor);
    box->setMargin(0);

    setLayout(box);

    // Json::Value json_theme = parse::loadJson("./light_vs.json");
    Json::Value json_theme = parse::loadJson("./dracula-soft.json");
    theme = parse_theme(json_theme);

    Json::Value json_grammar = parse::loadJson("../extensions/cpp/syntaxes/c.tmLanguage.json");
    grammar = parse::parse_grammar(json_grammar);

    //------------------
    // editor theme
    //------------------
    if (!theme->global_style.background.is_blank()) {
        QPalette p = editor->palette();
        p.setColor(QPalette::Active, QPalette::Base,
            QColor(theme->global_style.background.red * 255,
                theme->global_style.background.green * 255,
                theme->global_style.background.blue * 255, 255));
        p.setColor(QPalette::Inactive, QPalette::Base,
            QColor(theme->global_style.background.red * 255,
                theme->global_style.background.green * 255,
                theme->global_style.background.blue * 255, 255));
        editor->setPalette(p);
    }

    if (!theme->global_style.foreground.is_blank()) {
        QTextCharFormat fmt;
        fmt.setForeground(QBrush(QColor(theme->global_style.foreground.red * 255,
            theme->global_style.foreground.green * 255,
            theme->global_style.foreground.blue * 255, 255)));
        editor->mergeCurrentCharFormat(fmt);
    }

    // setup highlighter
    highlighter = new Highlighter(editor->document());
    highlighter->setTheme(theme);

    // openFile("../tests/cases/sqlite3.c");
    // openFile("../tests/cases/tinywl.c");
    openFile("../tests/cases/test.c");
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