#include <QDebug>
#include <QFile>
#include <QWebSettings>
#include "engine.h"

Engine::Engine(QWidget *parent) :
    QWidget(parent)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    view = new QWebView(this);
    view->hide();
    frame = view->page()->mainFrame();
    
    // connect(frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(setupEnvironment()));
    // view->setHtml("<script>document</script>");
    
    inspector = new QWebInspector();
    inspector->setPage(view->page());
    // inspector->setVisible(true);
    
    show();
    
    setupEnvironment();
}

void Engine::setupEnvironment()
{
    frame->addToJavaScriptWindowObject("engine", this);
}

QVariant Engine::runScript(QString script)
{
    return frame->evaluateJavaScript(script);
}

void Engine::showInspector()
{
    inspector->setVisible(true);
}

QVariant Engine::runScriptFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        return runScript(file.readAll());
    }
    return QVariant();
}

void Engine::log(QString message)
{
    qDebug() << message;
}