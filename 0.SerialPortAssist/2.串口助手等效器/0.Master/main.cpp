#include "MasterSerial.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MasterSerial w;
    w.show();
    return a.exec();
}
