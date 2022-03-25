#include "YoloV5_Window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");

    YoloV5_Window w;
    w.setGeometry(100, 100, 640, 480);
    w.setMaximumSize(QSize(640, 480));
    w.setWindowTitle(QString::fromLocal8Bit("yolov5s π”√GUI"));
    w.show();
    return a.exec();
}
