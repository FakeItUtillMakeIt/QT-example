#include "LicenseManagement.h"

LicenseManagement::LicenseManagement(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	init();

}

LicenseManagement::~LicenseManagement()
{}


void LicenseManagement::init()
{
	connect(ui.pushButton, &QPushButton::clicked, this, &LicenseManagement::slotBrowse);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &LicenseManagement::slotGenerator);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &LicenseManagement::slotCancel);
	ui.ExpiryDate->setDate(QDate::currentDate());
}

void LicenseManagement::slotGenerator()
{
	if (list.size() == 0)
		return;
	QString file_name = QFileDialog::getSaveFileName(NULL, "Save File", "", "XML (*.lic)");
	QFile file(file_name);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		//�������⴦��
		QDate date = ui.ExpiryDate->date();
		QString strDate = date.toString("yyyy/MM/dd");
		//���м����ַ���<�û���-IP-SYSID- - -DATA-NUMBER-TYPE>
		QString lic;
		for (auto ele : list)
		{
			lic += ele;
		}
		lic +=
			strDate +
			QString::number(ui.spinBox_number->value()) +
			ui.LicenseType->currentText() +
			QDate::currentDate().toString("yyyy/MM/dd");

		//������ϸ��Ϣ
		QString fuhao = "=";
		QString type = ui.LicenseType->currentText();		//����-����
		QString edate = strDate;								//��Ȩ��ֹ����
		QString time = QTime::currentTime().toString();		//ʱ��
		QString num = QString::number(ui.spinBox_number->value());	//�û�����
		QString sdate = QDate::currentDate().toString("yyyy/MM/dd");//��Ȩ��ʼʱ��(Ϊ������Ȩ�ļ�ʱ��)
		QString strData = list.at(0) + fuhao + type + fuhao + sdate + fuhao + time + fuhao + num + fuhao + edate + fuhao;

		//ʹ��Qt�Դ������㷨
		QByteArray hashData = QCryptographicHash::hash(lic.toLocal8Bit(), QCryptographicHash::Sha3_512);
		//�����ֽ������ʮ�����Ʊ��룬����ʹ������0-9����ĸa-f
		lic.clear();
		lic = hashData.toHex();
		strData += lic;
		file.write(strData.toUtf8());
	}
	file.close();
}

void LicenseManagement::slotCancel()
{
	close();
}

void LicenseManagement::slotBrowse()
{
	QString n = "\n";
	auto macAddrs = getLocalMacAddress();
	auto ipAddrs = getLocalIp();
	auto macName = getSysInfo();
	getReleInfo();
	ui.SystemInformationFileBrowser->setText(QString::fromLocal8Bit("������:") + n + macName + n + QString::fromLocal8Bit("����ID��:") + n + biosId.split(" ")[0] + n + QString::fromLocal8Bit("CPUID��:") + n + cpuId.split(" ")[0]);
	list.push_back(macName);
	list.push_back(biosId.split(" ")[0]);
	list.push_back(cpuId.split(" ")[0]);

	for (int addrIndex = 0; addrIndex < macAddrs.size(); addrIndex++)
	{
		ui.SystemInformationFileBrowser->append(QString::fromLocal8Bit("IP��ַ:") + n + ipAddrs[addrIndex] + n + QString::fromLocal8Bit("�����ַ:") + n + macAddrs[addrIndex]);
		list.push_back(ipAddrs[addrIndex]);
		list.push_back(macAddrs[addrIndex]);
	}
}

QStringList LicenseManagement::getLocalMacAddress() {
	QStringList macAdds;
	for (auto interface1 : QNetworkInterface::allInterfaces())
	{

		if (interface1.flags().testFlag(QNetworkInterface::IsUp) &&
			interface1.flags().testFlag(QNetworkInterface::IsRunning) &&
			!interface1.flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			for (int i = 0; i < interface1.addressEntries().size(); i++)
			{
				//mac��ַip����Ϊ172.0.0.1 Ϊipv4
				if (interface1.addressEntries().at(i).ip() != QHostAddress::LocalHost
					&& interface1.addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
				{
					macAdds.push_back(interface1.hardwareAddress());
				}
			}
		}

	}
	return macAdds;
}

QStringList LicenseManagement::getLocalIp() {
	QStringList ipAddrs;
	for (auto interface1 : QNetworkInterface::allInterfaces())
	{

		if (interface1.flags().testFlag(QNetworkInterface::IsUp) &&
			interface1.flags().testFlag(QNetworkInterface::IsRunning) &&
			!interface1.flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			for (int i = 0; i < interface1.addressEntries().size(); i++)
			{
				//mac��ַip����Ϊ172.0.0.1 Ϊipv4
				if (interface1.addressEntries().at(i).ip() != QHostAddress::LocalHost
					&& interface1.addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
				{
					ipAddrs.push_back(interface1.addressEntries().at(i).ip().toString());
				}
			}
		}

	}
	return ipAddrs;
}

QString LicenseManagement::getSysInfo() {
	macHostName = QSysInfo::machineHostName();
	return macHostName;
}

void LicenseManagement::getReleInfo() {
	QProcess p;

	p.start("wmic cpu get processorid");
	p.waitForFinished();
	cpuId = QString::fromLocal8Bit(p.readAllStandardOutput()).split("\n")[1];

	p.start("wmic baseboard get serialnumber");
	p.waitForFinished();
	biosId = QString::fromLocal8Bit(p.readAllStandardOutput()).split("\n")[1];


}