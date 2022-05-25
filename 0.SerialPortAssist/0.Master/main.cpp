#include "SimulationSendSerialPort.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimulationSendSerialPort w;
    w.show();
    return a.exec();
}
