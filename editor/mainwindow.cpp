#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"
#include "reader.h"
#include "theme.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , updateTimer(this)
{
    configure();

    setupTheme();
    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    updateTimer.singleShot(2000, this, SLOT(warmConfigure()));

    setWindowTitle(tr("Editor"));
    setMinimumSize(600, 400);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
        tr("<p>The <b>Syntax Highlighter</b> example shows how "
           "to perform simple syntax highlighting by subclassing "
           "the QSyntaxHighlighter class and describing "
           "highlighting rules using regular expressions.</p>"));
}

void MainWindow::configure()
{
    load_extensions(QString("../extensions"), extensions);
}

void MainWindow::setupTheme()
{
    // Json::Value json_theme = parse::loadJson("./dark_vs.json");
    // Json::Value json_theme = parse::loadJson("./light_vs.json");
    // Json::Value json_theme = parse::loadJson("./dracula-soft.json");
    Json::Value json_theme = parse::loadJson("./monokai-color-theme.json");
    // Json::Value json_theme = parse::loadJson("./shades-of-purple-color-theme-italic.json");
    // Json::Value json_theme = parse::loadJson("./shades-of-purple-color-theme.json");

    // std::cout << json_theme << std::endl;
    
    theme = parse_theme(json_theme);

    // QColor menuBarBg;
    // QColor menuBarFg;
    // if (theme_color(theme, "activityBar.background", menuBarBg)) {
    //     theme_color(theme, "activityBar.foreground", menuBarFg);
    //     menuBar()->setStyleSheet("QMenuBar{ color: " + menuBarFg.name() + "; background: " + menuBarBg.name() + " }");
    // }
}

void MainWindow::newFile()
{
    editor->newFile();
}

void MainWindow::saveFile()
{
    QString fileName = editor->fileName;

    // std::cout << fileName.toUtf8().constData() << std::endl;

    if (fileName.isNull() || fileName.isEmpty())
        fileName = QFileDialog::getOpenFileName(this, tr("Save File"), "", "");

    if (!fileName.isEmpty()) {
        editor->saveFile(fileName);
    }
}

void MainWindow::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (!fileName.isEmpty()) {
        editor->setGrammar(grammar_from_file(fileName, extensions));
        editor->openFile(fileName);
    }
}

void MainWindow::setupEditor()
{
    editor = new Editor();

    editor->setTheme(theme);
    editor->setupEditor();

    setCentralWidget(editor);
}

void MainWindow::setupFileMenu()
{
    fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this,
        &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(
        tr("&Open..."),
        this, [this]() { openFile(); }, QKeySequence::Open);
    fileMenu->addAction(
        tr("&Save..."),
        this, [this]() { saveFile(); }, QKeySequence::Save);
    fileMenu->addAction(tr("E&xit"), qApp,
        &QApplication::quit, QKeySequence::Quit);

    viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::warmConfigure()
{
    std::cout << "configure" << std::endl;
}

/*
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}
void MainWindow::readSettings()
{
    QSettings settings("MyCompany", "MyApp");
    restoreGeometry(settings.value("myWidget/geometry").toByteArray());
    restoreState(settings.value("myWidget/windowState").toByteArray());
}
*/