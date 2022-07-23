#include "CenterOperate.h"

#define FRAMELENGTH 10
CenterOperate::CenterOperate(QWidget* parent)
	: QWidget(parent)
{

	ui.setupUi(parent);
	m_app = AppCache::instance();
	m_pResponseSenderSocket = new QUdpSocket(this);
	m_pYaoCeSenderSocket = new QUdpSocket(this);

	Init();
}
CenterOperate::~CenterOperate()
{
	if (m_pResponseSenderSocket != nullptr)
	{
		m_pResponseSenderSocket->close();
		delete m_pResponseSenderSocket;
		m_pResponseSenderSocket = nullptr;
	}
	if (m_pYaoCeSenderSocket != nullptr)
	{
		m_pYaoCeSenderSocket->close();
		delete m_pYaoCeSenderSocket;
		m_pYaoCeSenderSocket = nullptr;
	}
	m_pYcTimer->stop();


}

void CenterOperate::Init()
{
	m_pReceiveCMDData = new ReceiveCMDData();
	connect(m_pReceiveCMDData, &ReceiveCMDData::receiverCMD, this, &CenterOperate::receiverCMD);

	m_pYcTimer = new QTimer();
	connect(m_pYcTimer, &QTimer::timeout, this, &CenterOperate::sendRocketData);//发送箭上数据
	m_pYcTimer->start(50);

	configOperateName << QString("参数管理") << QString("设备管理") << QString("指令管理") << QString("火箭型号管理");
	//添加设备管理器界面布局
	InitUILayout();
}

void CenterOperate::receiverCMD(QVariant oneCommand)
{
	Command* command = oneCommand.value<Command*>();
	//发送回令
	sendCMDResponse(5);
	QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), QString("收到测发指令,code等于%1！").arg(command->m_iCode));
}

/// <summary>
/// 发送测发回令
/// </summary>
/// <param name="cmd_id"></param>
void CenterOperate::sendCMDResponse(int cmd_id)
{
	QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
	m_pBuff[0] = 0x55;
	m_pBuff[1] = 0xBB;
	m_pBuff[2] = 0x02; //测发回令指令code
	m_pBuff[3] = 0x01; //参数，执行成功
	m_pBuff[4] = 0x00;
	m_pBuff[5] = 0x00;
	m_pBuff[6] = 0x00;
	m_pBuff[7] = 0x00;
	m_pBuff[8] = 0x00;
	m_pBuff[9] = 0x00;
	unsigned char vdata1[FRAMELENGTH];
	for (int i = 0; i < FRAMELENGTH; i++)
	{
		vdata1[i] = (unsigned char)m_pBuff[i];
	}
	unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
	memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);
	m_pResponseSenderSocket->writeDatagram(m_pBuff, QHostAddress(m_app->m_responseSender->m_strIP.c_str()), m_app->m_responseSender->m_iPort);
}


/// <summary>
/// 发送箭上数据
/// </summary>
void CenterOperate::sendRocketData()
{
	QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
	m_pBuff[0] = 0x55;
	m_pBuff[1] = 0xAA;
	m_pBuff[3] = 0x02; //测发指令code
	m_pBuff[4] = 0x00;
	m_pBuff[5] = 0x00;
	unsigned char vdata1[FRAMELENGTH];
	for (int i = 0; i < FRAMELENGTH; i++)
	{
		vdata1[i] = (unsigned char)m_pBuff[i];
	}
	unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
	memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);
	m_pYaoCeSenderSocket->writeDatagram(m_pBuff, QHostAddress(m_app->m_yaoCeSender->m_strIP.c_str()), m_app->m_yaoCeSender->m_iPort);
}



