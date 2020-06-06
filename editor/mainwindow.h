#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJSEngine>
#include <QMainWindow>
#include <QTimer>

#include "editor.h"
#include "extension.h"
#include "icons.h"
#include "js.h"
#include "json/json.h"
#include "sidebar.h"
#include "tabs.h"
#include "panel.h"
#include "theme.h"

Q_DECLARE_METATYPE(Editor*)

class QStackedWidget;
class QSplitter;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);

public slots:
    void about();
    void newFile();
    void saveFile();
    void openFile(const QString& path = QString());

public:
    void loadTheme(const QString& name);
    void configure();
    void setupMenu();
    void setupLayout();
    void applyTheme();
    void applySettings();

    Editor* openTab(const QString& path = QString());

    void closeEvent(QCloseEvent* event);
    void readSettings();

    bool processKeys(QString keys);
    void emitEvent(QString event, QString payload);

    Editor* createEditor();
    Editor* currentEditor();
    Panel* createPanel(QString name);

    QJSEngine& jsEngine() { return engine; }
    int currentTab() { return tabs->currentIndex(); }

    static MainWindow* instance();

public:
    std::vector<Extension> extensions;
    theme_ptr theme;
    icon_theme_ptr icons;

    editor_settings_ptr editor_settings;
    Json::Value settings;

public:

    void keyPressEvent(QKeyEvent* e) override;
    
public slots:
    void warmConfigure();
    void tabSelected(int index);
    void tabClose(int index);

private:
    QMenu* fileMenu;
    QMenu* viewMenu;

    QSplitter* splitter;
    QSplitter* splitterv;
    QStackedWidget* editors;
    QStackedWidget* panels;
    Tabs* tabs;
    Sidebar* sidebar;
    Panel* panel;

    QTimer updateTimer;

    QJSEngine engine;
    QJSValue module;
    QJSValue keybinding;
    QJSValue events;

    JSConsole* console;
    JSApp* app;
};

#endif // MAINWINDOW_H