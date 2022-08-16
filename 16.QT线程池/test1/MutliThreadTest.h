#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MutliThreadTest.h"
#include "MutliThread.h"
#include <QThread>
#include <QThreadPool>

class MutliThreadTest : public QMainWindow
{
	Q_OBJECT

public:
	MutliThreadTest(QWidget* parent = nullptr);
	~MutliThreadTest();

private:
	Ui::MutliThreadTestClass ui;

	MutliThread* work = new MutliThread[3];
	MutliThread* task = new MutliThread;

	int returnVal[2] = { 0 };

private slots:
	void onStartBuutonClk();

signals:
	void transParams1(int num);
	void transParams2(int num);
	void transParams3(int num);


};
