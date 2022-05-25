#include "opencvQTWidget1.h"

opencvQTWidget1::opencvQTWidget1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &opencvQTWidget1::openImageFile);
}


void opencvQTWidget1::openImageFile() {
	QString fileName = QFileDialog::getOpenFileName(this, "open Image", "", "Image File(*.bmp *.jpg *.jpeg *.png *.flv)");
	QTextCodec* code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(fileName).data();
	if (!fileName.endsWith(".flv"))
	{
		m_srcImage = imread(name);
		if (m_srcImage.data)
		{
			cvtColor(m_srcImage, m_srcImage, COLOR_BGR2RGB);//BGR转化为RGB
			QImage::Format format = QImage::Format_RGB888;
			switch (m_srcImage.type())
			{
			case CV_8UC1:
				format = QImage::Format_Indexed8;
				break;
			case CV_8UC3:
				format = QImage::Format_RGB888;
				break;
			case CV_8UC4:
				format = QImage::Format_ARGB32;
				break;
			}
			QImage img = QImage((const uchar*)m_srcImage.data, m_srcImage.cols, m_srcImage.rows,
				m_srcImage.cols * m_srcImage.channels(), format);
			recvShowPicSignal(img);
		}
	}
		else if (fileName.endsWith(".flv"))
		{
			Mat frame;
			VideoCapture capture;
			if (capture.open(fileName.toStdString())) {
				for (;;)
				{
					capture.read(frame);
					if (frame.data)
					{
						cvtColor(frame, frame, COLOR_BGR2RGB);//BGR转化为RGB
						QImage::Format format = QImage::Format_RGB888;
						switch (frame.type())
						{
						case CV_8UC1:
							format = QImage::Format_Indexed8;
							break;
						case CV_8UC3:
							format = QImage::Format_RGB888;
							break;
						case CV_8UC4:
							format = QImage::Format_ARGB32;
							break;
						}
						QImage img = QImage((const uchar*)frame.data, frame.cols, frame.rows,
							frame.cols * frame.channels(), format);
						recvShowPicSignal(img);
					}

					waitKey(10);
				}
			}


		}

	


}

void opencvQTWidget1::recvShowPicSignal(QImage image) {
	QPixmap convertPixmap = QPixmap::fromImage(image);
	QGraphicsScene* graphicScene = new QGraphicsScene;
	m_pImg = new CustomGraphicImageView(&convertPixmap);
	int mwidth = ui.graphicsView->width();
	int mheight = ui.graphicsView->height();
	m_pImg->setQGraphicViewWH(mwidth, mheight);
	graphicScene->addItem(m_pImg);
	ui.graphicsView->setSceneRect(QRectF(-(mwidth / 2), -(mheight / 2), mwidth, mheight));
	ui.graphicsView->setScene(graphicScene);
	ui.graphicsView->setFocus();

}	