#include "LicenseManagement.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LicenseManagement w;
    w.show();
    return a.exec();
}
