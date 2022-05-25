#include "CustomParamComponent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomParamComponent w;
    w.show();
    return a.exec();
}
