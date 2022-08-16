#include "MutliThreadTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MutliThreadTest w;
    w.show();
    return a.exec();
}
