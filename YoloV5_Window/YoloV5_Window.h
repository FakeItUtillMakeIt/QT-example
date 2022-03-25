#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_YoloV5_Window.h"
#include <QtWidgets>
#include <QSlider>
#include <QString>
#include <QCamera> 
#include <QCameraInfo>
#include "ImgProc.h"
#include <opencv2/videoio.hpp>

using namespace cv;

class YoloV5_Window : public QMainWindow
{
    Q_OBJECT

public:
    YoloV5_Window(QWidget *parent = Q_NULLPTR);
    ~YoloV5_Window() {
       
        
        detector?detector.release():NULL;
        tm.stop();
    }

private:
    Ui::YoloV5_WindowClass ui;

private:
    QPushButton* btnOK;
    QPushButton* btnSelectFile;
    
    QComboBox* operationBox;
    QString selectImgName;
    QLabel* labelDisplay;
    QLabel* labelDisplay1;
    QLabel* imgOpStyle;
    QLabel* imgSelectLabel;
    QLabel* constrastLabel;
    QLabel* brightnessLabel;
    QSlider* contrastSlider;
    QSlider* brightnessSlider;

    QLabel* labelVideoCap;
    QComboBox* cbxVideoCapDevice;
    QPushButton* btnVideoCapOK;
    QPushButton* btnVideoCapClose;

    int waitTime = 10;


    bool closeFlag = false;
    
private:
    ImgProc imgProc;
    Mat img, changImg;
    VideoCapture* capture;
    Ptr<FaceDetectorYN> detector = nullptr;
    QString fd_model = "face_detection_yunet_2021dec.onnx";
    QString fr_model = "face_recognition_sface_2021dec.onnx";
    float scoreThreshold = 0.9;
    float nms_threshold = 0.3;
    int top_k = 50;
    TickMeter tm;


private:
    void operate_yolov5();
    void operateContrast();
    void operateBrightness();
    void operateGamma();
    void operateHoughLine();
    void visualize(Mat& input, int frame, Mat& faces, double fps, int thickness = 2);

private slots:
    void clickOK();
    void clickSelectFile();
    void clickCameraDeviceOK();

};
