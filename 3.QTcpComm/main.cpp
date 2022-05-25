#include "TcpCommTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpCommTest w;
    w.show();
    return a.exec();
}
