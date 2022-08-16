#include "MutliThreadTest.h"

MutliThreadTest::MutliThreadTest(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QThreadPool::globalInstance()->setMaxThreadCount(1);

	//给子线程传值
	connect(this, &MutliThreadTest::transParams1, &work[0], &MutliThread::recvNum);
	connect(this, &MutliThreadTest::transParams2, &work[1], &MutliThread::recvNum);
	connect(this, &MutliThreadTest::transParams3, &work[2], &MutliThread::recvNum);

	//接收子线程发送的数据，更新进度条
	connect(&work[0], &MutliThread::working, this, [=](int l) {
		ui.progressBar->setValue(l);
		ui.progressBar->update();
		});

	connect(&work[0], &MutliThread::working, this, [=](int l) {
		ui.progressBar_2->setValue(l);
		ui.progressBar_2->update();
		});

	connect(&work[0], &MutliThread::working, this, [=](int l) {
		ui.progressBar_3->setValue(l);
		ui.progressBar_3->update();
		});

	connect(&work[0], &MutliThread::finish, this, [=](QString s) {
		ui.lineEdit->setText(s);
		returnVal[0] = s.toInt();
		if (returnVal[1] > 0)
		{
			ui.progressBar_3->setMaximum(returnVal[0] + returnVal[1]);
			emit transParams3(returnVal[0] + returnVal[1]);
			QThreadPool::globalInstance()->start(&work[2]);
		}
		});

	connect(&work[1], &MutliThread::finish, this, [=](QString s) {
		ui.lineEdit_2->setText(s);
		returnVal[1] = s.toInt();
		if (returnVal[0] > 0)
		{
			ui.progressBar_3->setMaximum(returnVal[0] + returnVal[1]);
			emit transParams3(returnVal[0] + returnVal[1]);
			QThreadPool::globalInstance()->start(&work[2]);
		}
		});

	connect(&work[2], &MutliThread::finish, this, [=](QString s) {
		ui.lineEdit_2->setText(s);
		});

	connect(ui.startButton, &QPushButton::clicked, this, [=]() {
		ui.progressBar->setMaximum(1000);
		ui.progressBar_2->setMaximum(2000);
		emit transParams1(1000);
		emit transParams2(2000);
		QThreadPool::globalInstance()->start(&work[0]);
		QThreadPool::globalInstance()->start(&work[1]);
		});
}

MutliThreadTest::~MutliThreadTest()
{
	delete[] work;
	QThreadPool::globalInstance()->waitForDone();
	delete& ui;
}

void MutliThreadTest::onStartBuutonClk() {

	for (int i = 0; i < 2; i++) {
		returnVal[i] = 0;
	}

	ui.progressBar->setValue(0);
	ui.lineEdit->setText(0);
	ui.progressBar_2->setValue(0);
	ui.lineEdit_2->setText(0);
	ui.progressBar_3->setValue(0);
	ui.lineEdit_3->setText(0);
}


