#include "ImgProc.h"
/**
    @brief 
    @param img 
**/
void ImgProc::setSrcImage(const Mat& img) {
	srcimg = img;

}
/**
    @brief 
**/
void ImgProc::operateSobel() {
	Sobel(srcimg, srcimg, srcimg.depth(),0,1);
}
/**
    @brief 
**/
void ImgProc::operateGauss() {
	GaussianBlur(srcimg, srcimg, Size(3, 3), 0);
}
/**
    @brief 
    @param img 
**/
void ImgProc::getDstImage(Mat& img) {
	img = srcimg;
}


//BGRÈýÍ¨µÀ
void ImgProc::operateContrast(double alpha) {

	auto newimg = srcimg;
	for (int y=0;y<srcimg.rows;y++)
	{
		for (int x=0;x<srcimg.cols;x++)
		{
			for (int c=0;c<srcimg.channels();c++)
			{
				srcimg.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * newimg.at<Vec3b>(y, x)[c]);
			}
		}
	}


}
/**
    @brief 
    @param beta 
**/
void ImgProc::operateBrightness(int beta) {

	auto newimg = srcimg;
	for (int y = 0; y < srcimg.rows; y++)
	{
		for (int x = 0; x < srcimg.cols; x++)
		{
			for (int c = 0; c < srcimg.channels(); c++)
			{
				srcimg.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(1.0*newimg.at<Vec3b>(y, x)[c]+beta*10);
			}
		}
	}
}

/**
    @brief 
    @param gamma 
**/
void ImgProc::operateGamma(double gamma) {
	auto newimg = srcimg;
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i=0;i<256;i++)
	{
		p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
		
	}
	cv::Mat res = newimg.clone();
	LUT(newimg, lookUpTable, srcimg);
}

void ImgProc::operateHoughLine(int thresold1,int thresold2) {

	auto newimg = srcimg;
	Canny(newimg, srcimg, thresold1, thresold2);


}

/**
    @brief  
    @param  img 
    @retval     
**/
QImage ImgProc::cvMat2QImage(const Mat& img) {

	switch (img.type())
	{
	case CV_8UC4:
	{
		QImage image1(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_ARGB32);
		return image1;
	}
		
	case CV_8UC3:
	{	QImage image1(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_RGB888);
		return image1.rgbSwapped();
	}
	case CV_8UC1:
	{
#if QT_VERSION>=QT_VERSION_CHECK(5,5,0)

		QImage image1(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Grayscale8);
#else
		static QVector<QRgb> sColorTable;
		if (sColorTable.isEmpty())
		{
			sColorTable.resize(256);
			for (int i = 0; i < 256; i++)
			{
				sColorTable[i] = qRgb(i, i, i);
			}
		}
		QImage image1(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_Indexed8);
		image1.setColorTable(sColorTable);
#endif
		return image1;
	}
	default:
	{
		qWarning() << "CV::cvMatToQImage";
		break;
	}
	}
	return QImage();
}
/**
    @brief  
    @param  img 
    @retval     
**/
QPixmap ImgProc::cvMat2QPixmap(const Mat& img) {
	return QPixmap::fromImage(cvMat2QImage(img));
}