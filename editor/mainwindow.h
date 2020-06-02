#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editor.h"
#include "extension.h"
#include "icons.h"
#include "json/json.h"
#include "sidebar.h"
#include "tabs.h"
#include "theme.h"

#include <QMainWindow>
#include <QTimer>

Q_DECLARE_METATYPE(Editor*)

class QStackedWidget;

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
    void setupEditor();
    void setupMenu();
    void setupLayout();
    void applyTheme();
    void applySettings();

    Editor* openTab(const QString& path = QString());

    void closeEvent(QCloseEvent* event);
    void readSettings();

public:
    Editor* editor; // todo << remove, adds confusion
    std::vector<Extension> extensions;
    theme_ptr theme;
    icon_theme_ptr icons;

    struct editor_settings_t editor_settings;
    Json::Value settings;

private Q_SLOTS:
    void warmConfigure();
    void tabSelected(int index);
    void tabClose(int index);

private:
    QMenu* fileMenu;
    QMenu* viewMenu;

    QWidget* central;
    QStackedWidget* editors;
    Tabs* tabs;
    Sidebar* sidebar;

    QTimer updateTimer;
};

#endif // MAINWINDOW_H