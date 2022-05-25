#include "SpiderWebWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpiderWebWidget w;
    w.show();
    return a.exec();
}

