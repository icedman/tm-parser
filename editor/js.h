#ifndef JS_H
#define JS_H

#include <QJSEngine>
#include <QObject>

typedef void set_text_t(QObject *obj, QString);
typedef QString get_text_t(QObject *obj);

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
    void zoomIn();
    void zoomOut();
    void addExtraCursor();
    void removeExtraCursors();
    bool find(QString string, QString options = QString());
    bool findAndCreateCursor(QString string, QString options = QString());

    QString selectedText();
};

enum ui_type_t {
    UI_UNKNOWN      = 0,
    UI_PANEL        = 1 << 1,
    UI_HBOX         = 1 << 2,
    UI_VBOX         = 1 << 3,
    UI_LABEL        = 1 << 4,
    UI_INPUTTEXT    = 1 << 5,
    UI_BUTTON       = 1 << 6,
    UI_WIDGET       = 1 << 7
};

class JSUIObject : public QObject {
    Q_OBJECT

public:
    JSUIObject(QObject* parent = 0, ui_type_t type = UI_UNKNOWN);
    QJSValue self;
    ui_type_t type;

public slots:
    // panel
    QJSValue vbox(QJSValue parent = QJSValue());
    QJSValue hbox(QJSValue parent = QJSValue());
    QJSValue button(QString text, QJSValue parent = QJSValue());
    QJSValue toggleButton(QString text, QJSValue parent = QJSValue());
    QJSValue label(QString text, QJSValue parent = QJSValue());
    QJSValue inputText(QString text, QJSValue parent = QJSValue());

    // button
    QJSValue isChecked();

    // layout
    QJSValue addStretch(QJSValue stretch = QJSValue(1));

    // all
    QJSValue setText(QString text);
    QJSValue getText();
    QJSValue setFocus();
    QJSValue setMinimumSize(int w, int h);
    QJSValue setMaximumSize(int w, int h);
    QJSValue setStyleSheet(QString s);
    QJSValue show();
    QJSValue hide();

    // custom widget
    QJSValue addWidget(QJSValue widget, QJSValue parent = QJSValue());

private Q_SLOTS:
    void clicked();
    void valueChanged(QString value);
    void submitted();

public:
    set_text_t *set_text;
    get_text_t *get_text;
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