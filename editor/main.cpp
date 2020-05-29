#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(640, 512);

    if (argc > 1) {
        window.openFile(argv[1]);
    }

    window.show();
    return app.exec();
}