#include "MasterSerial.h"

MasterSerial::MasterSerial(QWidget* parent)
    : QMainWindow(parent),
    m_portLabel(new QLabel(QString::fromLocal8Bit("端口："))),
    m_portComboBox(new QComboBox()),
    m_sendLabel(new QLabel(QString::fromLocal8Bit("发送数据："))),
    m_sendPlainText(new QPlainTextEdit()),
    m_recvLabel(new QLabel(QString::fromLocal8Bit("接收数据："))),
    m_recvPlainText(new QPlainTextEdit()),
    m_btnSend(new QPushButton(QString::fromLocal8Bit("发送"))),
    m_commandComboBox(new QComboBox()),
	m_openPort(new QPushButton(QString::fromLocal8Bit("打开串口")))
{
    ui.setupUi(this);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_portLabel, 0, 0, 1, 1);
    mainLayout->addWidget(m_portComboBox, 0, 1, 1, 1);
	mainLayout->addWidget(m_openPort, 1, 0, 1, 1);
    mainLayout->addWidget(m_sendLabel, 2, 0, 1, 1);
    mainLayout->addWidget(m_commandComboBox, 2, 1, 1, 1);
    mainLayout->addWidget(m_sendPlainText, 3, 0, 2, 2);
    mainLayout->addWidget(m_btnSend, 5, 0, 1, 1);
    mainLayout->addWidget(m_recvLabel, 6, 0, 1, 1);
    mainLayout->addWidget(m_recvPlainText, 7, 0, 2, 2);
    this->centralWidget()->setLayout(mainLayout);
    const auto portInfo = QSerialPortInfo::availablePorts();
    for (auto info : portInfo)
    {
        m_portComboBox->addItem(info.portName());
    }

    for each (QString command in allCmd.datamap)
    {
        m_commandComboBox->addItem(command);
    }

	connect(m_openPort, &QPushButton::clicked, this, [=]() {
		serial.setPortName(m_portComboBox->currentText());
		serial.open(QIODevice::ReadWrite);
		});

    connect(m_commandComboBox, &QComboBox::currentTextChanged, this, [=]() {
		m_sendPlainText->clear();
		BaseLuanchCommand launchCmd;
		QByteArray request;

		auto commandIndex = m_commandComboBox->currentIndex();
		switch (commandIndex)
		{
		case 0:
			request = launchCmd.generalMachinePowerOnRequest();
			break;
		case 1:
			request = launchCmd.generalMachinePowerOffRequest();
			break;
		case 2:
			request = launchCmd.generalMachineSelfCheckRequest();
			break;
		case 3:
			request = launchCmd.flightControlSoftwareStatusQueryRequest();
			break;
		case 4:
			request = launchCmd.singleDistributionRequest();
			break;
		case 5:
			request = launchCmd.intertialGroupDataMeasureRequest();
			break;
		case 6:
			request = launchCmd.gpsDataMeasureRequest();
			break;
		case 7:
			request = launchCmd.servoZeroBindingRequest();
			break;
		case 8:
			request = launchCmd.servoMeasureRequest();
			break;
		case 9:
			request = launchCmd.servoSmallLoopMeasureRequest();
			break;
		case 10:
			request = launchCmd.servoBatteryActivateRequest();
			break;
		case 11:
			request = launchCmd.metaDataControlRequest();
			break;
		case 12:
			request = launchCmd.metaDataUploadRequest();
			break;
		case 13:
			request = launchCmd.metaDataDownloadRequest();
			break;
		case 14:
			request = launchCmd.metaDataCheckRequest();
			break;
		case 15:
			request = launchCmd.metaDataLoadingRequest();
			break;
		case 16:
			request = launchCmd.simulationFlightDataUploadRequest();
			break;
		case 17:
			request = launchCmd.explosivePortTestRequest();
			break;
		case 18:
			request = launchCmd.explosiveResistMeasureRequest();
			break;
		case 19:
			request = launchCmd.levelComputeRequest();
			break;
		case 20:
			request = launchCmd.trunNavigateRequest();
			break;
		case 21:
			request = launchCmd.flightControlSoftwareUploadControlRequest();
			break;
		case 22:
			request = launchCmd.flightControlSoftwareDataUploadRequest();
			break;
		case 23:
			request = launchCmd.flightControlSoftwareDataCheckRequest();
			break;
		case 24:
			request = launchCmd.preLaunchCommandRequest();
			break;
		case 25:
			request = launchCmd.launchFireCommandRequest();
			break;
		case 26:
			request = launchCmd.safeControlPreCommandRequest();
			break;
		case 27:
			request = launchCmd.safeControlActivateCommandRequest();
			break;

		default:
			break;
		}
		m_sendPlainText->clear();
		m_sendPlainText->appendPlainText(request.toHex());
		m_sendData = request;
        });

	connect(&serial, &QSerialPort::readyRead, this, [=]() {
		
		auto recvData = serial.readAll();
		m_recvPlainText->appendPlainText(recvData.toHex());
		});

	connect(m_btnSend, &QPushButton::clicked, this, [=]() {
		
		
		serial.write(m_sendData);
		});
}
