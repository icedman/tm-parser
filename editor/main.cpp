#include <QApplication>

#include "json/json.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    // QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    // qobject_cast<QApplication *>(app.data())
    QApplication *app = new QApplication(argc, argv);
    app->setQuitOnLastWindowClosed(true);

    MainWindow window;
    // window.resize(640, 512);
    window.resize(1200, 700);

    if (argc > 1) {
        window.openFile(argv[1]);
    } else {
        window.newFile();
    }

    window.show();

    return app->exec();
}