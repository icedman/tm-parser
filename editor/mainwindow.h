#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editor.h"
#include "extension.h"
#include "theme.h"

#include <QMainWindow>
#include <QTimer>

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
    void setupFileMenu();
    void setupHelpMenu();

    Editor* editor;
    std::vector<Extension> extensions;

    theme_ptr theme;
};

#endif // MAINWINDOW_H