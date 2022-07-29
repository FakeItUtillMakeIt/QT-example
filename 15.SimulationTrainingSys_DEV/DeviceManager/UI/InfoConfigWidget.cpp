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
	@brief 根据不同模块初始化配置布局
**/
void InfoConfigWidget::InitUILayout() {
	//this->setFixedSize(640, 480);
	wss = WidgetStyleSheet::getInstace();

	configInfoHeader = new QLabel(QString("XXX配置"));
	configNameLabel = new QLabel(QString("名称:"));
	userInputName = new QLineEdit();
	configTypeLabel = new QLabel(QString("类型:"));
	userSelectType = new QComboBox();
	configRetCMDLabel = new QLabel(QString("回令:"));
	userSelectRetCMD = new QComboBox();
	configPrefixLabel = new QLabel(QString("前缀:"));
	userInputPrefix = new QLineEdit();
	configEncodeLabel = new QLabel(QString("编码:"));
	userInputEncode = new QLineEdit();
	configDevInRocketLabel = new QLabel(QString("设备所属火箭型号:"));
	userSlctDevInRocket = new QComboBox();
	userSlctDevStatBindIDLabel = new QLabel(QString("设备状态ID:"));
	userSlctDevStatBindID = new QComboBox();
	configDevStatBindFileLabel = new QLabel(QString("设备状态文件:"));
	userSlctDevStatBindFile = new QLabel();
	userSlctDevStatBindBtn = new QPushButton(QString("选择绑定文件"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 7;
	int columnCount = 4;
	int row = 0;
	int column = 0;
	//第0行
	infoUILayout->addWidget(configInfoHeader, row++, column, 1, columnCount, Qt::AlignAbsolute);
	//第一行
	infoUILayout->addWidget(configNameLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configTypeLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectType, row++ % rowCount, column++ % columnCount, 1, 1);
	//第二行
	infoUILayout->addWidget(configRetCMDLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectRetCMD, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configPrefixLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputPrefix, row++ % rowCount, column++ % columnCount, 1, 1);
	//第三行
	infoUILayout->addWidget(configEncodeLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputEncode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configDevInRocketLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevInRocket, row++ % rowCount, column++ % columnCount, 1, 1);
	//第四行
	infoUILayout->addWidget(userSlctDevStatBindIDLabel, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctDevStatBindID, row++ % rowCount, column++ % columnCount, 1, 1);
	//第五行
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