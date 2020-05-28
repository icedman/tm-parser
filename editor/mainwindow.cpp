#include <QtWidgets>

#include <iostream>

#include "mainwindow.h"
#include "reader.h"

//! [0]
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    updateTimer(this)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    setCentralWidget(editor);
    setWindowTitle(tr("Syntax Highlighter"));
    setMinimumSize(600, 400);
}
//! [0]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
        tr("<p>The <b>Syntax Highlighter</b> example shows how "
           "to perform simple syntax highlighting by subclassing "
           "the QSyntaxHighlighter class and describing "
           "highlighting rules using regular expressions.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            highlighter->setDeferRendering(true);
            editor->setPlainText(file.readAll());
            highlightBlocks();
        }
    }
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    //'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
    font.setFamily("Droid Sans Mono");
    // font.setFamily("Source Code Pro");

    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new QSublimeTextEdit;
    editor->setFont(font);

    // Json::Value json_theme = parse::loadJson("./light_vs.json");
    Json::Value json_theme = parse::loadJson("./dracula-soft.json");
    theme = parse_theme(json_theme);

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
//! [1]

void MainWindow::setupFileMenu()
{
    QMenu* fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this,
        &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(
        tr("&Open..."),
        this, [this]() { openFile(); }, QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), qApp,
        &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu* helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::highlightBlocks()
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