#include "SlaveSerial.h"

SlaveSerial::SlaveSerial(QWidget *parent)
    : QMainWindow(parent),
	m_portLabel(new QLabel(QString::fromLocal8Bit("端口："))),
	m_portComboBox(new QComboBox()),
	m_sendLabel(new QLabel(QString::fromLocal8Bit("发送数据："))),
	m_sendPlainText(new QPlainTextEdit()),
	m_recvLabel(new QLabel(QString::fromLocal8Bit("接收数据："))),
	m_recvPlainText(new QPlainTextEdit()),
	m_btnSend(new QPushButton(QString::fromLocal8Bit("发送"))),
	m_openPort(new QPushButton(QString::fromLocal8Bit("打开串口")))
{
    ui.setupUi(this);

	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->addWidget(m_portLabel, 0, 0, 1, 1);
	mainLayout->addWidget(m_portComboBox, 0, 1, 1, 1);
	mainLayout->addWidget(m_openPort, 1, 0, 1, 1);
	mainLayout->addWidget(m_sendLabel, 2, 0, 1, 1);
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

	connect(m_openPort, &QPushButton::clicked, this, [=]() {
		serial.setPortName(m_portComboBox->currentText());
		serial.open(QIODevice::ReadWrite);
		});

	connect(&serial, &QSerialPort::readyRead, this, [=]() {
		m_recvPlainText->clear();
		
		auto recvData = serial.readAll();

		if (recvData.size() < 263)
		{
			QMessageBox::warning(this, "warning", QString::fromLocal8Bit("传输数据不完整"));
			return;
		}
		RocketGroundBaseProtol protolParser;
		QByteArray rets;
		auto dataLen = protolParser.parseRocketGroundProtol(recvData, rets);
		if (dataLen > 0)
		{
			m_recvPlainText->appendPlainText(recvData.toHex());
			auto responseData = protolParser.parseProtolRequest(rets);
			if (responseData.isNull())
			{
				QMessageBox::warning(this, "warning", QString::fromLocal8Bit("无此事件处理"));
				return;
			}
			m_sendData = responseData;
			m_sendPlainText->clear();
			m_sendPlainText->appendPlainText(m_sendData.toHex());
		}


		});

	connect(m_btnSend, &QPushButton::clicked, this, [=]() {
		
		serial.write(m_sendData);
		});
}
