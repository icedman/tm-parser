#include <QDebug>
#include <QFile>
#include "engine.h"

Engine::Engine(QWidget *parent) :
    QWidget(parent)
{
    view = new QWebView(this);
    frame = view->page()->mainFrame();
    
    // connect(frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(setupEnvironment()));
    // view->setHtml("<script>document</script>");
    
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