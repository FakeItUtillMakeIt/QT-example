#pragma once

#ifndef IMG_RPOC_H
#define IMG_RPOC_H

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include <QImage>
#include <QPixmap>
#include <QDebug>

using namespace cv;

class ImgProc
{
private:
	 Mat srcimg;

public:
	void setSrcImage(const Mat& img);
	void operateSobel();
	void operateGauss();
	void getDstImage(Mat& img);
	void operateContrast(double alpha);
	void operateBrightness(int beta);
	void operateGamma(double gamma);
	void operateHoughLine(int thresold1, int thresold2);
	
	QImage cvMat2QImage(const Mat& img);
	QPixmap cvMat2QPixmap(const Mat& img);



};

#endif