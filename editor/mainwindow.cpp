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
    , icons(0)
    , editor(0)
{
    configure();

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

void MainWindow::loadTheme(const QString& name)
{
    theme_ptr _theme = theme_from_name(name, extensions);
    if (_theme) {
        theme = _theme;
        applyTheme();
    }
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
        theme = theme_from_name("Monokai", extensions);
    }

    if (settings["icon_theme"].isString()) {
        icons = icon_theme_from_name(settings["icon_theme"].asString().c_str(), extensions);
    }

    // editor settings
    if (settings["mini_map"] == true) {
        editor_settings.mini_map = true;
    }

    if (settings.isMember("font")) {
        editor_settings.font = settings["font"].asString();
    } else {
        editor_settings.font = "monospace";
    }

    if (settings.isMember("font_size")) {
        editor_settings.font_size = std::stof(settings["font_size"].asString());
    } else {
        editor_settings.font_size = 12;
    }

    if (settings.isMember("tab_size")) {
        editor_settings.tab_size = std::stoi(settings["tab_size"].asString());
    } else {
        editor_settings.tab_size = 4;
    }

    if (settings["tab_to_spaces"] == true) {
        editor_settings.tab_to_spaces = true;
    }

    // std::cout << settings << std::endl;

    // fix invalids
    if (editor_settings.font_size < 6) {
        editor_settings.font_size = 6;
    }
    if (editor_settings.font_size > 48) {
        editor_settings.font_size = 48;
    }
    if (editor_settings.tab_size < 1) {
        editor_settings.tab_size = 1;
    }
    if (editor_settings.tab_size > 8) {
        editor_settings.tab_size = 8;
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
    if (!theme_tabbar(theme, "sideBar", *tabs)) {
        if (!theme_tabbar(theme, "menu", *tabs)) {
            theme_tabbar(theme, "editor", *tabs);
        }
    }
    if (!theme_statusbar(theme, "statusBar", *statusBar())) {
        theme_sidebar(theme, "editor", *statusBar());
    }

    statusBar()->showMessage("hello", 1000);
}

void MainWindow::applySettings()
{
    if (settings["sidebar"] == true) {
        sidebar->show();

        QFont font;
        font.setFamily(editor_settings.font.c_str());
        font.setPointSize(editor_settings.font_size);
        font.setFixedPitch(true);

        sidebar->setFont(font);
    } else {
        sidebar->hide();
    }

    if (settings["statusbar"] == true) {
        statusBar()->show();
    } else {
        statusBar()->hide();
    }
}

void MainWindow::setupLayout()
{
    readSettings();

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    editors = new QStackedWidget();

    // editors->setMargin(0);
    // editors->setSpacing(0);

    setStyleSheet("* { font-size: 12pt; } QPlainTextEdit { font-size: 12pt; }");

    sidebar = new Sidebar(this);
    sidebar->mainWindow = this;

    tabs = new Tabs(this);
    tabs->setDrawBase(false);
    tabs->setExpanding(false);
    tabs->setTabsClosable(true);

    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabSelected(int)));
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClose(int)));

    QWidget* mainPane = new QWidget();
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(tabs);
    vbox->addWidget(editors);
    vbox->setMargin(0);
    vbox->setSpacing(0);
    mainPane->setLayout(vbox);

    splitter->addWidget(sidebar);
    splitter->addWidget(mainPane);
    splitter->setStretchFactor(1, 3);

    setCentralWidget(splitter);

    central = splitter;
}

void MainWindow::newFile()
{
    int tabIdx = tabs->addTab("untitled");
    setupEditor();
    editors->addWidget(editor);
    tabs->setTabData(tabIdx, QVariant::fromValue(editor));
    tabSelected(tabIdx);
}

void MainWindow::saveFile()
{
    QString fileName = editor->fileName;

    if (QFileInfo(fileName).fileName() == "untitled") {
        fileName = "";
    }

    // std::cout << fileName.toUtf8().constData() << std::endl;
    if (fileName.isNull() || fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "");
    }

    if (!fileName.isEmpty()) {
        editor->saveFile(QFileInfo(fileName).absoluteFilePath());
        tabs->setTabText(tabs->currentIndex(), QFileInfo(editor->fileName).fileName());
    }
}

void MainWindow::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!editors->count()) {
        sidebar->setRootPath(QFileInfo(fileName).path());
    }

    // directory open requested
    if (QFileInfo(fileName).isDir()) {
        if (tabs->count() == 0) {
            newFile();
        }
        return;
    }

    if (QFile::exists(fileName)) {
        fileName = QFileInfo(fileName).absoluteFilePath();
        openTab(fileName);
        if (editor->fileName != fileName) {
            editor->setLanguage(language_from_file(fileName, extensions));
            editor->openFile(fileName);
        }
        return;
    }
}

void MainWindow::setupEditor()
{
    editor = new Editor(this);
    editor->settings = &editor_settings;
    editor->setTheme(theme);
    editor->setupEditor();
}

void MainWindow::tabSelected(int index)
{
    // std::cout << "Tabs:" << tabs->count() << std::endl;
    // std::cout << "Editors:" << editors->count() << std::endl;
    // std::cout << "Selected:" << index << std::endl;
    if (index != -1) {
        QVariant data = tabs->tabData(index);
        Editor* _editor = qvariant_cast<Editor*>(data);
        editors->setCurrentWidget(_editor);
        tabs->setCurrentIndex(index);
        editor = _editor;
    }
}

void MainWindow::tabClose(int index)
{
    // std::cout << "Close " << index << std::endl;
    if (index != -1) {
        tabSelected(index);
        QVariant data = tabs->tabData(index);
        Editor* _editor = qvariant_cast<Editor*>(data);
        if (_editor) {
            editors->removeWidget(_editor);
            tabs->removeTab(index);
        }
    }

    if (!tabs->count()) {
        // close();
        newFile();
        return;
    }

    tabSelected(0);
}

Editor* MainWindow::openTab(const QString& _path)
{
    QString path = _path;

    int tabIdx = -1;
    for (int i = 0; i < tabs->count(); i++) {
        QVariant data = tabs->tabData(i);
        Editor* _editor = qvariant_cast<Editor*>(data);
        if (_editor->fileName == path) {
            tabIdx = i;
            std::cout << i << std::endl;
            break;
        }
    }

    if (tabIdx != -1) {
        tabSelected(tabIdx);
        return editor;
    }

    QString _fileName = QFileInfo(path).fileName();
    if (_fileName.isEmpty()) {
        _fileName = "untitled";
        path = QFileInfo(_fileName).absoluteFilePath();
    }

    tabIdx = tabs->addTab(_fileName);
    setupEditor(); // << creates a new editor
    editors->addWidget(editor);
    tabs->setTabData(tabIdx, QVariant::fromValue(editor));
    tabSelected(tabIdx);
    return editor;
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

void MainWindow::closeEvent(QCloseEvent* event)
{
    // save geometry
    QMainWindow::closeEvent(event);
}
void MainWindow::readSettings()
{
}
