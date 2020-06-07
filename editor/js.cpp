#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>

#include "commands.h"
#include "editor.h"
#include "panel.h"
#include "mainwindow.h"

#include "js.h"

//-----------------
// some macros to make code less verbose
//-----------------
#define JS_TO_OBJ(T, V) ((V).toQObject() && (V).toQObject()->parent() ? qobject_cast<T*>((V).toQObject()->parent()) : 0)
/*
// shorthand for
QObject *obj = layout.toQObject();
QBoxLayout *l = 0;
if (obj) {
    l = qobject_cast<QBoxLayout*>(obj->parent());
}
*/

#define UI_CONTAINED(ContainedTypeEnum, ContainedType, ContainedVar, ReturnOnFail) \
    ContainedType* ContainedVar = 0; \
    if (type & ContainedTypeEnum) { \
        ContainedVar = (ContainedType*)parent(); \
    } else { \
        if (ReturnOnFail) { \
            return QJSValue(); \
        } \
    }

//-----------------
// console
//-----------------
JSConsole::JSConsole(QObject* parent)
    : QObject(parent)
{
}

void JSConsole::log(QString msg)
{
    qDebug() << msg;
}

//-----------------
// editor
//-----------------
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

QString JSEditor::selectedText()
{
    return ((Editor*)parent())->editor->textCursor().selectedText();
}

void JSEditor::zoomIn()
{
    return ((Editor*)parent())->editor->zoomIn();
}

void JSEditor::zoomOut()
{
    return ((Editor*)parent())->editor->zoomOut();
}

void JSEditor::addExtraCursor() {
    ((Editor*)parent())->editor->addExtraCursor();
}

void JSEditor::removeExtraCursors() {
    ((Editor*)parent())->editor->removeExtraCursors();
}

bool JSEditor::find(QString string, QString options)
{
    return Commands::find((Editor*)parent(), string, options);
}

bool JSEditor::findAndCreateCursor(QString string, QString options)
{
    Editor *editor = (Editor*)parent();
    QTextCursor prev = editor->editor->textCursor();
    if (string.isEmpty()) {
        prev.select(QTextCursor::WordUnderCursor);
        editor->editor->setTextCursor(prev);
        return !prev.selectedText().isEmpty();
    }

    bool res = Commands::find(editor, string, options);
    if (res) {
        editor->editor->addExtraCursor(prev);
    }
    return res;
}

//-----------------
// panels
//-----------------
JSUIObject::JSUIObject(QObject* parent, ui_type_t _type)
    : QObject(parent)
    , type(_type)
    , set_text(0)
{}

QJSValue JSUIObject::addWidget(QJSValue widget, QJSValue parent)
{
//    UI_CONTAINED(UI_PANEL, Panel, contained, true);
//    QPushButton *btn = contained->addButton(text, JS_TO_OBJ(QBoxLayout, layout));
//    JSUIObject *jsobj = new JSUIObject((QObject*)btn, UI_BUTTON);
//    connect(btn, SIGNAL(clicked(bool)), jsobj, SLOT(clicked()));
//    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
//    return jsobj->self;
    return QJSValue();        
}

QJSValue JSUIObject::vbox(QJSValue layout) {
    UI_CONTAINED(UI_PANEL, Panel, contained, true);
    QObject *obj = layout.toQObject();
    QVBoxLayout *box = contained->vbox(JS_TO_OBJ(QBoxLayout, layout));
    JSUIObject *jsobj = new JSUIObject((QObject*)box, UI_VBOX);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    return jsobj->self;
}

QJSValue JSUIObject::hbox(QJSValue layout) {
    UI_CONTAINED(UI_PANEL, Panel, contained, true);
    QObject *obj = layout.toQObject();
    QHBoxLayout *box = contained->hbox(JS_TO_OBJ(QBoxLayout, layout));
    JSUIObject *jsobj = new JSUIObject((QObject*)box, UI_HBOX);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    return jsobj->self;
}

void button_set_text(QObject *obj, QString text)
{
    QPushButton *t = (QPushButton*)obj;
    t->setText(text);
}

QString button_get_text(QObject *obj)
{
    QPushButton *t = (QPushButton*)obj;
    return t->text();
}

QJSValue JSUIObject::button(QString text, QJSValue layout) {
    UI_CONTAINED(UI_PANEL, Panel, contained, true);
    QPushButton *btn = contained->addButton(text, JS_TO_OBJ(QBoxLayout, layout));
    JSUIObject *jsobj = new JSUIObject((QObject*)btn, UI_BUTTON);
    connect(btn, SIGNAL(clicked(bool)), jsobj, SLOT(clicked()));
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    jsobj->set_text = button_set_text;
    jsobj->get_text = button_get_text;
    return jsobj->self;
}

