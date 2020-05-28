#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include "editor.h"
#include "theme.h"

#include <QMainWindow>
#include <QTimer>

//! [0]
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);

public slots:
    void about();
    void newFile();
    void openFile(const QString& path = QString());

private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    Editor *editor;
};
//! [0]

#endif // MAINWINDOW_H