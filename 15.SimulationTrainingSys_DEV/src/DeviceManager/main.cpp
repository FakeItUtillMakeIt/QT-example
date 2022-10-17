#include "UI/DeviceManager.h"
#include "UI/DeviceManagerLogin.h"
#include <QtWidgets/QApplication>
#include <QProcess>
#include "AppCache.h"
#include "../Public/log.h"
#include "../Model/SingleApplication.h"

int main(int argc, char* argv[])
{
#if 1
	SingleApplication app(argc, argv);
	if (!app.isRunning())
	{

		// ��ʼ��������棬ָ��EXE����λ�ã�ͬʱ������־�ļ���
		if (!AppCache::instance()->SetExePath(app.applicationDirPath())) {
			QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("������־�ļ���ʧ��!"));
			app.exit();
		}
		AppCache::instance()->argc = &argc;
		AppCache::instance()->argv = &argv;
		//��ʼ����־
		LogInit(argv[0], app.applicationDirPath().toStdString());
		LOG(INFO) << "����ʼ����... " << endl;

		//��ȡ�����ļ�
		if (!AppCache::instance()->ReadConfig()) {
			QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��ȡ�����ļ�ʱ������ϸ��Ϣ��鿴��־��"));
			app.exit();
		}

		DeviceManager w;
		w.hide();
		DeviceManagerLogin login(&app,&w);
		login.show();


		return app.exec();
	}
	return 0;

#else
	QApplication app(argc, argv);

	// ֻ�����һ�����
	QProcess process;
	process.start("tasklist");
	if (process.waitForFinished()) {
		//�õ������б�
		QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
		if (result.count("TeachingManagement", Qt::CaseInsensitive) >= 2) {
			QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��ѧ�������������!"));
			app.exit();
			return 0;
		}
	}
	else {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��������������ʧ�ܣ��޷�ʶ���Ƿ��������ʵ��!"));
		app.exit();
		return 0;
	}

	// ��ʼ��������棬ָ��EXE����λ�ã�ͬʱ������־�ļ���
	if (!AppCache::instance()->SetExePath(app.applicationDirPath())) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("������־�ļ���ʧ��!"));
		app.exit();
	}
	AppCache::instance()->argc = &argc;
	AppCache::instance()->argv = &argv;
	//��ʼ����־
	LogInit(argv[0], app.applicationDirPath().toStdString());
	LOG(INFO) << "����ʼ����... " << endl;

	//��ȡ�����ļ�
	if (!AppCache::instance()->ReadConfig()) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��ȡ�����ļ�ʱ������ϸ��Ϣ��鿴��־��"));
		app.exit();
	}

	DeviceManager w;
	w.hide();
	DeviceManagerLogin login(&w);
	login.show();


	return app.exec();

#endif
}
