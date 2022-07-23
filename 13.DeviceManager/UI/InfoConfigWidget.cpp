#include "InfoConfigWidget.h"

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{

	InitUILayout();
}

InfoConfigWidget::~InfoConfigWidget()
{

}

/**
	@brief ���ݲ�ͬģ���ʼ�����ò���
**/
void InfoConfigWidget::InitUILayout() {
	//this->setFixedSize(640, 480);
	wss = WidgetStyleSheet::getInstace();

	configInfoHeader = new QLabel(QString("XXX����"));
	configNameLabel = new QLabel(QString("����:"));
	userInputName = new QLineEdit();
	configTypeLabel = new QLabel(QString("����:"));
	userSelectType = new QComboBox();
	configRetCMDLabel = new QLabel(QString("����:"));
	userSelectRetCMD = new QComboBox();
	configPrefixLabel = new QLabel(QString("ǰ׺:"));
	userInputPrefix = new QLineEdit();
	configEncodeLabel = new QLabel(QString("����:"));
	userInputEncode = new QLineEdit();
	configDevInRocketLabel = new QLabel(QString("�豸��������ͺ�:"));
	userSlctDevInRocket = new QComboBox();
	userSlctDevStatBindIDLabel = new QLabel(QString("�豸״̬ID:"));
	userSlctDevStatBindID = new QComboBox();
	configDevStatBindFileLabel = new QLabel(QString("�豸״̬�ļ�:"));
	userSlctDevStatBindFile = new QLabel();
	userSlctDevStatBindBtn = new QPushButton(QString("ѡ����ļ�"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 7;
	int columnCount = 4;
	int row = 0;
	int column = 0;
	//��0��
	infoUILayout->addWidget(configInfoHeader, row++, column, 1, columnCount, Qt::AlignAbsolute);
	//��һ��
	infoUILayout->addWidget(configNameLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configTypeLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectType, row++ % rowCount, column++ % columnCount, 1, 1);
	//�ڶ���
	infoUILayout->addWidget(configRetCMDLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectRetCMD, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configPrefixLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputPrefix, row++ % rowCount, column++ % columnCount, 1, 1);
	//������
	infoUILayout->addWidget(configEncodeLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputEncode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configDevInRocketLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevInRocket, row++ % rowCount, column++ % columnCount, 1, 1);
	//������
	infoUILayout->addWidget(userSlctDevStatBindIDLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevStatBindID, row++ % rowCount, column++ % columnCount, 1, 1);
	//������
	column = 0;
	infoUILayout->addWidget(configDevStatBindFileLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevStatBindFile, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevStatBindBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	this->setLayout(infoUILayout);
	QString qss = wss->labelStyleSheet.arg("QLabel") + wss->lineEditStyleSheet.arg("QLineEdit") + wss->widgetStyleSheet.arg("QPushButton");
	this->setAttribute(Qt::WA_StyledBackground);

	this->setAutoFillBackground(true);
	this->setStyleSheet(qss);

	userInputName->setStyleSheet("background-color:rgb(250,250,250);");
	configInfoHeader->setStyleSheet("background-color:rgb(240,240,240);font:bold 40px;height:40px;");
	configNameLabel->setStyleSheet("background-color:rgb(240,240,240);");
	configTypeLabel->setStyleSheet("background-color:rgb(240,240,240);");
	configRetCMDLabel->setStyleSheet("background-color:rgb(240,240,240);");

	configPrefixLabel->setStyleSheet("background-color:rgb(240,240,240);");
	userInputPrefix->setStyleSheet("background-color:rgb(250,250,250);");
	configEncodeLabel->setStyleSheet("background-color:rgb(240,240,240);");
	userInputEncode->setStyleSheet("background-color:rgb(250,250,250);");
	configDevInRocketLabel->setStyleSheet("background-color:rgb(240,240,240);");
	userSlctDevInRocket->setStyleSheet("height:20px;");
	userSlctDevStatBindID->setStyleSheet("height:20px;");
	userSelectRetCMD->setStyleSheet("height:20px;");
	userSelectType->setStyleSheet("height:20px;");
	userSlctDevStatBindIDLabel->setStyleSheet("background-color:rgb(240,240,240);");
	configDevStatBindFileLabel->setStyleSheet("background-color:rgb(240,240,240);");
	userSlctDevStatBindFile->setStyleSheet("background-color:rgb(173,216,230);border-radius:10px;");
	userSlctDevStatBindBtn->setStyleSheet("height:20px;border-radius:10px;");

}


void InfoConfigWidget::setConfigHeader(QString header) {
	configInfoHeader->setText(header);
}