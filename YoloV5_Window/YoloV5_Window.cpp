

#include "YoloV5_Window.h"
#include "yolo.h"

Net_config yolo_nets[4] = {
	{0.5, 0.5, 0.5, "yolov5s"},
	{0.5, 0.5, 0.5,  "yolov5m"},
	{0.5, 0.5, 0.5, "yolov5l"},
	{0.5, 0.5, 0.5, "yolov5x"}
};

YoloV5_Window::YoloV5_Window(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	ui.mainToolBar->hide();
	ui.menuBar->hide();
	ui.statusBar->hide();

	QGridLayout* windowLayout = new QGridLayout(this);
	btnSelectFile = new QPushButton();
	btnSelectFile->setText(QString::fromLocal8Bit("选择图片文件："));
	btnOK = new QPushButton();
	btnOK->setText(QString::fromLocal8Bit("确定"));

	labelVideoCap = new QLabel(QString::fromLocal8Bit("选择摄像头设备:"));
	cbxVideoCapDevice = new QComboBox();
	int ind = 0;
	for (auto deviceC:QCameraInfo::availableCameras())
	{
		cbxVideoCapDevice->addItem(deviceC.deviceName(),QVariant(ind));
		ind++;
		
	}

	btnVideoCapOK = new QPushButton(QString::fromLocal8Bit("确定"));
	btnVideoCapClose = new QPushButton(QString::fromLocal8Bit("播放/暂停视频流"));
	

	labelDisplay = new QLabel();
	labelDisplay->setMaximumSize(QSize(320, 240));
	labelDisplay1 = new QLabel();
	labelDisplay1->setMaximumSize(QSize(320, 240));
	imgSelectLabel = new QLabel(QString::fromLocal8Bit("选择图片文件:"));
	imgOpStyle = new QLabel(QString::fromLocal8Bit("图片操作方式:"));
	imgSelectLabel->setAlignment(Qt::AlignCenter);
	imgOpStyle->setAlignment(Qt::AlignCenter);
	operationBox = new QComboBox();
	operationBox->addItem(QString::fromLocal8Bit("高斯滤波"),QVariant(0));
	operationBox->addItem(QString::fromLocal8Bit("塞贝尔滤波"), QVariant(1));
	operationBox->addItem(QString::fromLocal8Bit("目标检测"), QVariant(2));
	operationBox->addItem(QString::fromLocal8Bit("对比度调节"), QVariant(3));
	operationBox->addItem(QString::fromLocal8Bit("亮度调节"), QVariant(4));
	operationBox->addItem(QString::fromLocal8Bit("伽马调节"), QVariant(5));
	operationBox->addItem(QString::fromLocal8Bit("霍夫变换"), QVariant(6));

	constrastLabel = new QLabel(QString::fromLocal8Bit("对比度调节:"));
	brightnessLabel = new QLabel(QString::fromLocal8Bit("亮度调节:"));
	contrastSlider = new QSlider(Qt::Horizontal);
	brightnessSlider = new QSlider(Qt::Horizontal);
	
	contrastSlider->setRange(0.0,10.0);
	brightnessSlider->setRange(0, 100);
	constrastLabel->hide();
	contrastSlider->hide();
	brightnessLabel->hide();
	brightnessSlider->hide();

	windowLayout->addWidget(imgSelectLabel, 0, 0, 1, 1);
	windowLayout->addWidget(btnSelectFile, 0, 1,1,1);
	windowLayout->addWidget(imgOpStyle, 1, 0, 1, 1);
	windowLayout->addWidget(operationBox, 1, 1, 1, 1);
	windowLayout->addWidget(btnOK, 1, 2,1,2);
	windowLayout->addWidget(labelVideoCap, 2, 0, 1, 1);
	windowLayout->addWidget(cbxVideoCapDevice, 2, 1, 1, 1);
	windowLayout->addWidget(btnVideoCapOK, 2, 2, 1, 1);
	windowLayout->addWidget(btnVideoCapClose, 2, 3, 1, 1);
	windowLayout->addWidget(constrastLabel, 3, 0, 1, 1);
	windowLayout->addWidget(contrastSlider, 3, 1, 1, 1);
	windowLayout->addWidget(brightnessLabel, 3, 2, 1, 1);
	windowLayout->addWidget(brightnessSlider, 3, 3, 1, 1);

	windowLayout->addWidget(labelDisplay, 4, 0, 4, 2);
	windowLayout->addWidget(labelDisplay1, 4, 2, 4, 2);

	this->centralWidget()->setLayout(windowLayout);
	

	connect(btnSelectFile, &QPushButton::clicked, this, &YoloV5_Window::clickSelectFile);
	connect(btnOK, &QPushButton::clicked, this, &YoloV5_Window::clickOK);
	connect(btnVideoCapOK, &QPushButton::clicked, this, &YoloV5_Window::clickCameraDeviceOK);
	connect(btnVideoCapClose, &QPushButton::clicked, this, [&]() {
		closeFlag = !closeFlag;
		
		});
}

