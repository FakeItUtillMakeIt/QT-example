#include "SlaveSerial.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlaveSerial w;
    w.show();
    return a.exec();
}
