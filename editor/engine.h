#pragma once

#include <QWidget>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

class Engine : public QWidget
{
    Q_OBJECT
public:
    Engine(QWidget *parent = 0);
    
    QWebView *view;
    QWebFrame *frame;
    
    QVariant runScript(QString script);
    QVariant runScriptFile(QString path);
    
private:
    
public slots:
    void log(QString message);
    
private Q_SLOTS:
    void setupEnvironment();

};