#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editor.h"
#include "extension.h"
#include "json/json.h"
#include "theme.h"
#include "icons.h"
#include "sidebar.h"

#include <QMainWindow>
#include <QTimer>

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

public:
    Editor* editor;
    std::vector<Extension> extensions;
    theme_ptr theme;
    icon_theme_ptr icons;

private Q_SLOTS:
    void warmConfigure();

private:

    QMenu* fileMenu;
    QMenu* viewMenu;

    QWidget* central;
    QStackedWidget* editors;
    Sidebar* sidebar;

    struct editor_settings_t editor_settings;
    Json::Value settings;

    QTimer updateTimer;
};

#endif // MAINWINDOW_H