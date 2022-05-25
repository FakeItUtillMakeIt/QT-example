#include "RocketGroundComProtol.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RocketGroundComProtol w;
    w.show();
    return a.exec();
}
