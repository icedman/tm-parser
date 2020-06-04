#ifndef JS_H
#define JS_H

#include <QJSEngine>
#include <QObject>

class JSConsole : public QObject {
    Q_OBJECT

public:
    JSConsole(QObject* parent = 0);

public slots:
    void log(QString msg);
};

class JSEditor : public QObject {
    Q_OBJECT

public:
    JSEditor(QObject* parent = 0);
public slots:

    void toggleComment();
    void toggleBlockComment();
    void indent();
    void unindent();
    void duplicateLine();
    void expandSelectionToLine();
};

class JSUIObject : public QObject {
    Q_OBJECT

public:
    JSUIObject(QObject* parent = 0);
    QJSValue self;

public slots:
    QJSValue vbox(QJSValue parent = QJSValue());
    QJSValue hbox(QJSValue parent = QJSValue());
    QJSValue button(QJSValue text, QJSValue parent = QJSValue());

private Q_SLOTS:
    void clicked();
    void valueChanged(QString value);
};

class JSApp : public QObject {
    Q_OBJECT

public:
    JSApp(QObject* parent = 0);

public slots:
    QJSValue editor();
    QJSValue createPanel(QJSValue name);

    void tab(int i);
    void newTab();
    void closeTab();
    void exit();
};

#endif // JS_H