/**
	@brief 管理器软件主界面布局
**/
void CenterOperate::InitUILayout() {

	//this->setMouseTracking(true);
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();
	QFont font;
	font.setBold(true);
	font.setPixelSize(16);
	QIcon icon(":/icon/icon/circled-right.png");
	QIcon iconClick(":/icon/icon/squareWl.png");
	QIcon iconRelease(":/icon/icon/squareBl.png");
	for each (QString configName in configOperateName)
	{
		QListWidgetItem* item = new QListWidgetItem(configName);
		item->setBackgroundColor(QColor(85, 170, 255));
		item->setSizeHint(QSize(ui.listWidget->width(), 50));
		item->setIcon(iconRelease);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(font);

		ui.listWidget->addItem(item);
	}
	ui.listWidget->setFocusPolicy(Qt::NoFocus);

	QString widgetStyleSheet = wss->listStyleSheet.arg(QString("QListWidget"));
	QString itemStyleSheet = wss->listItemStyleSheet.arg(QString("QListWidget"));
	QString listStyleSheet = widgetStyleSheet + itemStyleSheet;

	ui.listWidget->setStyleSheet(listStyleSheet);
	ui.listWidget->setContentsMargins(0, 0, 0, 0);

	ui.listWidget->setLayoutDirection(Qt::LeftToRight);
	ui.listWidget->horizontalScrollBar()->hide();
	ui.listWidget->verticalScrollBar()->hide();

	paramManageUI = new ParamManageModule();
	ui.gridLayout->addWidget(paramManageUI, 0, 1);
	deviceManageUI = new DeviceManageModule();
	ui.gridLayout->addWidget(deviceManageUI, 0, 2);
	commandManageUI = new CommandManageModule();
	ui.gridLayout->addWidget(commandManageUI, 0, 3);
	rocketManageUI = new RocketTypeManageModule();
	ui.gridLayout->addWidget(rocketManageUI, 0, 4);
	paramManageUI->hide();
	deviceManageUI->hide();
	commandManageUI->hide();
	rocketManageUI->hide();
	//


	connect(ui.listWidget, &QListWidget::itemPressed, this, [=]() {
		ui.widget->close();

		for (int i = 0; i < ui.listWidget->count(); i++)
		{
			ui.listWidget->item(i)->setIcon(iconRelease);
		}
		ui.listWidget->currentItem()->setIcon(iconClick);

		switch (ui.listWidget->currentRow())
		{
		case 0:
			//呼出对应属性窗口

			paramManageUI->show();
			deviceManageUI->hide();
			commandManageUI->hide();
			rocketManageUI->hide();

			break;
		case 1:
			paramManageUI->hide();
			deviceManageUI->show();
			commandManageUI->hide();
			rocketManageUI->hide();

			break;
		case 2:
			paramManageUI->hide();
			deviceManageUI->hide();
			commandManageUI->show();
			rocketManageUI->hide();
			break;
		case 3:
			paramManageUI->hide();
			deviceManageUI->hide();
			commandManageUI->hide();
			rocketManageUI->show();
			break;
		default:
			break;
		}


		});



	connect(paramManageUI, &ParamManageModule::changed, this, []() {
		qDebug() << "param changed";
		});

	//默认显示
	ui.listWidget->setCurrentItem(ui.listWidget->item(1));

	emit ui.listWidget->itemPressed(ui.listWidget->currentItem());

}



//void CenterOperate::paintEvent(QPaintEvent* event) {
//	qDebug() << ui.widget->width();
//
//	QTableWidget* displayWidget;
//
//	switch (ui.listWidget->currentRow())
//	{
//	case 0:
//		//呼出对应属性窗口
//		displayWidget = paramManageUI->configInfoTable;
//		break;
//	case 1:
//		displayWidget = deviceManageUI->configInfoTable;
//		break;
//	case 2:
//		displayWidget = commandManageUI->configInfoTable;
//		break;
//	case 3:
//		displayWidget = rocketManageUI->configInfoTable;
//		break;
//	default:
//		displayWidget = nullptr;
//		break;
//	}
//
//	if (displayWidget)
//	{
//		qDebug() << this->width() << this->parentWidget()->width();
//		for (int columnIndex = 0; columnIndex < displayWidget->columnCount(); columnIndex++)
//		{
//			//displayWidget->setColumnWidth(columnIndex, (this->parentWidget()->width()) / (displayWidget->columnCount() + 1));
//			displayWidget->setColumnWidth(columnIndex, (displayWidget->width()) / (displayWidget->columnCount()));
//		}
//	}
//
//}

