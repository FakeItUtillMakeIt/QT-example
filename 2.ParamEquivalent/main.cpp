#include "ParamEquivalent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParamEquivalent w;
    w.show();
    return a.exec();
}
