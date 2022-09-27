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
		//日期特殊处理
		QDate date = ui.ExpiryDate->date();
		QString strDate = date.toString("yyyy/MM/dd");//
		//进行加密字符串<用户名-IP-SYSID- - -DATA-NUMBER-TYPE>
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

		//整理详细信息
		QString fuhao = "=";
		QString type = ui.LicenseType->currentText();		//单机-网络
		QString edate = strDate;								//授权终止日期
		QString time = QTime::currentTime().toString();		//时间
		QString num = QString::number(ui.spinBox_number->value());	//用户数量
		QString sdate = QDate::currentDate().toString("yyyy/MM/dd");//授权起始时间(为生成授权文件时间)
		QString strData = list.at(0) + fuhao + type + fuhao + sdate + fuhao + time + fuhao + num + fuhao + edate + fuhao;

		//使用Qt自带加密算法
		QByteArray hashData = QCryptographicHash::hash(lic.toLocal8Bit(), QCryptographicHash::Sha3_512);
		//返回字节数组的十六进制编码，编码使用数字0-9和字母a-f
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
	ui.SystemInformationFileBrowser->setText(QString::fromLocal8Bit("电脑名:") + n + macName + n + QString::fromLocal8Bit("主板ID号:") + n + biosId.split(" ")[0] + n + QString::fromLocal8Bit("CPUID号:") + n + cpuId.split(" ")[0]);
	list.push_back(macName);
	list.push_back(biosId.split(" ")[0]);
	list.push_back(cpuId.split(" ")[0]);

	for (int addrIndex = 0; addrIndex < macAddrs.size(); addrIndex++)
	{
		ui.SystemInformationFileBrowser->append(QString::fromLocal8Bit("IP地址:") + n + ipAddrs[addrIndex] + n + QString::fromLocal8Bit("物理地址:") + n + macAddrs[addrIndex]);
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
				//mac地址ip不能为127.0.0.1 为ipv4
				if (interface1.addressEntries().at(i).ip() != QHostAddress::LocalHost
					&& interface1.addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
				{
					if (interface1.addressEntries().at(i).ip().toString().startsWith("192"))
					{
						continue;
					}
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
				//mac地址ip不能为172.0.0.1 为ipv4
				if (interface1.addressEntries().at(i).ip() != QHostAddress::LocalHost
					&& interface1.addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
				{
					if (interface1.addressEntries().at(i).ip().toString().startsWith("192"))
					{
						continue;
					}
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