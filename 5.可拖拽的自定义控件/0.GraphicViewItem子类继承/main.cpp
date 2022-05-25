#include "opencvQTWidget1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    opencvQTWidget1 w;
    w.show();
    return a.exec();
}
