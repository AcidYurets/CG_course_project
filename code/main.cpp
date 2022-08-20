#include "MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(START_WINDOW_WIDTH, START_WINDOW_HEIGHT);

    w.show();
    return a.exec();
}