/**
    @brief 选择文件
**/
void YoloV5_Window::clickSelectFile() {
	QFileDialog* fileDialog;
	fileDialog = new QFileDialog(this);
	fileDialog->setNameFilter(tr("Images(*.jpg *.png *.mp4 *.flv)"));
	if (fileDialog->exec())
	{
		selectImgName = fileDialog->selectedFiles()[0];
	}
	if (selectImgName.endsWith("jpg", Qt::CaseInsensitive) || selectImgName.endsWith("png",Qt::CaseInsensitive))
	{
		QPixmap pixmap(selectImgName);
		pixmap = pixmap.scaled(QSize(320, 240));

		labelDisplay->setPixmap(pixmap);
		labelDisplay->show();
	}
	else if(selectImgName.endsWith("mp4", Qt::CaseInsensitive) || selectImgName.endsWith("flv", Qt::CaseInsensitive))
	{
		
		capture = new VideoCapture(selectImgName.toStdString());
		if (capture->isOpened())
		{
			closeFlag = false;
			auto height = capture->get(CAP_PROP_FRAME_HEIGHT);
			auto width = capture->get(CAP_PROP_FRAME_WIDTH);
			detector = FaceDetectorYN::create(fd_model.toStdString(), "", Size(320, 240), scoreThreshold, nms_threshold, top_k);
			for (;;)
			{
				Mat firstFrame;
				if (!capture->read(firstFrame)) {
					break;
				}
				cv::resize(firstFrame, firstFrame, Size(320, 240));
				labelDisplay->setPixmap(imgProc.cvMat2QPixmap(firstFrame));

				detector->setInputSize(Size(320, 240));
				tm.start();
				Mat faces;
				detector->detect(firstFrame, faces);
				tm.stop();
				visualize(firstFrame, -1, faces, tm.getFPS());
				labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(firstFrame));
				waitKey(10);
				
				if (closeFlag)
				{
					break;
				}
			}
			
		}

		capture->release();
	}
	
}


void YoloV5_Window::clickCameraDeviceOK() {
	auto currentCameraDevice = cbxVideoCapDevice->currentData();
	if (currentCameraDevice.isNull())
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("无摄像头设备"));
		return;
	}
	capture = new VideoCapture(currentCameraDevice.toInt());
	if (capture->isOpened())
	{
		closeFlag = false;
		Mat firstFrame;
		auto height = capture->get(CAP_PROP_FRAME_HEIGHT);
		auto width = capture->get(CAP_PROP_FRAME_WIDTH);
		detector = FaceDetectorYN::create(fd_model.toStdString(), "", Size(320, 240), scoreThreshold, nms_threshold, top_k);
		for (;;)
		{
			
			if (!capture->read(firstFrame)) {
				break;
			}
			cv::resize(firstFrame, firstFrame, Size(320, 240));
			labelDisplay->setPixmap(imgProc.cvMat2QPixmap(firstFrame));

			detector->setInputSize(Size(320, 240));
			tm.start();
			Mat faces;
			detector->detect(firstFrame, faces);
			tm.stop();
			visualize(firstFrame, -1, faces, tm.getFPS());
			labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(firstFrame));
			waitKey(10);
			if (closeFlag)
			{
				break;
			}

		}

	}
	//capture->release();
}

void YoloV5_Window::visualize(Mat& input, int frame, Mat& faces, double fps, int thickness )
{
	std::string fpsString = cv::format("FPS : %.2f", (float)fps);
	if (frame >= 0)
		cout << "Frame " << frame << ", ";
	cout << "FPS: " << fpsString << endl;
	for (int i = 0; i < faces.rows; i++)
	{
		// Print results
		cout << "Face " << i
			<< ", top-left coordinates: (" << faces.at<float>(i, 0) << ", " << faces.at<float>(i, 1) << "), "
			<< "box width: " << faces.at<float>(i, 2) << ", box height: " << faces.at<float>(i, 3) << ", "
			<< "score: " << cv::format("%.2f", faces.at<float>(i, 14))
			<< endl;
		// Draw bounding box 画框
		rectangle(input, Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), Scalar(0, 255, 0), thickness);
		// Draw landmarks 画点
		circle(input, Point2i(int(faces.at<float>(i, 4)), int(faces.at<float>(i, 5))), 2, Scalar(255, 0, 0), thickness);
		circle(input, Point2i(int(faces.at<float>(i, 6)), int(faces.at<float>(i, 7))), 2, Scalar(0, 0, 255), thickness);
		circle(input, Point2i(int(faces.at<float>(i, 8)), int(faces.at<float>(i, 9))), 2, Scalar(0, 255, 0), thickness);
		circle(input, Point2i(int(faces.at<float>(i, 10)), int(faces.at<float>(i, 11))), 2, Scalar(255, 0, 255), thickness);
		circle(input, Point2i(int(faces.at<float>(i, 12)), int(faces.at<float>(i, 13))), 2, Scalar(0, 255, 255), thickness);
	}
	putText(input, fpsString, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
}


