#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editor.h"
#include "extension.h"
#include "theme.h"
#include "tree.h"
#include "json/json.h"

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

private:
    void configure();
    void setupEditor();
    void setupMenu();
    void setupLayout();
    void applyTheme();
    void applySettings();

private Q_SLOTS:
    void warmConfigure();

private:
    Editor* editor;

    std::vector<Extension> extensions;
    theme_ptr theme;

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