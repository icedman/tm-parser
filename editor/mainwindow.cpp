#include <QStackedWidget>
#include <QStandardPaths>
#include <QtWidgets>

#include <iostream>

#include "mainwindow.h"
#include "reader.h"
#include "settings.h"
#include "theme.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , updateTimer(this)
{
    configure();

    setupEditor();
    setupLayout();
    setupMenu();  

    applySettings();
    applyTheme();

    setWindowTitle(tr("Editor"));
    setMinimumSize(600, 400);

    updateTimer.singleShot(250, this, SLOT(warmConfigure()));
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
    QString userSettings = QStandardPaths::locate(QStandardPaths::HomeLocation, ".editor", QStandardPaths::LocateDirectory);
    load_settings(userSettings, settings);

    QString userExtensions = QStandardPaths::locate(QStandardPaths::HomeLocation, ".editor/extensions", QStandardPaths::LocateDirectory);
    load_extensions(userExtensions, extensions);
    load_extensions(QString("./extensions"), extensions);

    
    if (settings["theme"].isString()) {
        theme = theme_from_name(settings["theme"].asString().c_str(), extensions);
    } else {
        // theme = theme_from_name("Monokai", extensions);
        // theme = theme_from_name("Monokai Dimmed", extensions);
        // theme = theme_from_name("Solarized Dark", extensions);
        // theme = theme_from_name("Tomorrow Night Blue", extensions);
        theme = theme_from_name("Dracula Soft", extensions);
        // theme = theme_from_name("Dracula", extensions);
        // theme = theme_from_name("Shades of Purple", extensions);
    }
}

void MainWindow::applyTheme()
{
    QColor menuBarBg;
    QColor menuBarFg;
    if (theme_color(theme, "menu.background", menuBarBg)) {
        theme_color(theme, "menu.foreground", menuBarFg);
        menuBar()->setStyleSheet("QMenuBar{ color: " + menuBarFg.name() + "; background: " + menuBarBg.name() + " }");
    }

    theme_splitter(theme, "editor.background", *central);
    theme_sidebar(theme, "editor.background", *sidebar);
    theme_scrollbar(theme, "editor.background", *sidebar->horizontalScrollBar());
    theme_scrollbar(theme, "editor.background", *sidebar->verticalScrollBar());
    if (!theme_sidebar(theme, "sideBar", *sidebar)) {
        if (!theme_sidebar(theme, "menu", *sidebar)) {
            theme_sidebar(theme, "editor", *sidebar);
        }
    }
    if (!theme_statusbar(theme, "statusBar", *statusBar())) {
        theme_sidebar(theme, "editor", *statusBar());
    }

    statusBar()->showMessage("hello", 1000);
}

void MainWindow::applySettings()
{
    std::cout << settings["sidebar"] << std::endl;

    if (settings["sidebar"] == true) {
        sidebar->show();
    } else {
        sidebar->hide();
    }

    if (settings["statusbar"] == true) {
        statusBar()->show();
    } else {
        statusBar()->hide();
    }

    if (settings["editor"].isObject()) {
        editor_settings.miniMap = settings["editor"]["minimap"] == true;
    }
}

void MainWindow::setupLayout()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    editors = new QStackedWidget();

    // hbox->setMargin(0);
    // hbox->setSpacing(0);
    // editors->setMargin(0);
    // editors->setSpacing(0);

    setStyleSheet("* { font-size: 12pt; } QPlainTextEdit { font-size: 12pt; }");

    sidebar = new Sidebar();
    sidebar->mainWindow = this;

    splitter->addWidget(sidebar);
    splitter->addWidget(editors);
    splitter->setStretchFactor(1, 4);

    editors->addWidget(editor);
    setCentralWidget(splitter);

    central = splitter;
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
    if (!editor || !editor->isAvailable()) {
        return;
    }

    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (fileName.isEmpty()) {
        fileName = "./";
    }

    if (QFile::exists(fileName)) {
        language_info_t lang = language_from_file(fileName, extensions);
        editor->setGrammar(lang.grammar);
        editor->openFile(fileName);
        fileName = QFileInfo(fileName).path();
    }

    sidebar->setRootPath(fileName);
}

void MainWindow::setupEditor()
{
    editor = new Editor(this);
    editor->settings = &editor_settings;

    editor->setTheme(theme);
    editor->setupEditor();

    // setCentralWidget(editor);
}

void MainWindow::setupMenu()
{
    // File
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

    // View
    viewMenu = new QMenu(tr("&View"), this);
    // viewMenu->addAction(
    //     tr("Toggle Sidebar"),
    //     this, [this]() { sidebar->toggle(); });
    // viewMenu->addAction(
    //     tr("Toggle Minimap"),
    //     this, [this]() { editor_settings.miniMap = !editor_settings.miniMap; });
    // viewMenu->addAction(
    //     tr("Toggle Statusbar"),
    //     this, [this]() { saveFile(); });

    menuBar()->addMenu(viewMenu);

    // Help
    QMenu* helpMenu = new QMenu(tr("&Help"), this);
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
