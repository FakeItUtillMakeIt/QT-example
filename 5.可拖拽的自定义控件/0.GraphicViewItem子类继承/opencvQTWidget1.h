#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_opencvQTWidget1.h"
#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "customgraphicimageview.h"

using namespace cv;

class opencvQTWidget1 : public QMainWindow
{
    Q_OBJECT

public:
    opencvQTWidget1(QWidget *parent = Q_NULLPTR);

private:
    Ui::opencvQTWidget1Class ui;
    Mat m_srcImage;

    CustomGraphicImageView* m_pImg;
    void recvShowPicSignal(QImage);

private slots:
    void openImageFile();
};
