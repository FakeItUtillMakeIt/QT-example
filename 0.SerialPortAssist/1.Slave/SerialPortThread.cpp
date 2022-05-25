#include "SerialPortThread.h"

SerialPortThread::SerialPortThread(QObject* parent /* = nullptr */):
	QThread(parent)
{

}

SerialPortThread::~SerialPortThread() {
	m_mutex.lock();
	m_quit = true;
	m_mutex.unlock();
	wait();
}


void SerialPortThread::startSerialPort(const QString& portName, int waitTimeout,const QString& response) {
	const QMutexLocker locker(&m_mutex);
	m_portname = portName;
	m_waitTimeout = waitTimeout;
	m_response = response;
	if (!isRunning())
	{
		start();
	}

}

/**
    @brief 从缓冲器中读取数据和写入数据
**/
void SerialPortThread::run() {
	bool currentPortNameChanged = false;
	m_mutex.lock();
	QString currentPortName;
	if (currentPortName!=m_portname)
	{
		currentPortName = m_portname;
		currentPortNameChanged = true;
	}
	int currentWaitTimeout = m_waitTimeout;
	QString currentResponse=m_response;
	m_mutex.unlock();

	QSerialPort serial;
	while (!m_quit)
	{
		if (currentPortNameChanged)
		{
			serial.close();
			serial.setPortName(currentPortName);
			if (!serial.open(QIODevice::ReadWrite))
			{
				emit error(tr("can not open %1,error code %2")
					.arg(m_portname).arg(serial.error()));
				return;
			}
		}
		//serial.write("nihao");
		auto read=serial.waitForReadyRead(currentWaitTimeout);
		if (read)
		{
			//读取请求
			//这里添加数据处理
			QByteArray requestData = serial.readAll();
			while (serial.waitForReadyRead(10))
			{
				requestData += serial.readAll();
			}
			emit this->request(requestData);
			//此处可加入写入
			const QByteArray responseData = m_response.toUtf8();
			serial.write(responseData);
			if (serial.waitForBytesWritten(m_waitTimeout))
			{
				const QString request = QString::fromUtf8(requestData);
				emit this->request(requestData);
			}
			else
			{
				emit timeout(tr("wait time response timeout %1")
					.arg(QTime::currentTime().toString()));
			}
		}
		else
		{
			emit timeout(tr("wait read request time out %1")
				.arg(QTime::currentTime().toString()));
		}

		m_mutex.lock();
		if (currentPortName!=m_portname)
		{
			currentPortName = m_portname;
			currentPortNameChanged = true;
		}
		else
		{
			currentPortNameChanged = false;
		}
		currentWaitTimeout = m_waitTimeout;
		currentResponse = m_response;
		m_mutex.unlock();

	}
}