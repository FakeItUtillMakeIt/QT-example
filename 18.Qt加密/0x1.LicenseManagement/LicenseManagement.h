#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LicenseManagement.h"
#include <QtWidgets>
#include <QtNetwork/QNetworkInterface>

class LicenseManagement : public QMainWindow
{
	Q_OBJECT

public:
	LicenseManagement(QWidget* parent = nullptr);
	~LicenseManagement();

	void init();
	QStringList getLocalMacAddress();
	QStringList getLocalIp();
	QString getSysInfo();
	void getReleInfo();

private:
	Ui::LicenseManagementClass ui;
	QStringList list;
	QString macHostName;
	QString cpuId;
	QString biosId;

private slots:
	void slotBrowse();
	void slotGenerator();
	void slotCancel();
};
