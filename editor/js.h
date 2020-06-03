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
    void indent();
    void unindent();
};

class JSApp : public QObject {
    Q_OBJECT

public:
    JSApp(QObject* parent = 0);

public slots:
    QJSValue editor();
};

#endif // JS_H