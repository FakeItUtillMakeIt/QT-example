#include "RocketGroundComProtol.h"
#include "RocketGroundBaseProtol.h"

RocketGroundComProtol::RocketGroundComProtol(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    initSerialWindow();
	connect(ui.startSerialBtn, &QPushButton::clicked, this, &RocketGroundComProtol::startSerialPort);
	connect(&m_thread, &SerialPortThread::error, this, &RocketGroundComProtol::processError);
	connect(&m_thread, &SerialPortThread::timeout, this, &RocketGroundComProtol::processTimeout);
	//connect(&m_thread, &SerialPortThread::request, this, &RocketGroundComProtol::showRequest);
	connect(&m_thread,QOverload<const QByteArray&>::of(&SerialPortThread::request), this, &RocketGroundComProtol::showRequest);
	connect(ui.serialPortNameCx, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		this, &RocketGroundComProtol::activateRunButton);
	//connect(ui.timeOutSet, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &RocketGroundComProtol::activateRunButton);
	
	connect(ui.timeOutSet, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RocketGroundComProtol::activateRunButton);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &RocketGroundComProtol::startSerialPort);
}


void RocketGroundComProtol::initSerialWindow() {
	
	const auto	infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo& info:infos)
	{
		ui.serialPortNameCx->addItem(info.portName());
	}
	ui.timeOutSet->setRange(0, 10000);
	ui.timeOutSet->setValue(10000);

    
}

/**
    @brief 返回从串口拿出来的数据
    @param s 串口拿出来的数据
**/
//void RocketGroundComProtol::showRequest(const QString& s) {
//	ui.dataDisplay->appendHtml(s);
//	ui.trafficDisplay->setText(tr("traffic :\n\r-response:%1")
//						.arg(ui.lineEdit->text()));
//
//}

void RocketGroundComProtol::showRequest(const QByteArray& s) {
	ui.dataDisplay->appendHtml(s);
	ui.trafficDisplay->setText(tr("traffic :\n\r-response:%1")
		.arg(ui.lineEdit->text()));

}

void RocketGroundComProtol::processTimeout(const QString& s) {
	
	ui.statusDisplay->setText(tr("Not Running,%1").arg(s));

}

void RocketGroundComProtol::processError(const QString& s) {
	activateRunButton();
	ui.statusDisplay->setText(tr("Not Running,%1").arg(s));
}

void RocketGroundComProtol::startSerialPort() {

	ui.startSerialBtn->setEnabled(false);
	ui.statusDisplay->setText(tr("Running,connected to serial Port:%1")
		.arg(ui.serialPortNameCx->currentText()));

	m_thread.startSerialPort(ui.serialPortNameCx->currentText(), ui.timeOutSet->value(),ui.lineEdit->text());
}


void RocketGroundComProtol::activateRunButton() {

	ui.startSerialBtn->setEnabled(true);
}

//协议测试
void RocketGroundComProtol::protolTest() {
	RocketGroundBaseProtol* dataProtolPacket = new RocketGroundBaseProtol;
	byte inputBuf[263];
	byte outputBuf[259];
	inputBuf[0] = 0x55;
	inputBuf[1] = 0xAA;
	inputBuf[2] = 0xFF;
	inputBuf[3] = 0x01;
	inputBuf[4] = 0x01;
	inputBuf[5] = 0x01;
	for (int i = 0; i < 255; i++)
	{
		inputBuf[6 + i] = 0x02;
	}
	CRC16 crc(CRC16::eCCITT_FALSE);
	quint16 crc_cal = crc.crcCompute((char*)(inputBuf + 6), 255);
	inputBuf[6 + 255] = crc_cal;
	inputBuf[6 + 255 + 1] = crc_cal >> 8;

	dataProtolPacket->parseRocketGroundProtol(inputBuf, outputBuf);
}


