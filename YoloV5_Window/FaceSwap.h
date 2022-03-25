#pragma once
#include <QtNetwork>
#include <QtWidgets>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace cv;
using namespace dnn;
using namespace std;

class FaceSwap
{

private:
	void getFaceFromAPI(QString imgpath, Mat& faceData);

};