QJSValue JSUIObject::isChecked()
{
    UI_CONTAINED(UI_BUTTON, QPushButton, contained, true);
    return contained->isChecked();
}

QJSValue JSUIObject::toggleButton(QString text, QJSValue layout)
{
    QJSValue jbtn = button(text, layout);
    QPushButton *btn = JS_TO_OBJ(QPushButton, jbtn);
    btn->setCheckable(true);
    return jbtn;
}

void label_set_text(QObject *obj, QString text)
{
    QLabel *t = (QLabel*)obj;
    t->setText(text);
}

QString label_get_text(QObject *obj)
{
    QLabel *t = (QLabel*)obj;
    return t->text();
}

QJSValue JSUIObject::label(QString text, QJSValue layout) {
    UI_CONTAINED(UI_PANEL, Panel, contained, true);
    Panel *panel = (Panel*)parent();
    QLabel *label = panel->addLabel(text, JS_TO_OBJ(QBoxLayout, layout));
    JSUIObject *jsobj = new JSUIObject((QObject*)label, UI_LABEL);
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    jsobj->set_text = label_set_text;
    jsobj->get_text = label_get_text;
    return jsobj->self;
}

void input_set_text(QObject *obj, QString text)
{
    QLineEdit *t = (QLineEdit*)obj;
    t->setText(text);
}

QString input_get_text(QObject *obj)
{
    QLineEdit *t = (QLineEdit*)obj;
    return t->text();
}

QJSValue JSUIObject::inputText(QString text, QJSValue layout) {
    UI_CONTAINED(UI_PANEL, Panel, contained, true);
    QLineEdit *line = contained->addInputText(text, JS_TO_OBJ(QBoxLayout, layout));
    JSUIObject *jsobj = new JSUIObject((QObject*)line, UI_INPUTTEXT);
    connect(line, SIGNAL(textEdited(QString)), jsobj, SLOT(valueChanged(QString)));
    connect(line, SIGNAL(returnPressed()), jsobj, SLOT(submitted()));
    jsobj->self = MainWindow::instance()->jsEngine().newQObject(jsobj);
    jsobj->set_text = input_set_text;
    jsobj->get_text = input_get_text;
    return jsobj->self;
}

void JSUIObject::clicked()
{
    QJSValue onClick = self.property("onClick");
    onClick.call();
}

void JSUIObject::valueChanged(QString val)
{
    QJSValueList args;
    args << val;
    self.setProperty("value", QJSValue(val));
    QJSValue onChange = self.property("onChange");
    onChange.call(args);
}

void JSUIObject::submitted()
{
    QJSValue onSubmit = self.property("onSubmit");
    onSubmit.call();
}

QJSValue JSUIObject::addStretch(QJSValue stretch)
{
    UI_CONTAINED((UI_VBOX | UI_HBOX), QBoxLayout, contained, true);
    contained->addStretch(stretch.toInt());
    return QJSValue();
}

QJSValue JSUIObject::setText(QString text)
{
    if (set_text) {
        set_text(parent(), text);
    }
    return QJSValue();
}

QJSValue JSUIObject::getText()
{
    if (get_text) {
        return get_text(parent());
    }
    return QJSValue();   
}

QJSValue JSUIObject::setFocus()
{
    QWidget *widget = (QWidget*)parent();
    widget->setFocus(Qt::ActiveWindowFocusReason);
    return QJSValue();
}

QJSValue JSUIObject::setMinimumSize(int w, int h)
{
    QWidget *widget = (QWidget*)parent();
    widget->setMinimumSize(w, h);
    return QJSValue();
}

QJSValue JSUIObject::setMaximumSize(int w, int h)
{
    QWidget *widget = (QWidget*)parent();
    widget->setMaximumSize(w, h);
    return QJSValue();
}

QJSValue JSUIObject::setStyleSheet(QString s)
{
    QWidget *widget = (QWidget*)parent();
    widget->setStyleSheet(s);
    return QJSValue();
}

QJSValue JSUIObject::show()
{
    return QJSValue();
}

QJSValue JSUIObject::hide()
{
    return QJSValue();
}

//-----------------
// main app
//-----------------
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
    JSUIObject *jsobj = new JSUIObject((QObject*)p, UI_PANEL);
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

void JSApp::theme(QString name)
{
    MainWindow* mw = MainWindow::instance();
    mw->loadTheme(name);
}