#include <QDebug>
#include <QPushButton>
#include <QBoxLayout>

#include "commands.h"
#include "editor.h"
#include "panel.h"
#include "mainwindow.h"

#include "js.h"

#define JS_TO_OBJ(T, V) ((V).toQObject() && (V).toQObject()->parent() ? qobject_cast<T*>((V).toQObject()->parent()) : 0);
/*
// shorthand for
QObject *obj = layout.toQObject();
QBoxLayout *l = 0;
if (obj) {
    l = qobject_cast<QBoxLayout*>(obj->parent());
}
*/

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

void JSEditor::toggleBlockComment()
{
    Commands::toggleBlockComment((Editor*)parent());
}

void JSEditor::indent()
{
    Commands::indent((Editor*)parent());
}

void JSEditor::unindent()
{
    Commands::unindent((Editor*)parent());
}

void JSEditor::duplicateLine()
{
    Commands::duplicateLine((Editor*)parent());
}

void JSEditor::expandSelectionToLine()
{
    Commands::expandSelectionToLine((Editor*)parent());
}

JSUIObject::JSUIObject(QObject* parent)
    : QObject(parent)
{}

QJSValue JSUIObject::vbox(QJSValue layout) {
    Panel *panel = (Panel*)parent();
    QObject *obj = layout.toQObject();
    QBoxLayout *l = JS_TO_OBJ(QBoxLayout, layout);
    QVBoxLayout *box = panel->vbox(l);
    JSUIObject *jsobj = new JSUIObject((QObject*)box);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    return jsobj->self;
}

QJSValue JSUIObject::hbox(QJSValue layout) {
    Panel *panel = (Panel*)parent();
    QObject *obj = layout.toQObject();
    QBoxLayout *l = JS_TO_OBJ(QBoxLayout, layout);
    QHBoxLayout *box = panel->hbox(l);
    JSUIObject *jsobj = new JSUIObject((QObject*)box);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    return jsobj->self;
}

QJSValue JSUIObject::button(QJSValue text, QJSValue layout) {
    Panel *panel = (Panel*)parent();
    QString t = text.toString();
    QBoxLayout *l = JS_TO_OBJ(QBoxLayout, layout);
    QPushButton *btn = panel->addButton(t, l);
    JSUIObject *jsobj = new JSUIObject((QObject*)btn);
    connect(btn, SIGNAL(clicked(bool)), jsobj, SLOT(clicked()));
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    jsobj->self.setProperty("onClick", QJSValue());
    return jsobj->self;
}

void JSUIObject::clicked()
{
    QJSValue onClick = self.property("onClick");
    onClick.call();
}

void JSUIObject::valueChanged(QString val)
{
    // pass-on to value listener
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

void JSApp::newTab()
{
    MainWindow* mw = MainWindow::instance();
    mw->newFile();
}

void JSApp::closeTab()
{
    MainWindow* mw = MainWindow::instance();
    mw->tabClose(mw->currentTab());
}

void JSApp::exit()
{
    MainWindow::instance()->close();
}

QJSValue JSApp::createPanel(QJSValue name)
{
    QString n = name.toString();
    MainWindow* mw = MainWindow::instance();
    Panel *p = mw->createPanel(n);
    JSUIObject *jsobj = new JSUIObject((QObject*)p);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    return jsobj->self;
}

QJSValue JSApp::editor()
{
    MainWindow* mw = MainWindow::instance();
    Editor* editor = mw->currentEditor();
    QJSValue value = mw->jsEngine().newQObject(editor->jsobj);
    return value;
}
