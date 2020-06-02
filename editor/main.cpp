#include "json/json.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    // window.resize(640, 512);
    window.resize(1200, 700);

    if (argc > 1) {
        window.openFile(argv[1]);
    } else {
        window.newFile();
    }

    window.show();

    app.setQuitOnLastWindowClosed(true);
    return app.exec();
}