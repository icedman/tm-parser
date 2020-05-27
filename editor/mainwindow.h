#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include "theme.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QPlainTextEdit;   //<< one-block = one-line
typedef QPlainTextEdit QSublimeTextEdit;
QT_END_NAMESPACE

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

    void higlightAllBlocks();

    QSublimeTextEdit* editor;
    Highlighter* highlighter;
    QTimer updateTimer;

    theme_ptr theme;
private Q_SLOTS:
    void onUpdate();
};
//! [0]

#endif // MAINWINDOW_H