/**
    @brief 情况处理
**/
void YoloV5_Window::clickOK() {

	constrastLabel->hide();
	contrastSlider->hide();
	brightnessLabel->hide();
	brightnessSlider->hide();
	labelDisplay1->clear();
	int usr_Data = (operationBox->currentData()).toInt();
	
	img=imread(selectImgName.toStdString());
	if (img.empty())
	{
		qWarning() << "warning";
		return;
	}
	cv::resize(img, changImg, Size(320, 240));

	switch (usr_Data)
	{
	case 0:
	{
		imgProc.setSrcImage(changImg);
		imgProc.operateGauss();
		imgProc.getDstImage(changImg);
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		break;
	}

	case 1:
	{
		imgProc.setSrcImage(changImg);
		imgProc.operateSobel();
		imgProc.getDstImage(changImg);
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		break;
	}

	case 2:
		operate_yolov5();
		break;
	case 3:
	{
		constrastLabel->show();
		contrastSlider->show();
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		connect(contrastSlider, &QSlider::valueChanged, this, &YoloV5_Window::operateContrast);
		break;
	}
	case 4:
	{
		brightnessLabel->show();
		brightnessSlider->show();
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		connect(brightnessSlider, &QSlider::valueChanged, this, &YoloV5_Window::operateBrightness);
		break;
	}
	case 5:
	{
		brightnessLabel->show();
		brightnessSlider->show();
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		connect(brightnessSlider, &QSlider::valueChanged, this, &YoloV5_Window::operateHoughLine);
	}
	case 6:
	{
		brightnessSlider->show();
		contrastSlider->show();
		labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
		connect(brightnessSlider, &QSlider::valueChanged, this, &YoloV5_Window::operateHoughLine);
		connect(contrastSlider, &QSlider::valueChanged, this, &YoloV5_Window::operateHoughLine);
	}
	default:
		break;
	}

}

/**
    @brief 对比度
**/
void YoloV5_Window::operateContrast() {

	
	Mat newimg;
	cv::resize(img, newimg, Size(320, 240));
	imgProc.setSrcImage(newimg);
	imgProc.operateContrast(contrastSlider->value());
	imgProc.getDstImage(newimg);
	labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(newimg));
		
}

/**
    @brief 亮度
**/
void YoloV5_Window::operateBrightness() {

	Mat newimg;
	cv::resize(img, newimg, Size(320, 240));
	 
	imgProc.setSrcImage(newimg);
	imgProc.operateBrightness(contrastSlider->value());
	imgProc.getDstImage(newimg);
	labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(newimg));
		
}

/**
    @brief gamma 调节
**/
void YoloV5_Window::operateGamma() {

	Mat newimg;
	cv::resize(img, newimg, Size(320, 240));

	imgProc.setSrcImage(newimg);
	imgProc.operateGamma(brightnessSlider->value() / 100.0);
	imgProc.getDstImage(newimg);
	labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(newimg));
}

void YoloV5_Window::operateHoughLine() {

	Mat newimg;
	cv::resize(img, newimg, Size(320, 240));
	imgProc.setSrcImage(newimg);
	imgProc.operateHoughLine(contrastSlider->value()*10, brightnessSlider->value()*5);
	imgProc.getDstImage(changImg);
	labelDisplay1->setPixmap(imgProc.cvMat2QPixmap(changImg));
}

/**
    @brief 目标检测网络调用
**/
void YoloV5_Window::operate_yolov5() {
	try
	{
		YOLO yolo_model(yolo_nets[0]);

		string imgpath = selectImgName.toStdString();
		Mat srcimg = imread(imgpath);
		yolo_model.detect(srcimg);


		QString newimg;
		newimg = QString::fromStdString(imgpath);
		QString dealpath = newimg.split(".")[0] + "reg." + newimg.split(".")[1];
		imwrite(dealpath.toStdString(), srcimg);
		QPixmap pixmap(dealpath);
		pixmap = pixmap.scaled(QSize(320, 240));
		labelDisplay1->setPixmap(pixmap);
		labelDisplay1->resize(320, 240);
		labelDisplay1->show();
	}
	catch (cv::Exception)
	{
		QMessageBox::warning(this, "error", QString::fromLocal8Bit("无图像输入"));
	}
}