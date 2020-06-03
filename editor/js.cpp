#include <QDebug>

#include "commands.h"
#include "editor.h"
#include "js.h"
#include "mainwindow.h"

JSConsole::JSConsole(QObject* parent)
    : QObject(parent)
{
}

void JSConsole::log(QString msg)
{
    qDebug() << msg;
}

JSEditor::JSEditor(QObject* parent)
    : QObject(parent)
{
}

void JSEditor::toggleComment()
{
    Commands::toggleComment((Editor*)parent());
}

void JSEditor::indent()
{
    Commands::indent((Editor*)parent());
}

void JSEditor::unindent()
{
    Commands::unindent((Editor*)parent());
}

JSApp::JSApp(QObject* parent)
    : QObject(parent)
{
}

void JSApp::tab(int i)
{
    MainWindow* mw = MainWindow::instance();
    mw->tabSelected(i);
}

void JSApp::close()
{
    MainWindow* mw = MainWindow::instance();
    mw->tabClose(mw->currentTab());
}

void JSApp::exit()
{
    MainWindow::instance()->close();
}

QJSValue JSApp::editor()
{
    MainWindow* mw = MainWindow::instance();
    Editor* editor = mw->currentEditor();
    QJSValue value = mw->jsEngine().newQObject(editor->jsobj);
    return value;
}
